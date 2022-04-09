// C Program to find
// transpose of a matrix
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 700
#define MAX_VALUE 100000

void randomize(int matrix[][N], int l){
	
	for(int i = 0; i < l; i++){
		for(int j = 0; j < l; j++){
			matrix[i][j] = rand() % MAX_VALUE;
		}	
	}
}

// This function stores transpose of A[][] in B[][]
void transpose(int A[][N], int T[][N])
{
	int i, j;
	for (i = 0; i < N; i++)
		for (j = 0; j < N; j++)
			T[i][j] = A[j][i];
}

void mul_matrix(int matrix_a[][N], int matrix_b[][N], int matrix_ab[][N], int l){
	for(int i = 0; i < l; i++){
		for(int j = 0; j < l; j++){
			matrix_ab[i][j] = 0;
			for(int k = 0; k < l; k++){
				matrix_ab[i][j] += matrix_a[i][k] * matrix_b[k][j];
			}
		}
	}
	
}

void mul_matrix_t(int matrix_a[][N], int matrix_b[][N], int matrix_ab[][N], int l){
	for(int i = 0; i < l; i++){
		for(int j = 0; j < l; j++){
			matrix_ab[i][j] = 0;
			for(int k = 0; k < l; k++){
				matrix_ab[i][j] += matrix_a[i][k] * matrix_b[j][k];
			}
		}
	}
	
}

void print_matrix(int matrix[][N], int l){
	for(int i = 0; i < l; i++){
		for(int j = 0; j < l; j++){
			printf("%d ", matrix[i][j]);
		}
		printf("\n");		
	}
    printf("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
}

int main(){

    clock_t start, end;
    double cpu_time_used;
   	srand(time(NULL));


   	int A[N][N], B[N][N], T[N][N], AB[N][N], i, j;

	randomize(A, N);
	randomize(B, N);

    transpose(B, T);
    
    start = clock();
    mul_matrix_t(A, T, AB, N);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;	
    printf("time take by multpiplication: %f seconds\n", cpu_time_used);

    start = clock();
    mul_matrix(A, B, AB, N);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;	
    printf("time take by multpiplication: %f seconds\n", cpu_time_used);

    // print_matrix(A, N);
    // print_matrix(B, N);
    // print_matrix(T, N);
    // print_matrix(AB, N);

	return 0;
}
