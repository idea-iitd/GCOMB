#!/bin/bash
#Usage: ./flow.sh <graph> <seeds> <data-set-name> 12
execfilepmc="../Executables/pmc"
pathtograph="../Dataset/hepph/graph_ic.inf"
model="IC"
outputpmc="../Results/freshRuns/algorithmOutput/fig4f/pmc"
mkdir -p ../Results/freshRuns/processed/fig14m
python ../Codes/process5.py pmc $outputpmc"/flowst_" ../Results/freshRuns/processed/fig14m/output.txt
cp ../Results/Alreadypresent/fig14ijkl/plotnew.p ../Results/freshRuns/processed/fig14m/
cd ../Results/freshRuns/processed/fig14m/
gnuplot plotnew.p
epspdf output.eps

