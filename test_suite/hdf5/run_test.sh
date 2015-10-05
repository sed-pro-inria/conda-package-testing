#!/bin/bash

# Safe shell
set -o nounset
set -o errexit
set -o pipefail

bash create_inputs.sh

mkdir build
cd build
cmake ..
make

cd ..
mpirun -np 4 build/main input.h5
