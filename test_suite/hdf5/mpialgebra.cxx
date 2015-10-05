#include "mpialgebra.hxx"

#include <iostream>

using namespace std;

void
matrix_dot_vector(double* matrix, double *vector, double *product,
                  int nrows, int ncols)
{
    for (int irow = 0 ; irow < nrows ; irow++) {
        product[irow] = 0.;
        for (int icol = 0 ; icol < ncols ; icol++) {
            product[irow] += matrix[irow*ncols + icol] * vector[icol];
        }
    }
}

void
compute_sub_matrix_range(int size, int rank, int nrows,
                         int &irow0, int &irow1)
{
    irow0 = nrows * rank/size;
    irow1 = nrows * (rank+1)/size ;
}

double*
matrix_dot_vector_on_master(double *matrix, double *vector,
                            int nrows, int ncols, MPI_Comm comm)
{
    int rank, size; 
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Allocate array.
    double* product = new double[nrows];

    for (int slave = 1 ; slave < size ; slave++) {
        int irow0;
        int irow1;
        compute_sub_matrix_range(size,slave,nrows,irow0,irow1);

        // Send matrix shape to slave.
        int matrix_shape[2] = {nrows, ncols};
        int tag = 0;
        MPI_Send(matrix_shape, 2, MPI_INT, slave, tag, comm);

        // Send sub-matrix to slave.
        MPI_Send(matrix+irow0*ncols, (irow1-irow0)*ncols,
                 MPI_DOUBLE, slave, tag, comm);

        // Send vector to slave.
        MPI_Send(vector, ncols, MPI_DOUBLE, slave, tag, comm);

        // Receive sub-product from slave.
        MPI_Status status;
        int err = MPI_Recv(product+irow0, irow1-irow0, MPI_DOUBLE,
                           slave, tag, comm, &status);
        if (err != MPI_SUCCESS) cerr << "MPI error: " << err << endl;
    }

    // Compute sub-product on master.
    int irow0;
    int irow1;
    compute_sub_matrix_range(size,rank,nrows,irow0,irow1);
    matrix_dot_vector(matrix+irow0*ncols, vector,
                      product+irow0, irow1-irow0, ncols);

    return product;
}

void
matrix_dot_vector_on_slave(MPI_Comm comm, int master)
{
    int rank, size; 
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Receive matrix shape from master.
    int matrix_shape[2];
    int tag = 0;
    MPI_Status status;
    int err = MPI_Recv(matrix_shape, 2, MPI_INT, master, tag, comm, &status);
    if (err != MPI_SUCCESS) cerr << "MPI error: " << err << endl;
    int nrows = matrix_shape[0];
    int ncols = matrix_shape[1];

    int irow0;
    int irow1;
    compute_sub_matrix_range(size,rank,nrows,irow0,irow1);
    int nsubrows = irow1 - irow0;

    // Allocate arrays.
    double* sub_matrix = new double[nsubrows*ncols];
    double* vector = new double[ncols];

    // Receive sub-matrix from master.
    err = MPI_Recv(sub_matrix, nsubrows*ncols,
                   MPI_DOUBLE, master, tag, comm, &status);
    if (err != MPI_SUCCESS) cerr << "MPI error: " << err << endl;

    // Receive vector from master.
    err = MPI_Recv(vector, ncols, MPI_DOUBLE, master, tag, comm, &status);
    if (err != MPI_SUCCESS) cerr << "MPI error: " << err << endl;

    // Compute sub-product.
    double* sub_product = new double[nsubrows];
    matrix_dot_vector(sub_matrix, vector, sub_product, nsubrows, ncols);

    // Send sub_product to master.
    MPI_Send(sub_product, nsubrows, MPI_DOUBLE, master, tag, comm);

    delete[] sub_matrix;
    delete[] vector;
    delete[] sub_product;
}
