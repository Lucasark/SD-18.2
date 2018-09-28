#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

	int np = 8;
	int to=0;
	int orig_tam=0; 		// Eh o quanto da string cada processo pegarah sem dar overlap. O tamanho do Overlap chave_tam-1
	int resto=0; 			// O que sobrar da divisao  da String quando for separada para os processos.
	int block_tam=0; 		// tamanho (orig_tam + chave_tam-1), chamaremos de bloco o que cada processo receberah pra processar
	int nt = np-1; 			// numero de trabalhadores uma vez que o processo 0 serah mestre.

	int total=0;
	int result=0;

	int genes_tam=0;		//TOdos podem iniciar essas duas
	int chave_tam=0;

	char chave[] = "ABC";
	
	char genes[] = "FODAABCSSEERTABC3EFABCABCAHEABCHEHUAHUAABCABC";


	printf("%s / %s\n",chave,genes );

	genes_tam = sizeof(genes)/sizeof(char) -1;
	chave_tam = sizeof(chave)/sizeof(char) -1;

	printf("%d,%d\n",genes_tam,chave_tam );

	orig_tam = genes_tam / nt;						 //dividindo a sequencia genetica entre os processos,sem o overlay
	resto = genes_tam % nt; 						//de fato soh pegando o quanto vai restar da divisao.
	block_tam = orig_tam + (chave_tam-1);			//total que serah recebido por casa processo
	
	printf("Block :%d \nOrig_tam: %d \nResto: %d\n",block_tam,orig_tam,resto);

	char block[block_tam+1];		//criando espaco para enviar os genes necessarios
		
	int init_index=0;

	for (to = 1; to < nt; to++){


		cutstr(block,genes,block_tam,genes_tam,init_index);

		//strncpy(block,genes + init_index, block_tam);
			   
		printf("Bloco %d : %s\n",to,block);

		init_index += orig_tam;		//update de onde comeca os "genes" que serao enviados


		result=checkForKey(block,chave,block_tam,chave_tam);
		total+=result;

		 printf("Aparicoes da chave : %d \n",result);

	} 

	to = nt;

	block_tam=orig_tam+resto;

	cutstr(block,genes,block_tam,genes_tam,init_index);

	printf("Bloco %d : %s\n",to,block);

	result=checkForKey(block,chave,block_tam,chave_tam);
	total+=result;

	printf("Aparicoes da chave : %d \n",result);

	printf("TOTAL : %d\n",total);	

	return 0;
}