#include "compute_pi.h"

double compute_pi(int mpi_rank, int mpi_size, int n, MPI_Comm comm)
{
    int i;
    double h = 1.0 / (double) n; 
    double sum = 0.0; 
    double x;

    for (i = mpi_rank + 1; i <= n; i += mpi_size) { 
        x = h * ((double)i - 0.5); 
        sum += (4.0 / (1.0 + x*x)); 
    } 

    return h * sum; 
}
