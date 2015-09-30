#!/bin/bash

ROOT=$(realpath $(dirname $0))

# Download miniconda
cd test_suite
if [ ! -f Miniconda3-latest-Linux-x86_64.sh ]
then
    wget https://repo.continuum.io/miniconda/Miniconda3-latest-Linux-x86_64.sh
fi
cd ..

# Prepare provisioning archive
rm -f test_suite.tar.gz
tar cvzf test_suite.tar.gz test_suite

# Prepare result file.
log=$ROOT/results.txt
rm -f $log

for machine_path in \
    ubuntu/precise64 \
    ubuntu/vivid64 \
    boxcutter/fedora21
do
    echo $machine_path | tee -a $log
    echo "############################################" | tee -a $log
    cd $ROOT/machines/$machine_path
    vagrant destroy -f
    vagrant up
    vagrant destroy -f
    echo "openmpi" | tee -a $log
    echo "============================" | tee -a $log
    cat openmpi.log >> $log
    echo | tee -a $log
    echo | tee -a $log
done
