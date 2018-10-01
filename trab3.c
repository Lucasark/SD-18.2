#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mpi.h"

#define MAXQTT 100
#define MAXSIZ 1000
#define BLKSIZ 600

int checkForKey(char block[],char chave[],int block_tam,int chave_tam,int* lugar){
	int achado = 0;
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
				if(block[j]==chave[k_index]){
					if(!achado){
						*lugar = check_ini+j;   //se nunca tiver achado salva o lugar (pq serah a primeira)
												//check_ini+j pois eh quanto andamos com o inicio e quanto andamos dai em diante
					}
					achado=1;
					total++; //se chegou aqui entao tem a chave dentro do bloco,portanto almentamos o total
				}
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

	int key_total_local=0;
	int key_total=69;


	int qtt_genes, qtt_chaves;
	char **genes_names, **genes_list, **chaves_list;
	FILE *fout;

	int **lugares=(int**)malloc(MAXQTT*sizeof(int*));

	int **totais= (int**)malloc(MAXQTT*sizeof(int*));
	int x,y;
	for (x = 0; x < MAXQTT; ++x){
		totais[x]= (int*)malloc(MAXQTT*sizeof(int));
		lugares[x]= (int*)malloc(MAXQTT*sizeof(int));
		for (y = 0; y < MAXQTT; ++y){
			totais[x][y]=0;
			lugares[x][y]=0;
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////ROTINA DE PREPARO////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	if(meu_rank==0){	
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////// L E I T U R A   D E   A R Q U I V O //////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		FILE *fp;
		char buff[MAXSIZ], endc = 0;
		int i = 0, n = -1, tam = 0;
		fp = fopen("dna.in", "r");
		fout = fopen("dna.out", "w");

		genes_names = (char**) malloc(MAXQTT * sizeof(char *));
		genes_list = (char**) malloc(MAXQTT * sizeof(char *));
		chaves_list = (char**) malloc(MAXQTT * sizeof(char *));



		//capta o primeiro caracter para manter os nomes uniformes (sem o > antes)
		endc = fgetc(fp);
		while (endc != EOF){
			n++;
			genes_names[n] = (char*) malloc(MAXSIZ);
			genes_list[n] = (char*) malloc(MAXSIZ);
			
			fgets(genes_names[n], MAXSIZ, (FILE*)fp);
			//printf("Nome : %s\nGene : ", genes_names[n]);
			
			for(i; i < MAXSIZ;i++){
				
				genes_list[n][i] = fgetc(fp);
				//printf("%c", genes_list[n][i] );
				if(genes_list[n][i]=='\n' || genes_list[n][i] == EOF){
					genes_list[n][i] = '\0'; //Caracter terminal da string do gene
					break;
				}
			}
			i = 0;
			endc = fgetc(fp);
		}
		qtt_genes = n;							//SE ROLAR PROBLEMA SOMAR 1
		fp = fopen("query.in", "r");

		endc = fgetc(fp);
		n = -1;

		while (endc != EOF){
			n++;
			chaves_list[n] = (char*) malloc(MAXSIZ);
			
			fgets(chaves_list[n], MAXSIZ, (FILE*)fp);
			//printf("Nome : %sGene : ", chaves_list[n]);
			
			for(i; i < MAXSIZ;i++){
				
				chaves_list[n][i] = fgetc(fp);
				//printf("%c", chaves_list[n][i] );
				if(chaves_list[n][i]=='\n' || chaves_list[n][i] == EOF){
					chaves_list[n][i] = '\0'; //Caracter terminal da string do gene
					break;
				}
			}
			i = 0;
			endc = fgetc(fp);
		}
		qtt_chaves = n+1;

		fclose(fp);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		for (to = 1; to <= nt; ++to)
		{
			MPI_Send(&qtt_genes,
					1,
					MPI_INT,
					to,
					9,
					MPI_COMM_WORLD);

			MPI_Send(&qtt_chaves,
						1,
						MPI_INT,
						to,
						8,
						MPI_COMM_WORLD);
		}
	}else{

		MPI_Recv(&qtt_genes,
							1,
							MPI_INT,
							0,
							9,
							MPI_COMM_WORLD,
							&status);

		//printf("\nMeu Rank eh : %d e eu recebi: %d genes\n",meu_rank,qtt_genes);

		MPI_Recv(&qtt_chaves,
							1,
							MPI_INT,
							0,
							8,
							MPI_COMM_WORLD,
							&status);
		//printf("\n\nMeu Rank eh : %d e eu recebi: %d chaves\n",meu_rank,qtt_chaves);
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////ROTINA PRINCIPAL////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	int genes_ctrl,chave_ctrl;

	char *genes = (char*) malloc(MAXSIZ); //Todos os ranks vao alocar isso,entao tem que desalocar
	char *chave = (char*) malloc(MAXSIZ);

	for (genes_ctrl = 0; genes_ctrl < qtt_genes; ++genes_ctrl){		// aqui sao 2 for`s para percorrer todas as combinacoes de genes
																	// com suas respectivas chaves	

		if(meu_rank==0){//soh o processo 0 tem acesso a essas informacoes;		
			genes=genes_list[genes_ctrl]; 
		}	

		for (chave_ctrl = 0; chave_ctrl < qtt_chaves; ++chave_ctrl){														

			if(meu_rank==0){

				chave=chaves_list[chave_ctrl];			

				genes_tam = strlen(genes);		//CUidado com esse -1 ele eh para passarmos sem contar o \0
				chave_tam = strlen(chave);

				orig_tam = genes_tam / nt;						 //dividindo a sequencia genetica entre os processos,sem o overlay
				resto = genes_tam % nt; 						//de fato soh pegando o quanto vai restar da divisao.
				block_tam = orig_tam + (chave_tam-1);			//total que serah recebido por casa processo

				char * block = (char*) malloc(BLKSIZ);	//criando espaco para enviar os genes necessarios (Unico do rank 0)
				
				int init_index=0;
				for (to = 1; to < nt; to++){//agora enviaremos para cada processo a chave,enviarah a parte necessaria da string de genes, o resto eh deduzivel  (menos mensagem)


					cutstr(block,genes,block_tam,genes_tam,init_index);


					MPI_Send(&chave_tam,
								1,
								MPI_INT,
								to,
								1,
								MPI_COMM_WORLD);

					MPI_Send(&block_tam,
								1,  //ja equivale a strlen(block)+1,fica menos processamento
								MPI_INT,
								to,
								2,
								MPI_COMM_WORLD);	

					//printf("\nO tamanho da chave eh %d bits | O tamanho do bloco eh %d bits\n", (int) sizeof(chave_tam), (int) sizeof(block_tam));

					//printf("Chave_tam : %d | Bloco_tam : %d | Chave : %s | Bloco %s \n",chave_tam,block_tam,chave,block);

					MPI_Send(chave,
								chave_tam+1,  //ja equivale a strlen(block)+1,fica menos processamento
								MPI_CHAR,
								to,
								3,
								MPI_COMM_WORLD);

					MPI_Send(block,
								block_tam+1,  //ja equivale a strlen(block)+1,fica menos processamento
								MPI_CHAR,
								to,
								4,
								MPI_COMM_WORLD);

					init_index += orig_tam;		//update de onde comeca os "genes" que serao enviados
				} 
				
				//aqui eh o envio apenas para o ultimo processo
				//obs perceba que o ultimo eh nt, pois se tivessemos 6 processos,np=6 ,nt=5, e os processos sao 0,1,2,3,4,5. OU seja nt eh o ultimo

				to = nt;

				block_tam=orig_tam+resto;

				cutstr(block,genes,block_tam,genes_tam,init_index);

				MPI_Send(&chave_tam,
					 		1,
						 	MPI_INT,
						 	to,
						 	1,
						 	MPI_COMM_WORLD);

				MPI_Send(&block_tam,
							1,
							MPI_INT,
							to,
							2,
							MPI_COMM_WORLD);	

				//printf("Chave_tam : %d | Bloco_tam : %d | Chave : %s | Bloco %s \n",chave_tam,block_tam,chave,block);

				MPI_Send(chave,
							chave_tam+1,  //ja equivale a strlen(chave)+1,fica menos processamento
							MPI_CHAR,
							to,
							3,
							MPI_COMM_WORLD);

				MPI_Send(block,
							block_tam+1,  //ja equivale a strlen(block)+1,fica menos processamento
							MPI_CHAR,
							to,
							4,
							MPI_COMM_WORLD);


				//MPI_Reduce(&key_total_local, &key_total, 1, MPI_INT, MPI_SUM, 0,MPI_COMM_WORLD);
				
				MPI_Barrier(MPI_COMM_WORLD);


				//////////////////////////////////////////////////////////////////////////////////////
				////////////////////////////////////SOMANDO////////////////////////////////////////////
				//////////////////////////////////////////////////////////////////////////////////////
				int total_chave=0;
				int primeiro_lugar=1000000;
				int temp_lugar;
				int lugar_real;

				for (x = 1; x <= nt; ++x){

					MPI_Recv(&key_total_local,
							1,
							MPI_INT,
							x,
							10,
							MPI_COMM_WORLD,
							&status);

					MPI_Recv(&temp_lugar,
							1,
							MPI_INT,
							x,
							11,
							MPI_COMM_WORLD,
							&status);

					///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					////////////////////////////////////////////////////MUDANCA////////////////////////////////////////////////////////////////////////
					if(key_total_local){// se tiver pelo menos uma aparicao

						lugar_real=temp_lugar+((x-1)*orig_tam); //saber onde exatamente foi encontrado(POIS EH RELATIVO A ORIGIN_TAM)
						
						if(lugar_real < primeiro_lugar){

							primeiro_lugar=lugar_real;  //atualiza temporariamente

							printf("Bloco : %d | lugar_real : %d | orig_tam : %d | temp_lugar : %d \n",x,lugar_real,orig_tam,temp_lugar);
						}
					}
					///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					////////////////////////////////////////////////////MUDANCA////////////////////////////////////////////////////////////////////////
					total_chave+=key_total_local;
				}

				lugares[genes_ctrl][chave_ctrl]=primeiro_lugar;
				totais[genes_ctrl][chave_ctrl]=total_chave;

				//////////////////////////////////////////////////////////////////////////////////////
				//////////////////////////////////////////////////////////////////////////////////////
				//////////////////////////////////////////////////////////////////////////////////////


				free(block);// FAZER ISSO PRO ULTIMO tirar comentario quando redulce for implementado
			}else{
				
				
				//MPI_receive
				

				//RECEBE TAMANHO DA CHAVE
				MPI_Recv(&chave_tam,
							1,
							MPI_INT,
							0,
							1,
							MPI_COMM_WORLD,
							&status);
				
				//RECEBE TAMANHO DO BLOCO A SER PROCESSADO
				MPI_Recv(&block_tam,
							1,
							MPI_INT,
							0,
							2,
							MPI_COMM_WORLD,
							&status);

				char *chave_copy=(char*)malloc(chave_tam+2); //EM teoria eles ja tem um treco de nome chave alocado,soh que bem maior

				char *block_copy=(char*)malloc(block_tam+2);//coloquei  +2 soh pra coisas de \0

				//RECEBENDO A CHAVE
				MPI_Recv(chave_copy,
							chave_tam+2,
							MPI_CHAR,
							0,
							3,
							MPI_COMM_WORLD,
							&status);
				
				//RECEBENDO O BLOCO
				MPI_Recv(block_copy,
							block_tam+2,
							MPI_CHAR,
							0,
							4,
							MPI_COMM_WORLD,
							&status);


				//printf("\n Sou : %d | chave tamanho : %d | block tamanho : %d \n Chave : %s | Bloco : %s \n ",meu_rank,chave_tam,block_tam,chave_copy,block_copy);
				
				/*
				Observe que se nossa chave_tam=4 e o block_tam = 9, testaremos pela chave, (block_tam - chav_tam)+1. No exemplo eh 6
				Oque equivale ateh para o ultimo processo (ele apenas teria um block_tam menor). Veja o exemplo:

				ABCDEFGHI: 

				Teste 1 = ABCD
				Teste 2 = BCDE
				Teste 3 = CDEF
				Teste 4 = DEFG
				Teste 5 = EFGH
				Teste 6 = FGHI
				*/
				int lugar=0;

				key_total_local=checkForKey(block_copy,chave_copy,block_tam,chave_tam,&lugar);
				
				//printf("\n Gene : %d | Key : %d  Rank : %d | Bloco : %s, Chave : %s | O total de chaves q achei foi : %d\n",genes_ctrl,chave_ctrl,meu_rank,block_copy,chave_copy,key_total_local);
	
				MPI_Barrier(MPI_COMM_WORLD);

				free(chave_copy);
				free(block_copy);

				MPI_Send(&key_total_local,
							1, 
							MPI_INT,
							0,
							10,
							MPI_COMM_WORLD);


				MPI_Send(&lugar,
							1,  
							MPI_INT,
							0,
							11,
							MPI_COMM_WORLD);
			}
		}
	}	


	if(meu_rank==0){
		chave_ctrl=0;
		genes_ctrl=0;
		int sem_resultado=1;


		for (chave_ctrl = 0; chave_ctrl < qtt_chaves; ++chave_ctrl){
			
			sem_resultado=1;

			fprintf(fout, ">Query string #%d\n",chave_ctrl+1);

			for (genes_ctrl = 0; genes_ctrl < qtt_genes; ++genes_ctrl){

				 if(totais[genes_ctrl][chave_ctrl]){
				 		sem_resultado=0;
				 		fprintf(fout, ">%s",genes_names[genes_ctrl]);
						fprintf(fout, "Total de Ocorrencias : %d , Primeira ocorrencia : %d\n", totais[genes_ctrl][chave_ctrl], lugares[genes_ctrl][chave_ctrl]);
					}
			}

			if(sem_resultado){
				fprintf(fout, "NOT FOUND\n");
				sem_resultado=1;
			}
		}

		fclose(fout);
	}


	//Limpeza//

	if(meu_rank==0){

		int clr = 0;

		for(clr; clr < qtt_genes; clr++){
			free(genes_names[clr]);
			free(genes_list[clr]);
		}
		free(genes_names);
		free(genes_list);
		free(chaves_list);

		for (x = 0; x < MAXQTT; ++x){
			free(totais[x]);
			free(lugares[x]);
		}
		free(totais);
		free(lugares);
	}

	free(chave);
	free(genes);

	MPI_Finalize();

	return 0;
}