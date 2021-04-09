#!/bin/bash
#Usage: ./flow.sh <graph> <seeds> <data-set-name> 12
execfilepmc="../Executables/pmc"
pathtograph="../Dataset/hepph/graph_wc.inf"
model="IC"
outputpmc="../Results/freshRuns/algorithmOutput/fig4g/pmc"
outputsgcreate="../Results/freshRuns/algorithmOutput/fig4g/sg"
mkdir -p ../Results/freshRuns/algorithmOutput/fig4g
mkdir -p $outputpmc
mkdir -p $outputsgcreate
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

mkdir -p ../Results/freshRuns/processed/fig4g
python ../Codes/convergenceCheck.py pmc "../Results/freshRuns/algorithmOutput/fig4g/pmc/flowst_300.txt_300" "../Results/freshRuns/algorithmOutput/fig4g/pmc/flowst_"  ../Results/freshRuns/processed/fig4g/pmc.txt

pathtographsg="../../Dataset/hepph/graph_wc.inf"
outputsg="../../Results/freshRuns/algorithmOutput/fig4g/sg"
execFlowsg="../../Executables/wcFlowSeeds"
for snapshots in 10 25 50 75 100 125 150 175 200 250 300
#for snapshots in 1 #to test use this line instead of above for quick result check
do
	for seeds in 1 2 5 10 25 50 75 100 125 150 175 200 #to test use this line instead of above for quick result check
	do
		echo "static greedy k="$seeds "snapshots="$snapshots
		cd ../Codes/static_greedy
		g++  -g -std=c++11  -g -o mymain *.cpp  -DMAX_NODE=11204 -DMAX_EDGE=470476
		./mymain $pathtographsg $seeds $snapshots sgd $outputsg	
		echo ./mymain $pathtographsg $seeds $snapshots sgd $outputsg	
		$execFlowsg $pathtographsg $outputsg"/seedStaticGreedyUD_SGD_R"$snapshots"_k"$seeds".txt"  $seeds 1 $numiter 3 $outputsg"/flowst_snapshots="$snapshots $outputsg"/spreadsnapshots="$snapshots".txt"
		cd ..
	done
done
cd ../Runningscript
python ../Codes/convergenceCheck.py static $outputsgcreate"/flowst_snapshots=300_300" "../Results/freshRuns/algorithmOutput/fig4g/sg/flowst_snapshots="  ../Results/freshRuns/processed/fig4g/sg.txt
cp ../Results/Alreadypresent/fig4g/plotnew.p ../Results/freshRuns/processed/fig4g/
cd ../Results/freshRuns/processed/fig4g/
gnuplot plotnew.p
epspdf output.eps
