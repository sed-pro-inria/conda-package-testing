#include <iostream>
#include <iomanip>
 
#include "H5Cpp.h"
#include "mpi.h" 

#include "mpialgebra.hxx"

using namespace std;
using namespace H5;

void
read_matrix_vector(char const * const filename, 
                   double* &matrix,
                   double* &vector,
                   hsize_t &nrows,
                   hsize_t &ncols)
{
    // Open file with read-only mode.
    H5File file(filename, H5F_ACC_RDONLY);

    // Select dataset.
    DataSet dataset_matrix = file.openDataSet("/matrix");
    DataSet dataset_vector = file.openDataSet("/vector");

    // Allocate matrix and vector.
    matrix = new double[dataset_matrix.getSpace().getSimpleExtentNpoints()];
    vector = new double[dataset_vector.getSpace().getSimpleExtentNpoints()];

    // Read matrix and vector.
    dataset_matrix.read(matrix, dataset_matrix.getDataType());
    dataset_vector.read(vector, dataset_vector.getDataType());

    // Read dimensions.
    DataSpace dataspace = dataset_matrix.getSpace();
    int ndim = dataspace.getSimpleExtentNdims();
    hsize_t dims[ndim];
    dataspace.getSimpleExtentDims(dims, NULL);
    nrows = dims[0];
    ncols = dims[1];
}


void
print_vector(double* vector, hsize_t ncols)
{
    for (hsize_t icol = 0 ; icol < ncols ; icol++) {  
        cout << setw(2) << vector[icol] << " ";
    }
    cout << endl;
}

void
print_matrix(double* matrix, hsize_t nrows, hsize_t ncols)
{
    for (hsize_t irow = 0 ; irow < nrows ; irow++) {  
        for (hsize_t icol = 0 ; icol < ncols ; icol++) {  
            cout << setw(2) << matrix[irow*ncols + icol] << " ";
        }
        cout << endl;
    }
}

int main(int argc, char** argv)
{
    //Check command line args.
    if (argc !=2)
    {  
        cout << "Usage: main FILE.H5" << endl;
        return 1;
    }
    char const * const filename = argv[1];

    int rank, size; 
    MPI_Init(&argc, &argv); 
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int master = 0;

    if (rank == master) {
        double* matrix;
        double* vector;
        hsize_t nrows;
        hsize_t ncols;

        read_matrix_vector(filename, matrix, vector, nrows, ncols);

        cout << "vector:" << endl;
        print_vector(vector, ncols);

        cout << "matrix:" << endl;
        print_matrix(matrix, nrows, ncols);

        double* product = matrix_dot_vector_on_master(matrix, vector,
                                                      nrows, ncols,
                                                      MPI_COMM_WORLD);

        cout << "product:" << endl;
        print_vector(product, nrows);

        delete[] matrix;
        delete[] product;
        delete[] vector;

    } else {
        matrix_dot_vector_on_slave(MPI_COMM_WORLD, master);
    }

    MPI_Finalize(); 

    return 0;
}

