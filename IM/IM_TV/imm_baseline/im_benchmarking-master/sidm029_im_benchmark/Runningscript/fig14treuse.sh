#!/bin/bash
#Usage: ./flow.sh <graph> <seeds> <data-set-name> 12
pathtograph="../../Dataset/hepph/graph_ic.inf"
model="IC"
outputimrankcr="../Results/freshRuns/algorithmOutput/fig4d/imrank"

cd ../Runningscript
mkdir -p ../Results/freshRuns/processed/fig14t
python ../Codes/process5.py imrank $outputimrankcr"/flowst_l=1_rounds=" ../Results/freshRuns/processed/fig14t/output.txt
cp ../Results/Alreadypresent/fig14ijkl/plotnew.p ../Results/freshRuns/processed/fig14t/
cd ../Results/freshRuns/processed/fig14t/
gnuplot plotnew.p
epspdf output.eps
