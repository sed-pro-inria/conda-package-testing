mpicc -D_LONGINT -g -c pddrive.c
mpicc -D_LONGINT -g -c dcreate_matrix.c
mpicc -o pddrive dcreate_matrix.o pddrive.o \
    -lsuperlu_dist_4.0 -lopenblas -lparmetis -lmetis -lm

mpirun -n 4 ./pddrive -r 2 -c 2 g20.rua
