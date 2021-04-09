#!/bin/bash
pathtograph="../Dataset/hepph/graph_wc.inf"
model="WC"
outputimrankCorrect="../Results/freshRuns/algorithmOutput/fig10f/imrank_correct"
outputimrankWrong="../Results/freshRuns/algorithmOutput/fig10f/imrank_wrong"
mkdir -p ../Results/freshRuns/algorithmOutput/fig10f
mkdir -p ../Results/freshRuns/processed/fig10f
mkdir -p $outputimrankCorrect
mkdir -p $outputimrankWrong

execFlow="../Executables/wcFlowSeeds"
flowiter=10000
#flowiter=400 #To test quickly use this, for actual use the above line and comment this one!
for l in 1 2
do
	echo "Running IMRank with the (incorrect) convergence proposed by the authors, l = "$l
	for seeds in 1 2 5 10 25 50 75 100 125 150 175 200
	do
		echo "imrank k="$seeds
		cd ../Codes/imrank_incorrect
		g++  -g -std=c++11  -g -o mymain limit.h general_cascade.cpp graph.cpp random.cpp degree.cpp greedy.cpp degreediscount_ic.cpp weightedegree.cpp SPM_gc.cpp SP1M_gc.cpp pmia.cpp pagerank.cpp mia.cpp IMRank.cpp main.cpp -w "-DSET_SIZE="$seeds -DMAX_NODE=11204 -DMAX_EDGE=235238
		echo g++  -g -std=c++11  -g -o mymain limit.h general_cascade.cpp graph.cpp random.cpp degree.cpp greedy.cpp degreediscount_ic.cpp weightedegree.cpp SPM_gc.cpp SP1M_gc.cpp pmia.cpp pagerank.cpp mia.cpp IMRank.cpp main.cpp -w "-DSET_SIZE="$seeds -DMAX_NODE=11204 -DMAX_EDGE=235238
		cp mymain ../../Executables/
		cd ../../Executables
		echo ./mymain -imrank $l "WC" $pathtograph $outputimrankWrong
		./mymain -imrank $l "WC" $pathtograph $outputimrankWrong
		$execFlow $pathtograph $outputimrankWrong"/seedIMRank_k"$seeds"_l"$l"_LOOP10_irDegree.txt" $seeds 1 $flowiter 3 $outputimrankWrong"/flowst_l="$l".txt" $outputimrankWrong"/spreadl="$l".txt"
	done
done

cd ../Runningscript
cp $outputimrankWrong"/flowst_l=1.txt_300" ../Results/freshRuns/processed/fig10f/wrong.txt
for l in 1 2
do
	echo "Running IMRank with corrected convergence, l = "$l
	for seeds in 1 2 5 10 25 50 75 100 125 150 175 200
	do
		for rounds in 10
		do
			echo "imrank k="$seeds "rounds="$rounds
			cd ../Codes/imrank
			g++  -g -std=c++11  -g -o mymain limit.h general_cascade.cpp graph.cpp random.cpp degree.cpp greedy.cpp degreediscount_ic.cpp weightedegree.cpp SPM_gc.cpp SP1M_gc.cpp pmia.cpp pagerank.cpp mia.cpp IMRank.cpp main.cpp -w "-DIMRank_LOOP="$rounds "-DSET_SIZE="$seeds -DMAX_NODE=11204 -DMAX_EDGE=235238
			cp mymain ../../Executables/
                	cd ../../Executables
			./mymain -imrank $l "WC" $pathtograph $outputimrankCorrect
			$execFlow $pathtograph $outputimrankCorrect"/seedIMRank_k"$seeds"_l"$l"_LOOP"$rounds"_irDegree.txt" $seeds 1 $flowiter 3 $outputimrankCorrect"/flowst_l="$l"_rounds="$rounds".txt" $outputimrankCorrect"/spreadl="$l"_rounds="$rounds".txt"
		done
	done
done
cd ../Runningscript
cp $outputimrankCorrect"/flowst_l=1_rounds=10.txt_300" ../Results/freshRuns/processed/fig10f/correct.txt

cp ../Results/Alreadypresent/fig10f/plotnew.p ../Results/freshRuns/processed/fig10f/
cd ../Results/freshRuns/processed/fig10f/
gnuplot plotnew.p
epspdf output.eps
