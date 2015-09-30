#include "mpi.h" 
#include <stdio.h> 
#include <math.h> 

#include "compute_pi.h"

int main( int argc, char *argv[] ) 
{ 
    int rank, size; 
    double pi_ref = 3.141592653589793238462643; 
    double pi; 
    MPI_Init(&argc,&argv); 
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);

    printf("rank %d / %d\n", rank, size);

    int n = 100;
    double mypi = compute_pi(rank, size, n, MPI_COMM_WORLD);
    MPI_Reduce(&mypi, &pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD); 

    if (rank == 0)
        printf("pi is approximately %.16f, Error is %.16f\n", pi, fabs(pi - pi_ref)); 

    MPI_Finalize(); 

    return 0; 
} 
