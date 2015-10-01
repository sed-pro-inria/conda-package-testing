#!/bin/bash

# This script is executed on the host machine.

ROOT=$(realpath $(dirname $0))

# Download miniconda
cd test_suite
if [ ! -f Miniconda3-latest-Linux-x86_64.sh ]
then
    wget --no-check-certificate https://repo.continuum.io/miniconda/Miniconda3-latest-Linux-x86_64.sh
fi
cd ..

# Prepare provisioning archive
rm -f test_suite.tar.gz
tar cvzf test_suite.tar.gz test_suite

# Prepare result file.
log=$ROOT/results.txt
rm -f $log

# Prepare cache directory
mkdir -p $ROOT/pkgs_cache

machine_paths="
    ubuntu/precise64
    ubuntu/vivid64
    boxcutter/fedora21
    joshbeard/scientific-6.6-64
    smak/archlinux
"

for machine_path in $machine_paths
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
