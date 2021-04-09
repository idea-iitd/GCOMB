#!/bin/bash
execfiletim="../Executables/tim"
pathtograph="../Dataset/hepph/graph_lt.inf"
model="LT"
outputtim="../Results/freshRuns/algorithmOutput/fig4j/tim"

execFlowSeeds="../Executables/ltFlowSeeds"
flowMC=10000
#flowMC=400 #to test use this line instead of above for quick result check

num=$(awk 'BEGIN{for(i=0.05;i<=1.05;i+=0.05)print i}')
#num=1.0 #to test use this line instead of above for quick result check

echo "Running TIM+"
mkdir -p ../Results/freshRuns/processed/fig14i
python ../Codes/process5.py tim $outputtim"/flowstat_" ../Results/freshRuns/processed/fig14i/output.txt

cp ../Results/Alreadypresent/fig14ijkl/plotnew.p ../Results/freshRuns/processed/fig14i/
cd ../Results/freshRuns/processed/fig14i/
gnuplot plotnew.p
epspdf output.eps
