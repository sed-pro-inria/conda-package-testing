mpif77 -c dsimpletest.F
mpif77 -o dsimpletest dsimpletest.o -ldmumps -lmumps_common -lpord -lscalapack -lopenblas
mpirun -np 2 ./dsimpletest < input_simpletest_real

mpicc -c c_example.c
mpicc -o c_example c_example.o -ldmumps -lmumps_common -lpord -lscalapack \
    -lopenblas -lmpi_mpifh -lgfortran -lm
mpirun -np 3 ./c_example
