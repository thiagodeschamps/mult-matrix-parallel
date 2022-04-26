// Code for multiple 2 matrix using dynamic memory allocation

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>


#define D 3000
#define MAX_VALUE 100

void randomize(int **, int);
void print_matrix(int **, int);
void transpose(int **, int **);
void mul_matrix_t(int **, int **, int **, int);
void mul_matrix_t_par(int **, int **, int **, int);

int main(int argc, char *argv[])
{

    // Declaring variables to measure the time taken by the function
    double dtime;

    // Setting up seed to random function
    srand(time(NULL));

    // Allocating memory to matrix A
    int **matrix_a = malloc(D * sizeof(int *));
    for (int i = 0; i < D; i++)
        matrix_a[i] = malloc(D * sizeof(int));

    // Allocating memory to matrix B
    int **matrix_b = malloc(D * sizeof(int *));
    for (int i = 0; i < D; i++)
        matrix_b[i] = malloc(D * sizeof(int));

    // Allocating memory to matrix T
    int **matrix_t = malloc(D * sizeof(int *));
    for (int i = 0; i < D; i++)
        matrix_t[i] = malloc(D * sizeof(int));

    // Alocating memory for the resulting matrix
    int **matrix_ab = malloc(D * sizeof(int *));
    for (int i = 0; i < D; i++)
        matrix_ab[i] = malloc(D * sizeof(int));

    // Populating matrix with random numbers
    randomize(matrix_a, D);
    randomize(matrix_b, D);

    // Transpose matrix B and storing in T
    transpose(matrix_b, matrix_t);

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    printf("\nSERIAL\n");

	dtime = omp_get_wtime();
    mul_matrix_t(matrix_a, matrix_t, matrix_ab, D);
	dtime = omp_get_wtime() - dtime;

	printf("%f\n", dtime);
  // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    printf("\nPARALLEL\n");

	dtime = omp_get_wtime();
    mul_matrix_t_par(matrix_a, matrix_t, matrix_ab, D);
	dtime = omp_get_wtime() - dtime;

	printf("%f\n", dtime);
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

    // print_matrix(matrix_a, D);
    // print_matrix(matrix_b, D);
    // print_matrix(matrix_t, D);
    // print_matrix(matrix_ab, D);

    for (int i = 0; i < D; i++)
        free(matrix_a[i]);
    free(matrix_a);

    for (int i = 0; i < D; i++)
        free(matrix_b[i]);
    free(matrix_b);

    for (int i = 0; i < D; i++)
        free(matrix_t[i]);
    free(matrix_t);

    for (int i = 0; i < D; i++)
        free(matrix_ab[i]);
    free(matrix_ab);

    return 0;
}

void randomize(int **matrix, int l)
{

    int n;

    for (int i = 0; i < l; i++)
    {
        for (int j = 0; j < l; j++)
        {
            n = rand() % MAX_VALUE;
            matrix[i][j] = n;
        }
    }
}

void print_matrix(int **matrix, int l)
{
    for (int i = 0; i < l; i++)
    {
        for (int j = 0; j < l; j++)
        {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
    printf("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
}

// This function stores transpose of A[][] in T[][]
void transpose(int **A, int **T)
{
    int i, j;
    for (i = 0; i < D; i++)
        for (j = 0; j < D; j++)
            T[i][j] = A[j][i];
}

void mul_matrix_t(int **matrix_a, int **matrix_b, int **matrix_ab, int l)
{
    for (int i = 0; i < l; i++)
    {
        for (int j = 0; j < l; j++)
        {
            matrix_ab[i][j] = 0;
            for (int k = 0; k < l; k++)
            {
                matrix_ab[i][j] += matrix_a[i][k] * matrix_b[j][k];
            }
        }
    }
}

void mul_matrix_t_par(int **matrix_a, int **matrix_b, int **matrix_ab, int l)
{
#pragma omp parallel
    {
        int i, j, k;
#pragma omp for
        for (i = 0; i < l; i++)
        {
            for (j = 0; j < l; j++)
            {
                matrix_ab[i][j] = 0;
                for (k = 0; k < l; k++)
                {
                    matrix_ab[i][j] += matrix_a[i][k] * matrix_b[j][k];
                }
            }
        }
    }
}