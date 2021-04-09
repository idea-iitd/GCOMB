#!/bin/bash
#Usage: ./flow.sh <graph> <seeds> <data-set-name> 12
pathtograph="../../Dataset/hepph/graph_wc.inf"
model="WC"
outputsg="../../Results/freshRuns/algorithmOutput/fig14p/sg"
outputsgcr="../Results/freshRuns/algorithmOutput/fig14p/sg"
mkdir -p ../Results/freshRuns/algorithmOutput/fig14p
mkdir -p $outputsgcr
numiter=10000
execFlowsg="../../Executables/wcFlowSeeds"
for snapshots in 10 25 50 75 100 125 150 175 200 250 300
for snapshots in 1 #to test use this line instead of above for quick result check
do
	for seeds in 1 2 5 10 25 50 75 100 125 150 175 200 #to test use this line instead of above for quick result check
	do
		echo "static greedy k="$seeds "snapshots="$snapshots
		cd ../Codes/static_greedy
		g++  -g -std=c++11  -g -o mymain *.cpp  -DMAX_NODE=11204 -DMAX_EDGE=470476
		./mymain $pathtograph $seeds $snapshots sgd $outputsg	
		echo ./mymain $pathtograph $seeds $snapshots sgd $outputsg	
		$execFlowsg $pathtograph $outputsg"/seedStaticGreedyUD_SGD_R"$snapshots"_k"$seeds".txt"  $seeds 1 $numiter 3 $outputsg"/flowst_snapshots="$snapshots $outputsg"/spreadsnapshots="$snapshots".txt"
		cd ..
	done
done
cd ../Runningscript
mkdir -p ../Results/freshRuns/processed/fig14p
python ../Codes/process5.py static $outputsgcr"/flowst_snapshots=" ../Results/freshRuns/processed/fig14p/output.txt
