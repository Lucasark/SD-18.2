#include <stdio.h>
#include <string.h>
#include "mpi.h"

int main(int argc,char** argv){
	
	int meu_rank,np,tag=0;
	int from,to=0;
	MPI_Status status;
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&meu_rank);
	MPI_Comm_size(MPI_COMM_WORLD,&np);

	int orig_tam; 			// Eh o quanto da string cada processo pegarah sem dar overlap. O tamanho do Overlap chave_tam-1
	int resto; 				// O que sobrar da divisao  da String quando for separada para os processos.
	int block_tam; 			// tamanho (orig_tam + chave_tam-1), chamaremos de bloco o que cada processo receberah pra processar
	int nt = np-1; 			// numero de trabalhadores uma vez que o processo 0 serah mestre.

	if(meu_rank==0){
		/*
		IO
		Aqui vai o programa que recupera a string da chave e do genes (sequencia genetica), e deve guardar essas informacoes
		nas variaveis logo abaixo (que serao char*).
		*/
		char* chave;char* genes;

		int genes_tam = sizeof(genes)/sizeof(char*);
		int chave_tam = sizeof(chave)/sizeof(char*);

		orig_tam = genes_tam / nt;						 //dividindo a sequencia genetica entre os processos,sem o overlay
		resto = genes_tam % nt; 						//de fato soh pegando o quanto vai restar da divisao.
		block_tam = orig_tam + (chave_tam-1);			//total que serah recebido por casa processo

		char *message=(char *)malloc(block_tam*sizeof(char));		//criando espaco para enviar os genes necessarios
		int to;
		int init_index=0;
		for (to = 1; to < nt; to++){//agora enviaremos para cada processo a chave,enviarah a parte necessaria da string de genes, o resto eh deduzivel  (menos mensagem)
			
			//strncpy(dest, src + init_index, block_tam);Destino,fala qual array comeca

			strncpy(message,genes + init_index, block_tam);  //pega o que eh relevante pro processo da vez e coloca em message

			MPI_Send(message,
					block_tam+1,  //ja equivale a strlen(message)+1,fica menos processamento
					MPI_CHAR,
					to,
					tag,
					MPI_COMM_WORLD);


			MPI_Send(chave,
					chave_tam+1,  //ja equivale a strlen(message)+1,fica menos processamento
					MPI_CHAR,
					to,
					tag,
					MPI_COMM_WORLD);

			init_index += chave_tam;		//update de onde comeca os "genes" que serao enviados
		 } 



	}else if (meu_rank!=nt-1){	// caso voce nao seja o ultimo processo,uma vez qeu esse em ver de receber um overlap ele recebe o que sobra da divisao
		//MPI_receive
		MPI_Recv(msg,
					100,
					MPI_CHAR,
					0,
					tag,
					MPI_COMM_WORLD,
					&status);



	}else{//no caso do ultimo processo



	}



return 0;
}