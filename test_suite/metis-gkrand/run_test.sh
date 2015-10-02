gcc -c -I$CONDA_ENV_PATH/include main.c 
gcc -L$CONDA_ENV_PATH/lib -Wl,-rpath,$CONDA_ENV_PATH/lib main.o -lmetis
./a.out
