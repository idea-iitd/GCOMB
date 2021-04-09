#!/bin/bash
#Usage: ./flow.sh <graph> <seeds> <data-set-name> 12
pathtograph="../../Dataset/hepph/graph_ic.inf"
model="WC"
outputimrank="../../Results/freshRuns/algorithmOutput/fig4e/imrank"
outputimrankcr="../Results/freshRuns/algorithmOutput/fig4e/imrank"

cd ../Runningscript
mkdir -p ../Results/freshRuns/processed/fig14w
python ../Codes/process5.py imrank $outputimrankcr"/flowst_l=2_rounds=" ../Results/freshRuns/processed/fig14w/output.txt
cp ../Results/Alreadypresent/fig14ijkl/plotnew.p ../Results/freshRuns/processed/fig14w/
cd ../Results/freshRuns/processed/fig14w/
gnuplot plotnew.p
epspdf output.eps
