#!/bin/bash
#Usage: ./flow.sh <graph> <seeds> <data-set-name> 12
pathtograph="../../Dataset/hepph/graph_ic.inf"
model="IC"
outputimrankcr="../Results/freshRuns/algorithmOutput/fig4d/imrank"

cd ../Runningscript
mkdir -p ../Results/freshRuns/processed/fig5a
python ../Codes/process5.py imrank $outputimrankcr"/flowst_l=1_rounds=" ../Results/freshRuns/processed/fig5a/output.txt
cp ../Results/Alreadypresent/fig14ijkl/plotnew.p ../Results/freshRuns/processed/fig5a/
cd ../Results/freshRuns/processed/fig5a/
gnuplot plotnew.p
epspdf output.eps
