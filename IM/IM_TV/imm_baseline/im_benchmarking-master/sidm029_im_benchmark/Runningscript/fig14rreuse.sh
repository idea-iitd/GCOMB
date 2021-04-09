#!/bin/bash
execfileeasyim="../Executables/easyimWC"
pathtograph="../Dataset/hepph/graph_wc.inf"
model="IC"
outputeasyim="../Results/freshRuns/algorithmOutput/fig4b/easyim"


mkdir -p ../Results/freshRuns/processed/fig14r
python ../Codes/process5.py easyim $outputeasyim"/flowstat_" ../Results/freshRuns/processed/fig14r/output.txt
cp ../Results/Alreadypresent/fig14ijkl/plotnew.p ../Results/freshRuns/processed/fig14r/
cd ../Results/freshRuns/processed/fig14r/
gnuplot plotnew.p
epspdf output.eps
