#!/bin/bash
execfileeasyim="../Executables/easyimLT"
pathtograph="../Dataset/hepph/graph_lt.inf"
model="LT"
outputeasyim="../Results/freshRuns/algorithmOutput/fig4c/easyim"
mkdir -p ../Results/freshRuns/processed/fig14s
python ../Codes/process5.py easyim $outputeasyim"/flowstat_" ../Results/freshRuns/processed/fig14s/output.txt
cp ../Results/Alreadypresent/fig14ijkl/plotnew.p ../Results/freshRuns/processed/fig14s/
cd ../Results/freshRuns/processed/fig14s/
gnuplot plotnew.p
epspdf output.eps
