#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define MS 2

int main(int argc, char* argv[]){

    int i, j, k;
    int x, c;
    int matrix_a[MS][MS];
    int matrix_b[MS][MS];
    int matrix_c[MS][MS];
    int myrank, p;
    int NRPE;
    double starttime, endtime;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    MPI_Status status;

    NRPE = MS / p;

    

}