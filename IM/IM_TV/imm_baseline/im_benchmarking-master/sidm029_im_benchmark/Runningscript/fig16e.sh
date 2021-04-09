#!/bin/bash
#Usage: ./flow.sh <graph> <seeds> <data-set-name> 12
execfilepmc="../Executables/pmc"
pathtograph="../Dataset/youtube/graph_ic.inf"
model="IC"
outputpmc="../Results/freshRuns/algorithmOutput/fig16e/pmc"
outputsg="../../Results/freshRuns/algorithmOutput/fig16e/sg"
outputsgcr="../Results/freshRuns/algorithmOutput/fig16e/sg"
mkdir -p ../Results/freshRuns/algorithmOutput/fig16e
mkdir -p $outputpmc
mkdir -p $outputsgcr
numiter=10000
execFlow="../Executables/icFlow"
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

mkdir -p ../Results/freshRuns/processed/fig16e
python ../Codes/convergenceCheck.py pmc "../Results/freshRuns/algorithmOutput/fig16e/pmc/flowst_300.txt_300" "../Results/freshRuns/algorithmOutput/fig16e/pmc/flowst_"  ../Results/freshRuns/processed/fig16e/pmc.txt

pathtograph="../../Dataset/youtube/graph_ic.inf"
execFlowsg="../../Executables/icFlowSeeds"
for snapshots in 10 25 50 75 100 125 150 175 200 250 300
#for snapshots in 1 #to test use this line instead of above for quick result check
do
        for seeds in 1 2 5 10 25 50 75 100 125 150 175 200 #to test use this line instead of above for quick result check
        do
                echo "static greedy k="$seeds "snapshots="$snapshots
                cd ../Codes/static_greedy
                g++  -g -std=c++11  -g -o mymain *.cpp  -DMAX_NODE=1134890 -DMAX_EDGE=11950496
                ./mymain $pathtograph $seeds $snapshots sgd $outputsg   
                echo ./mymain $pathtograph $seeds $snapshots sgd $outputsg      
                $execFlowsg $pathtograph $outputsg"/seedStaticGreedyUD_SGD_R"$snapshots"_k"$seeds".txt"  $seeds 1 $numiter 3 $outputsg"/flowst_snapshots="$snapshots $outputsg"/spreadsnapshots="$snapshots".txt"
        	cd ..
	done
done
cd ../Runningscript
python ../Codes/convergenceCheck.py static $outputsgcreate"/flowst_snapshots=300_300" "../Results/freshRuns/algorithmOutput/fig16e/sg/flowst_snapshots="  ../Results/freshRuns/processed/fig16e/sg.txt
~      
