#!/bin/bash
#Usage: ./flow.sh <graph> <seeds> <data-set-name> 12
pathtograph="../../Dataset/hepph/graph_ic.inf"
model="IC"
outputimrank="../../Results/freshRuns/algorithmOutput/fig5a/imrank"
outputimrankcr="../Results/freshRuns/algorithmOutput/fig5a/imrank"
mkdir -p ../Results/freshRuns/algorithmOutput/fig5a
mkdir -p $outputimrankcr

execFlow="../../Executables/icFlowSeeds"
flowiter=10000
for l in 1
do
	#for seeds in 200 #to test use this line instead of above for quick result check
	for seeds in 1 50 100 150 200 #to test use this line instead of above for quick result check
	do
		for rounds in 1 2 3 4 5 6 7 8 9 10
		do
			echo "imrank k="$seeds "rounds="$rounds
			cd ../Codes/imrank
			g++  -g -std=c++11  -g -o mymain limit.h general_cascade.cpp graph.cpp random.cpp degree.cpp greedy.cpp degreediscount_ic.cpp weightedegree.cpp SPM_gc.cpp SP1M_gc.cpp pmia.cpp pagerank.cpp mia.cpp IMRank.cpp main.cpp -w "-DIMRank_LOOP="$rounds "-DSET_SIZE="$seeds -DMAX_NODE=11204 -DMAX_EDGE=235238
			./mymain -imrank $l "IC" $pathtograph $outputimrank
			$execFlow $pathtograph $outputimrank"/seedIMRank_k"$seeds"_l"$l"_LOOP"$rounds"_irDegree.txt" $seeds 1 $flowiter 3 $outputimrank"/flowst_l="$l"_rounds="$rounds".txt" $outputimrank"/spreadl="$l"_rounds="$rounds".txt"
			cd ..
		done
	done
done
cd ../Runningscript
mkdir -p ../Results/freshRuns/processed/fig5a
python ../Codes/process5.py imrank  $outputimrankcr"/flowst_l=1_rounds=" ../Results/freshRuns/processed/fig5a/output.txt
