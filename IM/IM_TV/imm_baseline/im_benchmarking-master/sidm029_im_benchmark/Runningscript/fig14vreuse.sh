#!/bin/bash
#Usage: ./flow.sh <graph> <seeds> <data-set-name> 12
pathtograph="../../Dataset/hepph/graph_ic.inf"
model="IC"
outputimrank="../../Results/freshRuns/algorithmOutput/fig4d/imrank"
outputimrankcr="../Results/freshRuns/algorithmOutput/fig4d/imrank"
cd ../Runningscript
mkdir -p ../Results/freshRuns/processed/fig14v
python ../Codes/process5.py imrank $outputimrankcr"/flowst_l=2_rounds=" ../Results/freshRuns/processed/fig14v/output.txt
cp ../Results/Alreadypresent/fig14ijkl/plotnew.p ../Results/freshRuns/processed/fig14v/
cd ../Results/freshRuns/processed/fig14v/
gnuplot plotnew.p
epspdf output.eps
