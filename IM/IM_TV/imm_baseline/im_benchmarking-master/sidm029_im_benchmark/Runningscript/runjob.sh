#!/bin/bash
#
#SBATCH --job-name=test
#SBATCH --output=res_%j.txt  # output file
#SBATCH -e res_%j.err        # File to which STDERR will be written
#SBATCH --partition=longq    # Partition to submit to 
#
#SBATCH --ntasks=1
#SBATCH --time=2-24:00         # Runtime in D-HH:MM
#SBATCH --mem-per-cpu=60000    # Memory in MB per cpu allocated

$1
sleep 1
exit

