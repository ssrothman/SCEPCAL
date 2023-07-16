#!/bin/bash
source /cvmfs/sw.hsf.org/key4hep/setup.sh
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/eos/user/w/wochung/src/SCEPCal/install/lib64
export PYTHONPATH=$PYTHONPATH:/eos/user/w/wochung/src/SCEPCal/install/python

ddsim /eos/user/w/wochung/src/SCEPCal/scripts/scepcal_steering.py