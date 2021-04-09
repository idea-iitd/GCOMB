#!/bin/bash
execfileimm="../Executables/imm"
execfileeasyim="../Executables/easyimIC"
pathtograph="../Dataset/youtube/graph_ic.inf"
model="IC"
outputimm="../Results/freshRuns/algorithmOutput/fig1b-1c/imm"
outputeasyim="../Results/freshRuns/algorithmOutput/fig1b-1c/easyim"
mkdir -p ../Results/freshRuns/algorithmOutput/fig1b-1c
mkdir -p ../Results/freshRuns/processed/fig1b
mkdir -p ../Results/freshRuns/processed/fig1c
mkdir -p $outputimm
mkdir -p $outputeasyim

eps=0.5
echo "running imm" 
for seeds in 1 2 5 10 25 50 75 100 125 150 175 200
do
	echo $execfileimm" -dataset "$pathtograph" -k "$seeds" -model "$model" -epsilon "$eps" -output "$outputimm
	$execfileimm -dataset $pathtograph -k $seeds -model $model -epsilon $eps -output $outputimm
	tail -1 ../Results/freshRuns/algorithmOutput/fig1b-1c/imm/stat_IMM_IC_$k"_0.5.txt" >> ../Results/freshRuns/processed/fig1b/imm.txt
	tail -1 ../Results/freshRuns/algorithmOutput/fig1b-1c/imm/stat_IMM_IC_$k"_0.5.txt" >> ../Results/freshRuns/processed/fig1c/imm.txt
done

iter=100
echo "running easyim" 
for seeds in 1 2 5 10 25 50 75 100 125 150 175 200
do
	echo $execfileeasyim $pathtograph" 2 "$seeds" 1 "$iter" 3 "$outputeasyim
	$execfileeasyim $pathtograph 2 $seeds 1 $iter 3 $outputeasyim
	tail -1 ../Results/freshRuns/algorithmOutput/fig1b-1c/easyim/stat_seeds=$seeds"_iter="$iter"_depth=3.txt" >> ../Results/freshRuns/processed/fig1b/easyim.txt
	tail -1 ../Results/freshRuns/algorithmOutput/fig1b-1c/easyim/stat_seeds=$seeds"_iter="$iter"_depth=3.txt" >> ../Results/freshRuns/processed/fig1c/easyim.txt
done
