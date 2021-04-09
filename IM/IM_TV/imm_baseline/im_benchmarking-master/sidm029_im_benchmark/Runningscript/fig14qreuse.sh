#!/bin/bash
execfileeasyim="../Executables/easyimIC"
pathtograph="../Dataset/hepph/graph_ic.inf"
model="IC"
outputeasyim="../Results/freshRuns/algorithmOutput/fig4a/easyim"


mkdir -p ../Results/freshRuns/processed/fig14q
python ../Codes/process5.py easyim $outputeasyim"/flowstat_" ../Results/freshRuns/processed/fig14q/output.txt
cp ../Results/Alreadypresent/fig14ijkl/plotnew.p ../Results/freshRuns/processed/fig14q/
cd ../Results/freshRuns/processed/fig14q/
gnuplot plotnew.p
epspdf output.eps
