#!/bin/bash
#SBATCH --job-name=SCEPCal
#SBATCH --output=/scratch/gpfs/wonyongc/scepcal.out
#SBATCH --error=/scratch/gpfs/wonyongc/scepcal.err
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=1
#SBATCH --mem=32G
#SBATCH --time=10:00:00
#SBATCH --mail-type=all
#SBATCH --mail-user=wonyongc@princeton.edu

ddsim --steeringFile /scratch/gpfs/wonyongc/scepcal_steering.py
        