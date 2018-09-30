#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXQTT 100
#define MAXSIZ 1000

int main(int argc,char** argv){
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////// L E I T U R A   D E   A R Q U I V O //////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	FILE *fp;
	char buff[MAXSIZ], endc = 0;
	int i = 0, n = -1, tam = 0;
	fp = fopen("dna.in", "r");
	char **genes_names, **genes_list, **chaves_list;
	int qtt_genes, qtt_chaves;
	
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
		printf("Nome : %s\nGene : ", genes_names[n]);
		
		for(i; i < MAXSIZ;i++){
			
			genes_list[n][i] = fgetc(fp);
			printf("%c", genes_list[n][i] );
			if(genes_list[n][i]=='\n' || genes_list[n][i] == EOF){
				genes_list[n][i] = '\0'; //Caracter terminal da string do gene
				break;
			}
		}
		i = 0;
		endc = fgetc(fp);
	}
	qtt_genes = n+1;
	fp = fopen("query.in", "r");
	
	endc = fgetc(fp);
	n = -1;
	
	while (endc != EOF){
		n++;
		chaves_list[n] = (char*) malloc(MAXSIZ);
		
		fgets(chaves_list[n], MAXSIZ, (FILE*)fp);
		printf("Nome : %sGene : ", chaves_list[n]);
		
		for(i; i < MAXSIZ;i++){
			
			chaves_list[n][i] = fgetc(fp);
			printf("%c", chaves_list[n][i] );
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
	
	int clr = 0;
	
	for(clr; clr < qtt_genes; clr++){
		free(genes_names[clr]);
		free(genes_list[clr]);
	}
	free(genes_names);
	free(genes_list);
	free(chaves_list);

	return 0;
}