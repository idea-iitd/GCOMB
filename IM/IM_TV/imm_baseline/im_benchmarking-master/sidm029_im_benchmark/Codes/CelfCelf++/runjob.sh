#!/bin/bash
#
#SBATCH --job-name=test
#SBATCH --output=res_%j.txt  # output file
#SBATCH -e res_%j.err        # File to which STDERR will be written
#SBATCH --partition=longq    # Partition to submit to 
#
#SBATCH --ntasks=1
#SBATCH --time=30-4:00         # Runtime in D-HH:MM
#SBATCH --mem-per-cpu=10000    # Memory in MB per cpu allocated

$1
sleep 1
exit

