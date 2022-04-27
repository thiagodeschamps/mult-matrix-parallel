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
#define N 3000
#define MAX_VALUE 10

void randomize(double[N][N], int);
void print_matrix(double[N][N], int);

MPI_Status status;

// Matrix holders are created
double matrix_a[N][N], matrix_b[N][N], matrix_c[N][N];

int main(int argc, char **argv)
{
  int processCount, rank, slaveTaskCount, source, dest, rows, offset;

  // Inicia o ambiente MPI
  MPI_Init(&argc, &argv);
  // Cada processo tem seu ID unico (rank)
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  // Numero de processos vai ser assimilado ao processcount
  MPI_Comm_size(MPI_COMM_WORLD, &processCount);

  // Atribuindo a quantidade de slaves a variavel slaveTaskCount
  slaveTaskCount = processCount - 1;

  // Master Process
  if (rank == 0)
  {

    // Preenchendo a matriz A e B com numeros aleatorios
    srand(time(NULL));
    randomize(matrix_a, N);
    randomize(matrix_b, N);

    printf("\n\t\tMatrix - Matrix Multiplication using MPI\n");
    double start = MPI_Wtime();

    // Print Matrix A and B
    // print_matrix(matrix_a, N);
    // print_matrix(matrix_b, N);

    // Determine number of rows of the Matrix A, that is sent to each slave process
    rows = N / slaveTaskCount;
    // Offset determina o ponto de comeco que sera enviado para os processos escravos
    offset = 0;

    // Operacoes de calculos serao atribuidas para os processos escravos (rank > 0);
    // Cada tag da mensagem eh 1
    for (dest = 1; dest <= slaveTaskCount; dest++)
    {
      // Reconhecendo o offset da matriz A
      MPI_Send(&offset, 1, MPI_INT, dest, 1, MPI_COMM_WORLD);
      // Reconhecendo a qntd de linhas
      MPI_Send(&rows, 1, MPI_INT, dest, 1, MPI_COMM_WORLD);
      // Enviando linhas da matriz A que sera assimilado a um escravo para computar
      MPI_Send(&matrix_a[offset][0], rows * N, MPI_DOUBLE, dest, 1, MPI_COMM_WORLD);
      // Matriz B eh enviada
      MPI_Send(&matrix_b, N * N, MPI_DOUBLE, dest, 1, MPI_COMM_WORLD);

      // Offset eh incrementado de acordo com a quantidade de linhas enviadas no processo
      offset = offset + rows;
    }

    // Processo mestre espera ate que cada processo escravo envie o resultado do calculo com a tag da mensagem 2
    for (int i = 1; i <= slaveTaskCount; i++)
    {
      source = i;
      // Recebe o offset de cada processo escravo
      MPI_Recv(&offset, 1, MPI_INT, source, 2, MPI_COMM_WORLD, &status);
      // Recebe a quantidade de linhas processadas por cada processo escravo
      MPI_Recv(&rows, 1, MPI_INT, source, 2, MPI_COMM_WORLD, &status);
      // Linhas calculadas por cada processo sera armazenadas na matriz de acordo com cada offset e o numeor de linhas processadas
      MPI_Recv(&matrix_c[offset][0], rows * N, MPI_DOUBLE, source, 2, MPI_COMM_WORLD, &status);
    }

      double finish = MPI_Wtime();
      printf("Done in %f seconds.\n", finish - start);

    // Print the result matrix
    // print_matrix(matrix_c, N);
  }

  // Processos escravos
  if (rank > 0)
  {

    // ID do processo mestre eh definido
    source = 0;

    // Processo escravo espera que o buffer da mensagem com tag = 1, enviada pelo processo mestre
    // Cada processo recebera e executara paralelamente o processo

    // O processo escravo recebe o offset enviado pelo processo mestre
    MPI_Recv(&offset, 1, MPI_INT, source, 1, MPI_COMM_WORLD, &status);
    // O processo escravo recebe o numero de linhas enviado pelo processo mestre
    MPI_Recv(&rows, 1, MPI_INT, source, 1, MPI_COMM_WORLD, &status);
    // The slave process receives the sub portion of the Matrix A which assigned by Root
    // O processo escravo recebera uma linha da matriz A, que sera atirbuida pelo Mestre
    MPI_Recv(&matrix_a, rows * N, MPI_DOUBLE, source, 1, MPI_COMM_WORLD, &status);
    // O processo escravo recebe a Matriz B
    MPI_Recv(&matrix_b, N * N, MPI_DOUBLE, source, 1, MPI_COMM_WORLD, &status);

    // Matrix multiplication

    for (int k = 0; k < N; k++)
    {
      for (int i = 0; i < rows; i++)
      {
        matrix_c[i][k] = 0.0;
        for (int j = 0; j < N; j++)
          matrix_c[i][k] = matrix_c[i][k] + matrix_a[i][j] * matrix_b[k][j];
      }
    }

    // O resultado do calculo sera enviado de volta ao processo Mestre com tag = 2

    // Offset sera enviado para o Mesre, o qual determina o ponto de partida do valor calculado na matriz C
    MPI_Send(&offset, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
    // Numero de linhas calculadas sera enviado para o processo Mestre
    MPI_Send(&rows, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
    // Matriz resultante com as linhas calculadas serao enviadas para o processo mestre
    MPI_Send(&matrix_c, rows * N, MPI_DOUBLE, 0, 2, MPI_COMM_WORLD);
  }
  MPI_Finalize();
}

void randomize(double matrix[N][N], int l)
{

  double n;

  for (int i = 0; i < l; i++)
  {
    for (int j = 0; j < l; j++)
    {
      n = rand() % MAX_VALUE + 1.0;
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