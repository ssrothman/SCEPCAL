#!/bin/bash
#SBATCH --partition=physics
#SBATCH --job-name=scepcal
#SBATCH --output=/home/wonyongc/src/fcc/SCEPCAL/scepcalverb.out # stdout file
#SBATCH --error=/home/wonyongc/src/fcc/SCEPCAL/scepcalverb.err  # stderr file
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=4
#SBATCH --mem=50G
#SBATCH --time=1:00:00
#SBATCH --mail-type=all
#SBATCH --mail-user=wonyongc@princeton.edu
#SBATCH --constraint=cascade

module purge
# module load gcc-toolset/10

source /home/wonyongc/src/spack/share/spack/setup-env.sh
spack env activate -d /home/wonyongc/src/fcc/SCEPCAL/spack

# spack install scheduler=slurm
spack module tcl refresh
spack module tcl loads --dependencies dd4hep geant4 root edm4hep podio

source /home/wonyongc/src/fcc/SCEPCAL/spack/.spack-env/view/bin/thisroot.sh
source /home/wonyongc/src/fcc/SCEPCAL/spack/.spack-env/view/bin/geant4.sh
source /home/wonyongc/src/fcc/SCEPCAL/spack/.spack-env/view/bin/thisdd4hep.sh

export SCEPCAL=/home/wonyongc/src/fcc/SCEPCAL/install

export LD_LIBRARY_PATH=/lib64:/lib:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH=/home/wonyongc/src/fcc/SCEPCAL/spack/.spack-env/view/lib:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH=/home/wonyongc/src/fcc/SCEPCAL/spack/.spack-env/view/lib64:$LD_LIBRARY_PATH

export LD_LIBRARY_PATH=$SCEPCAL/lib64:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH=$SCEPCAL/lib:$LD_LIBRARY_PATH
export ROOT_INCLUDE_PATH=$SCEPCAL/include:$ROOT_INCLUDE_PATH
export PYTHONPATH=$SCEPCAL/python:$PYTHONPATH

$(which ddsim) --steeringFile /home/wonyongc/src/fcc/SCEPCAL/scripts/scepcal_steering.py -G --gun.energy "10*MeV" --gun.particle "gamma" --gun.direction "0 1 1" -O /home/wonyongc/src/fcc/SCEPCAL/myDRreadout_d011_slurm_verbose.root

