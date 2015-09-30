# Install miniconda
echo "Installing miniconda"
bash Miniconda3-latest-Linux-x86_64.sh -b
export PATH=$HOME/miniconda3/bin:$PATH
rm -f Miniconda3-latest-Linux-x86_64.sh

# Open MPi
echo "Running Open MPI test"
rm -f /vagrant/openmpi.out /vagrant/openmpi.err /vagrant/openmpi.ret
cd $HOME/test_suite/openmpi
conda install --yes --channel asmeurer --channel sed-pro-inria openmpi gcc
bash run_test.sh 2>&1 | tee /vagrant/openmpi.log
echo $? >> /vagrant/openmpi.log

echo "complete"
