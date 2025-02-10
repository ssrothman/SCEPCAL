source /cvmfs/sw.hsf.org/key4hep/setup.sh

export TOP_DIR=$PWD
export MY_INSTALL_DIR=$PWD/install
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$MY_INSTALL_DIR/lib64
export PYTHONPATH=$PYTHONPATH:$MY_INSTALL_DIR/python
