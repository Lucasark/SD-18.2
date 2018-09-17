#include <stdio.h>
#include <string.h>
#include "mpi.h"

float f(float x){

	float resp= x*x;

	return resp;

}


float calculaIntegral(float a,float b,int n,float h){

	float integral;

	integral = (f(a) + f(b))/2;

	float temp = a;
	for (int i = 1; i < n; ++i)
	{
		 temp +=h;
		 integral += f(temp);
	}

	integral *= h;

	return integral;

}


int main(int argc,char** argv){

	int meu_rank,np,tag=0;
	int from,to=0;
	MPI_Status status;
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&meu_rank);
	MPI_Comm_size(MPI_COMM_WORLD,&np);

	int nTraps = 2048;
	float h;
	float a=0.0,b=5.0;
	float thisA,thisB;
	float integral;
	float total=0;
	float message=0.0;
	int thisN;


	
	h = (b-a)/nTraps;	
	thisN = nTraps/np;	
	thisA = a + meu_rank*thisN*h;
	thisB = thisA + thisN*h;

	integral = calculaIntegral(thisA,thisB,thisN,h);

	if (meu_rank==0){
		total = integral;
		for (from = 1; from < np; ++from)
		{
			MPI_Recv(&message,
					1,
					MPI_FLOAT,
					from,
					tag,
					MPI_COMM_WORLD,
					&status);
			total+=message;
		}
	}else{
		MPI_Send(&integral,
					1,
					MPI_FLOAT,
					to,
					tag,
					MPI_COMM_WORLD);
	}

	if(meu_rank==0) printf("A integral eh: %f\n",total);

	MPI_Finalize();

	return 0;
}