/*
** PPD / DC/UFSCar - Helio
** Programa : multiplicacao de matrizes
** Objetivo: paralelizacao om OpenMP
** Alunos :
** 
** Wesley Nogueira Galvão 757665
   gcc matriz_multi_paralelo.c -o mmp -lm -fopenmp && ./mmp
*/

#include <math.h> 
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <sys/time.h>
#include <omp.h>


float *A, *B, *C;

void multiplicacaoMatrizes(int lin_a, int col_a, int col_b, int num_threads){

    int lin_b = col_a; 
    int lin_c = lin_a;
    int col_c = col_b;
    int i,j,k;
    
    C=(float *)malloc(lin_c*col_c*sizeof(float));
    omp_set_num_threads(num_threads);

    #pragma omp parallel for schedule(dynamic) firstprivate(A,B) private(i,j,k)
    for(i=0; i < lin_c; i++)
        for(j=0; j < col_c; j++) {

        C[i*col_c+j]=0;
    
        for(k=0; k < col_a; k++) 
            C[i*col_c+j] = C[i*col_c+j] + A[i*col_a+k] * B[k*col_b+j];
        }
}

int main(int argc, char *argv[]){
  
    int lin_a,col_a,lin_b,col_b,lin_c,col_c;
    int i,j,k;


    struct timeval begin, end;


    //printf("Linhas A: ");   
    scanf("%d",&lin_a);
    //printf("Colunas A / Linhas B: "); 
    scanf("%d",&col_a);
    lin_b=col_a;
    //printf("Colunas B: ");  
    scanf("%d",&col_b);
    //printf("\n");
    lin_c=lin_a;
    col_c=col_b;

    // Alocacao dinamica das matrizes, com linhas em sequencia 
    A = (float *)malloc(lin_a*col_a*sizeof(float));
    B = (float *)malloc(lin_b*col_b*sizeof(float));
    //C = (float *)malloc(lin_c*col_c*sizeof(float));

    // Atribucao de valores iniciais as matrizes 
    srandom(time(NULL));


    for(i=0; i < lin_a * col_a; i++) 
        A[i]=(float)rand() / (float)RAND_MAX; 

    for(i=0; i < lin_b * col_b; i++) 
        B[i]=(float)rand() / (float)RAND_MAX;

    // calculo da multiplicacao
    
    // Qual/quais loop(s) paralelizar? Vale a pena paralelizar todos?
    // Qual é o efeito de fazer um parallel for em cada um dos fors abaixo?
    // É necessários sincronizar alguma operação, garantindo exclusão mútua?

    
    int num_execs;
    //printf("Quantidade de iteracoes por thread (para media): ");   
    scanf("%d",&num_execs);
    int num_threads;
    int expoente = 0;
    printf("Exec_id; num_threads; elapsed_time;\n");
    while(expoente < 8 ){

        num_threads = pow(2, expoente);        

        for(int exec = 0; exec < num_execs; exec++){
            // Start measuring time
            gettimeofday(&begin, 0);

            multiplicacaoMatrizes(lin_a, col_a, col_b, num_threads);

            // Stop measuring time and calculate the elapsed time
            gettimeofday(&end, 0);
            long seconds = end.tv_sec - begin.tv_sec;
            long microseconds = end.tv_usec - begin.tv_usec;
            double elapsed = seconds + microseconds*1e-6;
            printf("%d; %d; %.6f\n", exec+1, num_threads, elapsed);
            //printf("Execução %d Num Threads %d \n", exec+1, num_threads);
            //printf("  Tempo de execução: %.6f s\n", elapsed);
        }
        expoente++;
        printf("\n");
    } 
    
    // desaloca as matrizes
        free(A);
        free(B);
        free(C);

    return(0);
}