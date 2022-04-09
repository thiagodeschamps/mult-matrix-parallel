//Universidade Federal de Sao Carlos
//Programação Paralela e Distribuída
//2020/12
//Atividade multiplicação de matrizes com OpenMP
//Vinicius Carvalho 743602
//João Augusto Leite 743551

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <omp.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/resource.h>

float *A, *B, *C;

int
main(int argc, char *argv[])
{

    int n_threads[8] = {1,2,4,8,16,32,64,128};
	int lin_a,col_a,lin_b,col_b,lin_c,col_c;
	int i,j,k;
    float elapsed_time_all[8];

    struct timeval inic,fim;
	struct rusage r1, r2;
 
	printf("Linhas A: ");
	scanf("%d",&lin_a);
	printf("Colunas A / Linhas B: ");
	scanf("%d",&col_a);
	lin_b=col_a;
	printf("Colunas B: ");
	scanf("%d",&col_b);
	printf("\n");
	lin_c=lin_a;
	col_c=col_b;

	// Alocacao dinamica das matrizes, com linhas em sequencia 
	A=(float *)malloc(lin_a*col_a*sizeof(float));
	B=(float *)malloc(lin_b*col_b*sizeof(float));
	

	// Atribucao de valores iniciais as matrizes 
    // Se não descomenentar a linha a seguir, os mesmos valores serão gerados em todas as execuções,
    // o que é bom no caso de fazermos uma análise comparativa.
	// srandom(time(NULL));

	for(i=0; i < lin_a * col_a; i++) 
		A[i]=(float)rand() / (float)RAND_MAX; 

	for(i=0; i < lin_b * col_b; i++) 
		B[i]=(float)rand() / (float)RAND_MAX; 

	
    for (int i = 0; i < 8; i++)
    {
        float media,valores[3];
        

        omp_set_num_threads(n_threads[i]);

        //Executa 3 vezes com a mesma quantidade de threads para criar a média
        for (int j = 0; j < 3; j++)
        {
        
        gettimeofday(&inic,0);
        getrusage(RUSAGE_SELF, &r1);

        gettimeofday(&fim,0);
        getrusage(RUSAGE_SELF, &r2);

        C=(float *)malloc(lin_c*col_c*sizeof(float));

        //Cria a paralelização para o for utilizando schedule dynamic para
        //evitar que threads que terminarem antes fiquem paradas ociosas. First
        //private para as matrizes base com o objetivo de manter os valores já atribuídos
        //na fase sequencial e i,j,k privados para cada thread ter seu próprio valor 
        //das variáveis de controle do loop
        #pragma omp parallel for schedule(dynamic) firstprivate(A,B) private(i,j,k)
        // calculo da multiplicacao
        for(i=0; i < lin_c; i++)
            for(j=0; j < col_c; j++) {
                C[i*col_c+j]=0;    
                for(k=0; k < col_a; k++) 
                    C[i*col_c+j] = C[i*col_c+j] + A[i*col_a+k] * B[k*col_b+j];
            }

        gettimeofday(&fim,0);
        getrusage(RUSAGE_SELF, &r2);

        //armazena o tempo de execução da iteração J
        valores[j] = (fim.tv_sec+fim.tv_usec/1000000.) - (inic.tv_sec+inic.tv_usec/1000000.);
        printf("Execução %d com %d threads: %f \n",j,n_threads[i],valores[j]);
        free(C);
        }
        
        //Média das 3 execuções
        media = (valores[0] + valores[1] + valores[2])/3;
        //Salva a média para o número de threads n_threads[i]
        elapsed_time_all[i] =  media;
    }
    
    for (int i = 0; i < 8; i++)
    {
        printf("Média do tempo de execução com %d threads: %f \n",n_threads[i],elapsed_time_all[i]);
    }

	return(0);
}