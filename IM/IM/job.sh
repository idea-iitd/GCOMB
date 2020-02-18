#!/bin/bash
#PBS -N trainReinforcement
#PBS -P cse
#PBS -m bea
#PBS -M cs1150208@hpc.iitd.ac.in
#PBS -l select=1:ncpus=16:ngpus=1:mem=8gb
#PBS -l walltime=02:00:00
#PBS -o out.txt
#PBS -e error.txt
module load compiler/mpi/openmpi/3.0.1/gnu
module load compiler/gcc/5.1.0/compilervars
module load suite/intel/psxe2015/psxevars
module load compiler/cuda/8.0/compilervars
module load pythonpackages/2.7.13/ucs4/gnu/447/networkx/1.11/gnu
module load compiler/python/2.7.13/ucs4/gnu/447
module load pythonpackages/2.7.13/ucs4/gnu/447/numpy/1.12.1/gnu
module load pythonpackages/2.7.13/absl-py/0.1.10/gnu
module load pythonpackages/2.7.13/backports.weakref/1.0/gnu
module load pythonpackages/2.7.13/bleach/1.5.0/gnu
module load pythonpackages/2.7.13/ucs4/gnu/447/decorator/4.0.11/gnu
module load pythonpackages/2.7.13/enum34/1.1.6/gnu
module load pythonpackages/2.7.13/ucs4/gnu/447/funcsigs/1.0.2/gnu
module load pythonpackages/2.7.13/futures/3.2.0/gnu
module load pythonpackages/2.7.13/html5lib/0.99/gnu
module load pythonpackages/2.7.13/markdown/2.6.11/gnu
module load pythonpackages/2.7.13/ucs4/gnu/447/pbr/3.0.0/gnu
module load apps/protobuf/3.5.1/gnu
module load pythonpackages/2.7.13/protobuf/3.5.0/gnu
module load pythonpackages/2.7.13/ucs4/gnu/447/scikit-learn/0.18.1/gnu
module load pythonpackages/2.7.13/ucs4/gnu/447/scipy/0.19.0/gnu
module load pythonpackages/2.7.13/ucs4/gnu/447/six/1.10.0/gnu
module load pythonpackages/2.7.13/tensorflow_tensorboard/1.5.0/gnu
module load apps/pythonpackages/2.7.13/tensorflow/1.5.0/gpu
module load pythonpackages/2.7.13/tensorflow_tensorboard/1.5.0/gnu
module load pythonpackages/2.7.13/ucs4/gnu/447/werkzeug/0.12.1/gnu
cd /home/cse/btech/cs1150208/btp/btp/
sh ./run.sh
echo "end"
