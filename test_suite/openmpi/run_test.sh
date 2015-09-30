mpicc -c compute_pi.c
mpicc -c main.c
mpicc compute_pi.o main.o

mpirun -n 4 ./a.out
