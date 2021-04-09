#!/bin/bash
#Usage: ./flow.sh <graph> <seeds> <data-set-name> 12
pathtograph="../../Dataset/hepph/graph_wc.inf"
model="WC"
outputimrank="../../Results/freshRuns/algorithmOutput/fig4e/imrank"
outputimrankcr="../Results/freshRuns/algorithmOutput/fig4e/imrank"
cd ../Runningscript
mkdir -p ../Results/freshRuns/processed/fig14u
python ../Codes/process5.py imrank $outputimrankcr"/flowst_l=1_rounds\=" ../Results/freshRuns/processed/fig14u/output.txt
cp ../Results/Alreadypresent/fig14ijkl/plotnew.p ../Results/freshRuns/processed/fig14u/
cd ../Results/freshRuns/processed/fig14u/
gnuplot plotnew.p
epspdf output.eps
