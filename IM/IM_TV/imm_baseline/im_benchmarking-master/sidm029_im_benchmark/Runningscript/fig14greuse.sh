#!/bin/bash
execfiletim="../Executables/tim"
pathtograph="../Dataset/hepph/graph_ic.inf"
model="IC"
outputtim="../Results/freshRuns/algorithmOutput/fig4h/tim"

mkdir -p ../Results/freshRuns/processed/fig14g
python ../Codes/process5.py tim $outputtim"/flowstat_" ../Results/freshRuns/processed/fig14g/output.txt

cp ../Results/Alreadypresent/fig14ijkl/plotnew.p ../Results/freshRuns/processed/fig14g/
cd ../Results/freshRuns/processed/fig14g/
gnuplot plotnew.p
epspdf output.eps
