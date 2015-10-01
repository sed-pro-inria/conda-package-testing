#!/bin/bash

# This script is executed on guest machines.

function cp_cache_archives
{
    src=$1
    dst=$2
    here=$(pwd)
    cd $src
    for archive in $(ls *.tar.bz2)
    do
        if [ ! -f $dst/$archive ]
        then
            echo "cp $archive $dst"
            cp $archive $dst
        fi
    done
    cd $here
}

CONDA_DIR=$HOME/miniconda3
SYNCED_PKGS_CACHE_DIR=$HOME/pkgs_cache

# Install miniconda
echo "Installing miniconda"
bash Miniconda3-latest-Linux-x86_64.sh -b
export PATH=$CONDA_DIR/bin:$PATH
rm -f Miniconda3-latest-Linux-x86_64.sh

# Package cache.
cp_cache_archives $SYNCED_PKGS_CACHE_DIR $CONDA_DIR/pkgs

# openmpi
echo "Running openmpi test"
rm -f /vagrant/openmpi.log
cd $HOME/test_suite/openmpi
conda install --yes --channel asmeurer --channel sed-pro-inria openmpi gcc
bash run_test.sh 2>&1 | tee /vagrant/openmpi.log
echo $? >> /vagrant/openmpi.log

# Package cache.
cp_cache_archives $CONDA_DIR/pkgs $SYNCED_PKGS_CACHE_DIR

echo "complete"
