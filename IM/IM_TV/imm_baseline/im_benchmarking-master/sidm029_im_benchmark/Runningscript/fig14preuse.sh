#!/bin/bash
#Usage: ./flow.sh <graph> <seeds> <data-set-name> 12
pathtograph="../../Dataset/hepph/graph_wc.inf"
model="WC"
outputsgcr="../Results/freshRuns/algorithmOutput/fig4g/sg"
cd ../Runningscript
mkdir -p ../Results/freshRuns/processed/fig14p
python ../Codes/process5.py static $outputsgcr"/flowst_snapshots=" ../Results/freshRuns/processed/fig14p/output.txt
cp ../Results/Alreadypresent/fig14ijkl/plotnew.p ../Results/freshRuns/processed/fig14p/
cd ../Results/freshRuns/processed/fig14p/
gnuplot plotnew.p
epspdf output.eps
