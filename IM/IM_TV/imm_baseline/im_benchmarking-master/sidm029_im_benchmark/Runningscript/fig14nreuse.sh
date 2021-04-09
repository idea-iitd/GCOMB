#!/bin/bash
#Usage: ./flow.sh <graph> <seeds> <data-set-name> 12
execfilepmc="../Executables/pmc"
pathtograph="../Dataset/hepph/graph_wc.inf"
model="IC"
outputpmc="../Results/freshRuns/algorithmOutput/fig4g/pmc"
mkdir -p ../Results/freshRuns/processed/fig14n
python ../Codes/process5.py pmc $outputpmc"/flowst_" ../Results/freshRuns/processed/fig14n/output.txt
cp ../Results/Alreadypresent/fig14ijkl/plotnew.p ../Results/freshRuns/processed/fig14n/
cd ../Results/freshRuns/processed/fig14n/
gnuplot plotnew.p
epspdf output.eps
