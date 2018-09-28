#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mpi.h"


int checkForKey(char block[],char chave[],int block_tam,int chave_tam){
	int total=0;
	int check_ini; 		//"Ponteiro" dizendo onde ta comecando a parte que estamos analizando no bloco
	int j;				//ponteiro que controla UMA interacao de checagem da chave
	int end;			//controle de onde termina a checagem
	int k_index;		//index da chave para ser comparado com o J (de forma relativa,a segunda interacao de J compara com k_index =2)
	
	for (check_ini = 0; check_ini < block_tam; ++check_ini)
	{	
		end = check_ini+chave_tam;
		k_index=-1; //comecamos com -1 soh para que seja a PRIMEIRA coisa que seja feita no for,para caso de break;,nao perdermos a info

		for (j = check_ini; j < end; ++j)
		{	
			k_index++;
			if(j==(end-1)){ 
				if(block[j]==chave[k_index])total++; //se chegou aqui entao tem a chave dentro do bloco,portanto almentamos o total
			}
			if(block[j]!=chave[k_index])break;		//caso qualquer caracter esteja errado da break do for.
		}
	}

	return total;
}

void cutstr(char block[],char genes[],int block_tam,int genes_tam,int init_index){

	int current=0;
	int i=init_index;
	int final_index = init_index + block_tam;

	for(i ; i < final_index ; i++){
		block[current]=genes[i];
		current++;
	} 

	block[current]='\0';
}


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

	/*
	Aqui embaixo (no if abaixo) prepararemos 3 arrays
	char genes_list[][] 	Contendo todos os genes a nalizados
	char genes_names[][]	Os nomes qeu acompanham as sequencias de genes (ja que temos que escreve-las no arquivo out)
	char chave_list[][]		Contendo a lista das chaves	
	*/
	//char genes_list[][];
	//char genes_names[][];
	//char chave_list[][];

	if(meu_rank==0){

	}

	/*
	TUDO MARCADO COM <== talvez possa deixar de ser c omentario,eh soh para dar a ideia de como passariamos por todos
	os genes e todas as chaves, 
	*/

	//char chave[];																								<==   
	//char genes[];																								<==

	//int n_genes;	//tem o numero de genes que veio no arquivo													<==
	//int n_keys;		//tem o numero de keys que vieram no arquivo											<==

	//int genes_ctrl;	//dizer qual gene esta sendo analizado no momento,um Indice								<==
	//int chave_ctrl;	//dizer qual chave esta sendo utilizada no momento, um Indice							<==

	//for (genes_ctrl = 0; genes_ctrl < n_genes; ++genes_ctrl){													<==

	//	if(meu_rank==0)genes=genes_list[genes_ctrl]; //soh o processo 0 tem acesso a essas informacoes;			<==

	//	for (key_ctrl = 0; key_ctrl < n_keys; ++key_ctrl){														<==

			if(meu_rank==0){

				//chave=chave_list[chave_ctrl];			<==

				char chave[] = "ABC";
				char genes[] = "FODASSEEEEEEABCABCAHEHEHUAHUAABCABC";

				genes_tam = sizeof(genes)/sizeof(char)-1;		//CUidado com esse -1 ele eh para passarmos sem contar o \0
				chave_tam = sizeof(chave)/sizeof(char)-1;

				orig_tam = genes_tam / nt;						 //dividindo a sequencia genetica entre os processos,sem o overlay
				resto = genes_tam % nt; 						//de fato soh pegando o quanto vai restar da divisao.
				block_tam = orig_tam + (chave_tam-1);			//total que serah recebido por casa processo

				char block[1000];		//criando espaco para enviar os genes necessarios

				int init_index=0;
				for (to = 1; to < nt; to++){//agora enviaremos para cada processo a chave,enviarah a parte necessaria da string de genes, o resto eh deduzivel  (menos mensagem)


					cutstr(block,genes,block_tam,genes_tam,init_index);


					MPI_Send(&chave_tam,
								1,
								MPI_INT,
								to,
								tag,
								MPI_COMM_WORLD);

					MPI_Send(&block_tam,
								1,  //ja equivale a strlen(block)+1,fica menos processamento
								MPI_INT,
								to,
								tag,
								MPI_COMM_WORLD);	

					MPI_Send(chave,
								chave_tam+1,  //ja equivale a strlen(block)+1,fica menos processamento
								MPI_CHAR,
								to,
								tag,
								MPI_COMM_WORLD);

					MPI_Send(block,
								block_tam+1,  //ja equivale a strlen(block)+1,fica menos processamento
								MPI_CHAR,
								to,
								tag,
								MPI_COMM_WORLD);


					init_index += orig_tam;		//update de onde comeca os "genes" que serao enviados
				} 

				//aqui eh o envio apenas para o ultimo processo
				//obs perceba que o ultimo eh nt, pois se tivessemos 6 processos,np=6 ,nt=5, e os processos sao 0,1,2,3,4,5. OU seja nt eh o ultimo

				to = nt;

				block_tam=orig_tam+resto;

				cutstr(block,genes,block_tam,genes_tam,init_index);

				//strncpy(block,genes + init_index, block_tam);  //fazendo o mesmo processo de antes mas a quantidade que eh pega eh menor (ou igual),relativo ao resto que der da divisao


				MPI_Send(&chave_tam,
					 		1,
						 	MPI_INT,
						 	to,
						 	tag,
						 	MPI_COMM_WORLD);

				MPI_Send(&block_tam,
							1,
							MPI_INT,
							to,
							tag,
							MPI_COMM_WORLD);	

				MPI_Send(chave,
							chave_tam+1,  //ja equivale a strlen(chave)+1,fica menos processamento
							MPI_CHAR,
							to,
							tag,
							MPI_COMM_WORLD);

				MPI_Send(block,
							block_tam+1,  //ja equivale a strlen(block)+1,fica menos processamento
							MPI_CHAR,
							to,
							tag,
							MPI_COMM_WORLD);






				/*
				AQUI PREPARAR O 0 PARA RECEER O REDULCE
				*/


			}else{
				
				//MPI_receive
				

				//RECEBE TAMANHO DA CHAVE
				MPI_Recv(&chave_tam,
							1,
							MPI_INT,
							0,
							tag,
							MPI_COMM_WORLD,
							&status);
				
				//RECEBE TAMANHO DO BLOCO A SER PROCESSADO


				MPI_Recv(&block_tam,
							1,
							MPI_INT,
							0,
							tag,
							MPI_COMM_WORLD,
							&status);

				char chave[chave_tam+1];

				char block[block_tam+1];

				//RECEBENDO A CHAVE
				MPI_Recv(chave,
							chave_tam+1,
							MPI_CHAR,
							0,
							tag,
							MPI_COMM_WORLD,
							&status);

				printf("Meu Rank eh : %d , A chave eh : %s\n", meu_rank,chave);
				
					
				//RECEBENDO O BLOCO
				MPI_Recv(block,
							block_tam+1,
							MPI_CHAR,
							0,
							tag,
							MPI_COMM_WORLD,
							&status);

				printf("Meu Rank eh : %d , Meu bloco eh : %s\n", meu_rank,block);

				

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

				int key_total=checkForKey(block,chave,block_tam,chave_tam);
				
				printf("Meu Rank eh : %d, o total de chaves q achei foi : %d\n",meu_rank,key_total);

				//Aqui tem que fazer o REDULCE, pro 0 receber tudo
			}
//		}					<==
//	}						<==
		
	return 0;
}