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

# Run tests on Ubuntu Trusty
echo "Ubuntu trusty" | tee -a $log
echo "############################################" | tee -a $log
cd $ROOT/machines/ubuntu/trusty64
vagrant destroy -f
vagrant up
vagrant destroy -f
echo "openmpi" | tee -a $log
echo "============================" | tee -a $log
cat openmpi.log >> $log

# Run tests on Fedora 21
echo "Fedora 21" | tee -a $log
echo "############################################" | tee -a $log
cd $ROOT/machines/boxcutter/fedora21
vagrant destroy -f
vagrant up
vagrant destroy -f
echo "openmpi" | tee -a $log
echo "============================" | tee -a $log
cat openmpi.log >> $log
