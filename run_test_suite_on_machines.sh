#!/bin/bash

HERE=$(realpath $(dirname $0))

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
log=$HERE/results.txt
rm -f $log

# Run tests on ubuntu
echo "Ubuntu trusty64" | tee -a $log
echo "############################################" | tee -a $log
cd machines/ubuntu/trusty64
vagrant destroy -f
vagrant up
vagrant destroy -f
echo "openmpi" | tee -a $log
echo "============================" | tee -a $log
cat openmpi.log >> $log
