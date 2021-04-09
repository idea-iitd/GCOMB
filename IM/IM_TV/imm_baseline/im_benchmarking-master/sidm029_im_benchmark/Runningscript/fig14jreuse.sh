#!/bin/bash
execfileimm="../Executables/imm"
pathtograph="../Dataset/hepph/graph_ic.inf"
model="IC"
outputimm="../Results/freshRuns/algorithmOutput/fig4h/imm"

execFlowSeeds="../Executables/icFlowSeeds"
flowMC=10000
#flowMC=400 #to test use this line instead of above for quick result check

num=$(awk 'BEGIN{for(i=0.05;i<=1.05;i+=0.05)print i}')
#num=1.0 #to test use this line instead of above for quick result check

echo "Running IMM"

mkdir -p ../Results/freshRuns/processed/fig14j
python ../Codes/process5.py imm $outputimm"/flowstat_" ../Results/freshRuns/processed/fig14j/output.txt

cp ../Results/Alreadypresent/fig14ijkl/plotnew.p ../Results/freshRuns/processed/fig14j/
cd ../Results/freshRuns/processed/fig14j/
gnuplot plotnew.p
epspdf output.eps
