// Code for multiple 2 matrix using dynamic memory allocation

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define D 5
#define MAX_VALUE 100

void randomize(int **matrix, int l){
	
	int n;
	
	for(int i = 0; i < l; i++){
		for(int j = 0; j < l; j++){
			n = rand() % MAX_VALUE;
			matrix[i][j] = n;
		}	
	}
}

void print_matrix(int **matrix, int l){
	for(int i = 0; i < l; i++){
		for(int j = 0; j < l; j++){
			printf("%d ", matrix[i][j]);
		}
		printf("\n");		
	}
    printf("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n");
}

void mul_matrix(int **matrix_a, int **matrix_b, int **matrix_ab, int l){
	for(int i = 0; i < l; i++){
		for(int j = 0; j < l; j++){
			matrix_ab[i][j] = 0;
			for(int k = 0; k < l; k++){
				matrix_ab[i][j] += matrix_a[i][k] * matrix_b[k][j];
			}
		}
	}
	
}


int main(int argc, char *argv[]){
	
    // Setting up seed to random function
	srand(time(NULL));
	
    // Allocating memory to matrix A
	int **matrix_a = malloc(D * sizeof(int*));
	for(int i = 0; i < D; i++)
		matrix_a[i] = malloc(D * sizeof(int));
	
	// Allocating memory to matrix B
	int **matrix_b = malloc(D * sizeof(int*));
	for(int i = 0; i < D; i++)
		matrix_b[i] = malloc(D * sizeof(int));
	
	// Alocating memory for the resulting matrix
	int **matrix_ab_normal = malloc(D * sizeof(int*));
	for(int i = 0; i < D; i++)
		matrix_ab_normal[i] = malloc(D * sizeof(int));
	
    // Populating matrix with random numbers
	randomize(matrix_a, D);
	randomize(matrix_b, D);
	
	mul_matrix(matrix_a, matrix_b, matrix_ab_normal, D);
	
	print_matrix(matrix_a, D);
	print_matrix(matrix_b, D); 
	print_matrix(matrix_ab_normal, D);
	
	for(int i = 0; i < D; i++)
		free(matrix_a[i]);
	free(matrix_a);
	
	for(int i = 0; i < D; i++)
		free(matrix_b[i]);
	free(matrix_b);
	
	for(int i = 0; i < D; i++)
		free(matrix_ab_normal[i]);
	free(matrix_ab_normal);
	
	return 0;
	
}