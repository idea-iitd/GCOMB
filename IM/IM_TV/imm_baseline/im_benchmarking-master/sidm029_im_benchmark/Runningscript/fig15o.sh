#!/bin/bash
#Usage: ./flow.sh <graph> <seeds> <data-set-name> 12
pathtograph="../../Dataset/youtube/graph_wc.inf"
model="WC"
edge=5975248
node=1134890
outputimrank="../../Results/freshRuns/algorithmOutput/fig15o/imrank"
outputimrankcr="../Results/freshRuns/algorithmOutput/fig15o/imrank"
mkdir -p ../Results/freshRuns/algorithmOutput/fig15o
mkdir -p $outputimrank

execFlow="../../Executables/wcFlowSeeds"
flowiter=10000
for l in 1 2
do
	for seeds in 1 2 5 10 25 50 75 100 125 150 175 200 #to test use this line instead of above for quick result check
	do
		for rounds in 1 2 3 4 5 6 7 8 9 10
		do
			echo "imrank k="$seeds "rounds="$rounds
			cd ../Codes/imrank
			g++  -g -std=c++11  -g -o mymain limit.h general_cascade.cpp graph.cpp random.cpp degree.cpp greedy.cpp degreediscount_ic.cpp weightedegree.cpp SPM_gc.cpp SP1M_gc.cpp pmia.cpp pagerank.cpp mia.cpp IMRank.cpp main.cpp -w "-DIMRank_LOOP="$rounds "-DSET_SIZE="$seeds -DMAX_NODE=$node -DMAX_EDGE=$edge
			./mymain -imrank $l $model $pathtograph $outputimrank
			$execFlow $pathtograph $outputimrank"/seedIMRank_k"$seeds"_l"$l"_LOOP"$rounds"_irDegree.txt" $seeds 1 $flowiter 3 $outputimrank"/flowst_l="$l"_rounds="$rounds".txt" $outputimrank"/spreadl="$l"_rounds="$rounds".txt"
			cd ..
		done
	done
done
cd ../Runningscript

mkdir -p ../Results/freshRuns/processed/fig15o
for l in 1 2
do
        python ../Codes/convergenceCheck.py imrank "../Results/freshRuns/algorithmOutput/fig15o/imrank/flowst_l="$l"_rounds=10.txt_300" "../Results/freshRuns/algorithmOutput/fig15o/imrank/flowst_l="$l"_rounds="  ../Results/freshRuns/processed/fig15o/$l".txt"
done
