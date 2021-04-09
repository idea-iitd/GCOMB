#!/bin/bash
execfileimm="../Executables/imm"
outputimm="../Results/freshRuns/algorithmOutput/fig4j/imm"


echo "Running IMM"

mkdir -p ../Results/freshRuns/processed/fig14l
python ../Codes/process5.py imm $outputimm"/flowstat_" ../Results/freshRuns/processed/fig14l/output.txt

cp ../Results/Alreadypresent/fig14ijkl/plotnew.p ../Results/freshRuns/processed/fig14l/
cd ../Results/freshRuns/processed/fig14l/
gnuplot plotnew.p
epspdf output.eps
