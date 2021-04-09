#!/bin/bash
execfileimm="../Executables/imm"
pathtograph_ic="../Dataset/orkut/graph_ic.inf"
pathtograph_wc="../Dataset/orkut/graph_wc.inf"
model="IC"
outputimm_ic="../Results/freshRuns/algorithmOutput/fig1a/imm_ic"
outputimm_wc="../Results/freshRuns/algorithmOutput/fig1a/imm_wc"
mkdir -p ../Results/freshRuns/algorithmOutput/fig1a
mkdir -p $outputimm_ic
mkdir -p $outputimm_wc
mkdir -p ../Results/freshRuns/processed/fig1a
#eps=$(awk 'BEGIN{for(i=0.05;i<=1.05;i+=0.05)print i}')
eps=0.5

echo "running imm_ic" 
for seeds in 1 2 5 10 25 50 75 100 125 150 175 200
do
	echo $execfileimm" -dataset "$pathtograph_ic" -k "$seeds" -model "$model" -epsilon "$eps" -output "$outputimm_ic
	$execfileimm -dataset $pathtograph_ic -k $seeds -model $model -epsilon $eps -output $outputimm_ic
	tail -1 ../Results/freshRuns/algorithmOutput/fig1a/imm_ic/stat_IMM_IC_$k"_0.5.txt" >> ../Results/freshRuns/processed/fig1a/immic.txt
done

echo "running imm_wc" 
for seeds in 1 2 5 10 25 50 75 100 125 150 175 200
do
	echo $execfileimm" -dataset "$pathtograph_wc" -k "$seeds" -model "$model" -epsilon "$eps" -output "$outputimm_wc
	$execfileimm -dataset $pathtograph_wc -k $seeds -model $model -epsilon $eps -output $outputimm_wc
	tail -1 ../Results/freshRuns/algorithmOutput/fig1a/imm_wc/stat_IMM_IC_$k"_0.5.txt" >> ../Results/freshRuns/processed/fig1a/immwc.txt
done
