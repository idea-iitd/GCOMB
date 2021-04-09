#!/bin/bash
#Usage: ./flow.sh <graph> <seeds> <data-set-name> 12
execfilepmc="../Executables/pmc"
pathtograph="../Dataset/hepph/graph_wc.inf"
model="IC"
outputpmc="../Results/freshRuns/algorithmOutput/fig14n/pmc"
mkdir -p ../Results/freshRuns/algorithmOutput/fig14m
mkdir -p $outputpmc
numiter=10000
execFlow="../Executables/wcFlow"
for snapshots in 10 25 50 75 100 125 150 175 200 250 300
#for snapshots in 1 #to test use this line instead of above for quick result check
do
	echo "running pmc"
	echo $execfilepmc $pathtograph 200 $snapshots $outputpmc #graph k R outdir 
	$execfilepmc $pathtograph 200 $snapshots $outputpmc #graph k R outdir 
	#[sainyam@swarm2 Runningscript]$ less ../Results/freshRuns/algorithmOutput/fig4f/pmc/seeds_PMC_IC_1.txt 
	echo $execFlow $pathtograph $outputpmc"/seeds_PMC_IC_"$snapshots".txt" 200 1 $numiter 3 $outputpmc"/flowst_"$snapshots".txt" $outputpmc"/spread_"$snapshots".txt"
	$execFlow $pathtograph $outputpmc"/seeds_PMC_IC_"$snapshots".txt" 200 1 $numiter 3 $outputpmc"/flowst_"$snapshots".txt" $outputpmc"/spread_"$snapshots".txt"
done
mkdir -p ../Results/freshRuns/processed/fig14n
python ../Codes/process5.py pmc $outputpmc"/flowst_" ../Results/freshRuns/processed/fig14n/output.txt
