#!/bin/bash
execfileimm="../Executables/imm"
outputimm="../Results/freshRuns/algorithmOutput/fig4i/imm"


echo "Running IMM"

mkdir -p ../Results/freshRuns/processed/fig14k
python ../Codes/process5.py imm $outputimm"/flowstat_" ../Results/freshRuns/processed/fig14k/output.txt

cp ../Results/Alreadypresent/fig14ijkl/plotnew.p ../Results/freshRuns/processed/fig14k/
cd ../Results/freshRuns/processed/fig14k/
gnuplot plotnew.p
epspdf output.eps
