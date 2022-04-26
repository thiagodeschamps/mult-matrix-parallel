#include <stdio.h>
#include <math.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stddef.h>
#include "mpi.h"

#define N 4
#define MAX_VALUE 10


void print_results(char *prompt, int a[N][N]);
void randomize(int **, int);


int main(int argc, char *argv[])
{

    // // Allocating memory to matrix A
	// int **matrix_a = malloc(D * sizeof(int*));
	// for(int i = 0; i < D; i++)
	// 	matrix_a[i] = malloc(D * sizeof(int));

    int rank, size, sum = 0;
    
    int **a = malloc(N * sizeof(int*));
	for(int i = 0; i < N; i++)
        a[i] = malloc(N * sizeof(int));
   	randomize(a, N);

    int **b = malloc(N * sizeof(int*));
	for(int i = 0; i < N; i++)
        b[i] = malloc(N * sizeof(int));
   	randomize(b, N);

    int **c = malloc(N * sizeof(int*));
	for(int i = 0; i < N; i++)
        c[i] = malloc(N * sizeof(int));
   	randomize(c, N);

    int *aa = malloc(N * sizeof(int*));
    int *cc = malloc(N * sizeof(int*));

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    //scatter rows of first matrix to different processes     
    MPI_Scatter(a, N*N/size, MPI_INT, aa, N*N/size, MPI_INT,0,MPI_COMM_WORLD);

    //broadcast second matrix to all processes
    MPI_Bcast(b, N*N, MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);

          //perform vector multiplication by all processes
          for (int i = 0; i < N; i++)
            {
                    for (int j = 0; j < N; j++)
                    {
                            sum = sum + aa[j] * b[j][i];  //MISTAKE_WAS_HERE               
                    }
                    cc[i] = sum;
                    sum = 0;
            }

    MPI_Gather(cc, N*N/size, MPI_INT, c, N*N/size, MPI_INT, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);        
    MPI_Finalize();

    if (rank == 0){                         //I_ADDED_THIS
        printf("deu bom \n");
    }

	for(int i = 0; i < N; i++)
		free(a[i]);
	free(a);
	for(int i = 0; i < N; i++)
		free(b[i]);
	free(b);
	for(int i = 0; i < N; i++)
		free(c[i]);
	free(c);

    free(aa);
    free(cc);
}

void print_results(char *prompt, int a[N][N])
{
    int i, j;

    printf ("\n\n%s\n", prompt);
    for (i = 0; i < N; i++) {
            for (j = 0; j < N; j++) {
                    printf(" %d", a[i][j]);
            }
            printf ("\n");
    }
    printf ("\n\n");
}

void randomize(int **matrix, int l){
	
	int n;
	
	for(int i = 0; i < l; i++){
		for(int j = 0; j < l; j++){
			n = rand() % MAX_VALUE;
			matrix[i][j] = n;
		}	
	}
}