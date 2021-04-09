#!/bin/bash
execfileimm="../Executables/imm"
execfiletim="../Executables/tim"
pathtograph="../Dataset/youtube/graph_ic.inf"
model="IC"
outputimm="../Results/freshRuns/algorithmOutput/fig4h/imm"
outputtim="../Results/freshRuns/algorithmOutput/fig4h/tim"
mkdir -p ../Results/freshRuns/algorithmOutput/fig4h
mkdir -p $outputimm
mkdir -p $outputtim

execFlowSeeds="../Executables/icFlowSeeds"
flowMC=10000
#flowMC=400 #to test use this line instead of above for quick result check
eps=0.1
for seeds in 75 100
do
	echo $execfileimm" -dataset "$pathtograph" -k "$seeds" -model "$model" -epsilon "$eps" -output "$outputimm
	$execfileimm -dataset $pathtograph -k $seeds -model $model -epsilon $eps -output $outputimm

done


