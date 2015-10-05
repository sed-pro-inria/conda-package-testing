#ifndef MPIALGEBRA_HXX_INCLUDED
#define MPIALGEBRA_HXX_INCLUDED

#include "mpi.h" 

void
compute_sub_matrix_range(int size, int rank, int nrows,
                         int &irow0, int &irow1);

double*
matrix_dot_vector_on_master(double *matrix, double *vector,
                            int nrows, int ncols,
                            MPI_Comm comm);

void
matrix_dot_vector_on_slave(MPI_Comm comm, int master);

#endif
