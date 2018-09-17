#include <stdio.h>
#include <string.h>
#include "mpi.h"

int main(int argc,char** argv){

	int meu_rank,np,tag=0;
	int from,to=0;
	char msg[100]; 
	MPI_Status status;
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&meu_rank);
	MPI_Comm_size(MPI_COMM_WORLD,&np);

	if(meu_rank==0){
		sprintf(msg,"Processo %d mandou oi para processo ",meu_rank);
		for (to = 1; to < np; ++to){
			MPI_Send(msg,
						strlen(msg)+1,
						MPI_CHAR,
						to,
						tag,
						MPI_COMM_WORLD);
		}
	}else{
		MPI_Recv(msg,
					100,
					MPI_CHAR,
					from,
					tag,
					MPI_COMM_WORLD,
					&status);

		printf("%s %d\n", msg,meu_rank);
	}


	/*
	if(meu_rank != 0) {
		sprintf(msg,"Processo %d esta vivo!",meu_rank);
		MPI_Send (msg,
					strlen(msg)+1,
					MPI_CHAR,
					0,
					tag,
					MPI_COMM_WORLD);
	}else{ //if (meu_rank == 0)
		for(from=1;from<np;from++){
			MPI_Recv(msg,
					100,
					MPI_CHAR,
					from,
					tag,
					MPI_COMM_WORLD,
					&status);

			printf("%s\n", msg);
		}
	}
	*/
	MPI_Finalize();
}