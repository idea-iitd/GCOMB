#!/bin/bash
execfiletim="../Executables/tim"
pathtograph="../Dataset/hepph/graph_wc.inf"
model="IC"
outputtim="../Results/freshRuns/algorithmOutput/fig4i/tim"
mkdir -p ../Results/freshRuns/processed/fig14h
python ../Codes/process5.py tim $outputtim"/flowstat_" ../Results/freshRuns/processed/fig14h/output.txt
cp ../Results/Alreadypresent/fig14ijkl/plotnew.p ../Results/freshRuns/processed/fig14h/
cd ../Results/freshRuns/processed/fig14h/
gnuplot plotnew.p
epspdf output.eps
