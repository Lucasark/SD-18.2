#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "mpi.h"

int checkForKey(){
	return 0;
}//Deve ser implementado,pode ter quantos parametros quiser,ou ser feito la no for que eh chamado tanto faz.


int main(int argc,char** argv){

	int meu_rank,np,tag=0;
	int to=0;
	MPI_Status status;
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&meu_rank);
	MPI_Comm_size(MPI_COMM_WORLD,&np);

	int orig_tam=0; 		// Eh o quanto da string cada processo pegarah sem dar overlap. O tamanho do Overlap chave_tam-1
	int resto=0; 			// O que sobrar da divisao  da String quando for separada para os processos.
	int block_tam=0; 		// tamanho (orig_tam + chave_tam-1), chamaremos de bloco o que cada processo receberah pra processar
	int nt = np-1; 			// numero de trabalhadores uma vez que o processo 0 serah mestre.

	int genes_tam=0;		//TOdos podem iniciar essas duas
	int chave_tam=0;

	if(meu_rank==0){
		/*
		IO
		Aqui vai o programa que recupera a string da chave e do genes (sequencia genetica), e deve guardar essas informacoes
		nas variaveis logo abaixo (que serao char*).
		*/
		char* chave; char* genes;

		genes_tam = sizeof(genes)/sizeof(char*);
		chave_tam = sizeof(chave)/sizeof(char*);

		orig_tam = genes_tam / nt;						 //dividindo a sequencia genetica entre os processos,sem o overlay
		resto = genes_tam % nt; 						//de fato soh pegando o quanto vai restar da divisao.
		block_tam = orig_tam + (chave_tam-1);			//total que serah recebido por casa processo

		char *block=(char *)malloc(block_tam*sizeof(char));		//criando espaco para enviar os genes necessarios

		int init_index=0;
		for (to = 1; to < nt; to++){//agora enviaremos para cada processo a chave,enviarah a parte necessaria da string de genes, o resto eh deduzivel  (menos mensagem)

			//strncpy(dest, src + init_index, block_tam);Destino,fala qual array comeca

			strncpy(block,genes + init_index, block_tam);  //pega o que eh relevante pro processo da vez e coloca em block

			//Envia a chave
			MPI_Send(chave_tam, 1, MPI_INT, to, tag, MPI_COMM_WORLD);
			//ja equivale a strlen(block)+1,fica menos processamento

			//Envia o tamanho do Bloco
			MPI_Send(block_tam, 1, MPI_INT, to, tag, MPI_COMM_WORLD);
			//ja equivale a strlen(block)+1,fica menos processamento

			//Envia a chave
			MPI_Send(chave, chave_tam+1, MPI_CHAR, to, tag, MPI_COMM_WORLD);

			//Envia o parametro do block (init)
			MPI_Send(block, block_tam+1, MPI_CHAR, to, tag, MPI_COMM_WORLD);


			init_index += chave_tam;		//update de onde comeca os "genes" que serao enviados
		 }

		//aqui eh o envio apenas para o ultimo processo
		//obs perceba que o ultimo eh nt, pois se tivessemos 6 processos,np=6 ,nt=5, e os processos sao 0,1,2,3,4,5. OU seja nt eh o ultimo

		to = nt;

		block_tam=orig_tam+resto;

		strncpy(block,genes + init_index, block_tam);  //fazendo o mesmo processo de antes mas a quantidade que eh pega eh menor (ou igual),relativo ao resto que der da divisao

		MPI_Send(chave_tam, 1, MPI_INT, to, tag, MPI_COMM_WORLD);

		MPI_Send(block_tam, 1, MPI_INT, to, tag, MPI_COMM_WORLD);

		MPI_Send(chave, chave_tam+1, MPI_CHAR, to, tag, MPI_COMM_WORLD);

		MPI_Send(block, block_tam+1, MPI_CHAR, to, tag, MPI_COMM_WORLD);
		//Obs, nos envios os blocos enviados foram chamados de block soh para deixar claro que

		/*
		AQUI PREPARAR O 0 PARA RECEER O REDULCE
		*/

	}
	else{	// caso voce nao seja o ultimo processo,uma vez qeu esse em ver de receber um overlap ele recebe o que sobra da divisao

		//MPI_receive

		//RECEBE TAMANHO DA CHAVE
		MPI_Recv(chave_tam, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);
		//RECEBE TAMANHO DO BLOCO A SER PROCESSADO
		MPI_Recv(block_tam, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);

		//ALCANDO MEMORIA PARA CHAVE E BLOCO
		char* chave= (char*)malloc(chave_tam*sizeof(char));

		char* block= (char*)malloc(block_tam*sizeof(char));

		//RECEBENDO A CHAVE
		MPI_Recv(chave, 100, MPI_CHAR, 0, tag, MPI_COMM_WORLD, &status);
		//RECEBENDO O BLOCO
		MPI_Recv(block, 100, MPI_CHAR, 0, tag, MPI_COMM_WORLD, &status);

		int key_total;

		/*
		Observe que se nossa chave_tam=4 e o block_tam = 9, testaremos pela chave, (block_tam - chav_tam)+1. No exemplo eh 6
		Oque equivale ateh para o ultimo processo (ele apenas teria um blocktam menor). Veja o exemplo:
		ABCDEFGHI:
		Teste 1 = ABCD
		Teste 2 = BCDE
		Teste 3 = CDEF
		Teste 4 = DEFG
		Teste 5 = EFGH
		Teste 6 = FGHI
		*/

		int repetitions = (block_tam - chave_tam) +1;
		for (int i = 0; i < repetitions; ++i){
			if(checkForKey()){ 				//criar metodo checkForKey
				key_total++;
			}
		}

		//Aqui tem que fazer o REDULCE, pro 0 receber tudo
	}


return 0;
}



