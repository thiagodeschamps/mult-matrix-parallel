#include <stdlib.h>
#include <stdio.h>
#include "mpi.h"
#include <time.h>
#include <sys/time.h>

/*
  In here 'process o' which distribute the workload to other processes is considered 
  as Root (Master) process and other processes which do the computation is considered 
  as Slave task processes. 
*/

// Number of rows and columnns in a matrix
#define N 5
#define MAX_VALUE 10

void randomize(double [N][N], int);
void print_matrix(double [N][N], int);


MPI_Status status;

// Matrix holders are created
double matrix_a[N][N],matrix_b[N][N],matrix_c[N][N];

int main(int argc, char **argv)
{
  int processCount, rank, slaveTaskCount, source, dest, rows, offset;

  struct timeval start, stop;

// MPI environment is initialized
  MPI_Init(&argc, &argv);
// Each process gets unique ID (rank)
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
// Number of processes in communicator will be assigned to variable -> processCount
  MPI_Comm_size(MPI_COMM_WORLD, &processCount);

// Number of slave tasks will be assigned to variable -> slaveTaskCount
  slaveTaskCount = processCount - 1;

// Root (Master) process
 if (rank == 0) {
	
// Matrix A and Matrix B both will be filled with random numbers
    srand(time(NULL));
    randomize(matrix_a, N);
    randomize(matrix_b, N);

  printf("\n\t\tMatrix - Matrix Multiplication using MPI\n");

// Print Matrix A and B
    print_matrix(matrix_a, N);
    print_matrix(matrix_b, N);

// Determine number of rows of the Matrix A, that is sent to each slave process
    rows = N/slaveTaskCount;
// Offset variable determines the starting point of the row which sent to slave process
    offset = 0;

// Calculation details are assigned to slave tasks. Process 1 onwards;
// Each message's tag is 1
    for (dest=1; dest <= slaveTaskCount; dest++)
    {
      // Acknowledging the offset of the Matrix A
      MPI_Send(&offset, 1, MPI_INT, dest, 1, MPI_COMM_WORLD);
      // Acknowledging the number of rows
      MPI_Send(&rows, 1, MPI_INT, dest, 1, MPI_COMM_WORLD);
      // Send rows of the Matrix A which will be assigned to slave process to compute
      MPI_Send(&matrix_a[offset][0], rows*N, MPI_DOUBLE,dest,1, MPI_COMM_WORLD);
      // Matrix B is sent
      MPI_Send(&matrix_b, N*N, MPI_DOUBLE, dest, 1, MPI_COMM_WORLD);
      
      // Offset is modified according to number of rows sent to each process
      offset = offset + rows;
    }

// Root process waits untill the each slave proces sent their calculated result with message tag 2
    for (int i = 1; i <= slaveTaskCount; i++)
    {
      source = i;
      // Receive the offset of particular slave process
      MPI_Recv(&offset, 1, MPI_INT, source, 2, MPI_COMM_WORLD, &status);
      // Receive the number of rows that each slave process processed
      MPI_Recv(&rows, 1, MPI_INT, source, 2, MPI_COMM_WORLD, &status);
      // Calculated rows of the each process will be stored int Matrix C according to their offset and
      // the processed number of rows
      MPI_Recv(&matrix_c[offset][0], rows*N, MPI_DOUBLE, source, 2, MPI_COMM_WORLD, &status);
    }

// Print the result matrix
    print_matrix(matrix_c, N);

  }

// Slave Processes 
  if (rank > 0) {

    // Source process ID is defined
    source = 0;

    // Slave process waits for the message buffers with tag 1, that Root process sent
    // Each process will receive and execute this separately on their processes

    // The slave process receives the offset value sent by root process
    MPI_Recv(&offset, 1, MPI_INT, source, 1, MPI_COMM_WORLD, &status);
    // The slave process receives number of rows sent by root process  
    MPI_Recv(&rows, 1, MPI_INT, source, 1, MPI_COMM_WORLD, &status);
    // The slave process receives the sub portion of the Matrix A which assigned by Root 
    MPI_Recv(&matrix_a, rows*N, MPI_DOUBLE, source, 1, MPI_COMM_WORLD, &status);
    // The slave process receives the Matrix B
    MPI_Recv(&matrix_b, N*N, MPI_DOUBLE, source, 1, MPI_COMM_WORLD, &status);

    // Matrix multiplication

    for (int k = 0; k<N; k++) {
      for (int i = 0; i<rows; i++) {
        // Set initial value of the row summataion
        matrix_c[i][k] = 0.0;
        // Matrix A's element(i, j) will be multiplied with Matrix B's element(j, k)
        for (int j = 0; j<N; j++)
          matrix_c[i][k] = matrix_c[i][k] + matrix_a[i][j] * matrix_b[j][k];
      }
    }

    // Calculated result will be sent back to Root process (process 0) with message tag 2
    
    // Offset will be sent to Root, which determines the starting point of the calculated
    // value in matrix C 
    MPI_Send(&offset, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
    // Number of rows the process calculated will be sent to root process
    MPI_Send(&rows, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
    // Resulting matrix with calculated rows will be sent to root process
    MPI_Send(&matrix_c, rows*N, MPI_DOUBLE, 0, 2, MPI_COMM_WORLD);
  }

  MPI_Finalize();
}

void randomize(double matrix[N][N], int l){
	
	int n;
	
	for(int i = 0; i < l; i++){
		for(int j = 0; j < l; j++){
			n = rand() % MAX_VALUE;
			matrix[i][j] = n;
		}	
	}
}

void print_matrix(double matrix[N][N], int l)
{
    for (int i = 0; i < l; i++)
    {
        for (int j = 0; j < l; j++)
        {
            printf("%.0f ", matrix[i][j]);
        }
        printf("\n");
    }
    printf("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
}