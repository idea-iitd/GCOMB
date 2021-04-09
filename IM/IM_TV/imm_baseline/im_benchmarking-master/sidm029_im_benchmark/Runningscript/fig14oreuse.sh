#!/bin/bash
#Usage: ./flow.sh <graph> <seeds> <data-set-name> 12
pathtograph="../../Dataset/hepph/graph_ic.inf"
model="IC"
outputsgcr="../Results/freshRuns/algorithmOutput/fig4f/sg"
cd ../Runningscript
mkdir -p ../Results/freshRuns/processed/fig14o
python ../Codes/process5.py static $outputsgcr"/flowst_snapshots=" ../Results/freshRuns/processed/fig14o/output.txt
cp ../Results/Alreadypresent/fig14ijkl/plotnew.p ../Results/freshRuns/processed/fig14o/
cd ../Results/freshRuns/processed/fig14o/
gnuplot plotnew.p
epspdf output.eps
