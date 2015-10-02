mpicc -c main.c 
mpicc main.o -lparmetis
mpirun -n 3 ./a.out
