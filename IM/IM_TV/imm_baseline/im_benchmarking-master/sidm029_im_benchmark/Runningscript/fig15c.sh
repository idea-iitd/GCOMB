#!/bin/bash
execfilecelf="../Executables/celf"
execfilecelfpp="../Executables/celfpp"
execfileeasyim="../Executables/easyimLT"
pathtograph="../Dataset/nethept/graph_lt.inf"
model="LT"
outputcelf="../Results/freshRuns/algorithmOutput/fig15c/celf"
outputcelfpp="../Results/freshRuns/algorithmOutput/fig15c/celfpp"
outputeasyim="../Results/freshRuns/algorithmOutput/fig15c/easyim"
mkdir -p ../Results/freshRuns/algorithmOutput/fig15c
mkdir -p $outputcelf
mkdir -p $outputcelfpp
mkdir -p $outputeasyim

execFlow="../Executables/ltFlow"
execFlowSeeds="../Executables/ltFlowSeeds"
flowiter=10000
for mc in 1000 2500 5000 7500 10000
#for mc in 1 #to test use this line instead of above for quick result check
do
	echo "running celf" 
	$execfilecelf -propModel $model -mcruns $mc -budget 200 -celfPlus 0 -probGraphFile $pathtograph -outdir $outputcelf
	echo $execFlow $pathtograph $outputcelf"/seeds_CELF_LT_"$mc".txt" 200 1 $flowiter 3 $outputcelf"/flowst_"$mc".txt" $outputcelf"/spread.txt"
	$execFlow $pathtograph $outputcelf"/seeds_CELF_LT_"$mc".txt" 200 1 $flowiter 3 $outputcelf"/flowst_"$mc".txt" $outputcelf"/spread.txt"
	echo "running celfpp" 
	$execfilecelfpp -propModel $model -mcruns $mc -budget 200 -celfPlus 1 -probGraphFile $pathtograph -outdir $outputcelfpp
	echo $execFlow $pathtograph $outputcelfpp"/seeds_CELF++_LT_"$mc".txt" 200 1 $flowiter 3 $outputcelfpp"/flowst_"$mc".txt" $outputcelfpp"/spread.txt"
	$execFlow $pathtograph $outputcelfpp"/seeds_CELF++_LT_"$mc".txt" 200 1 $flowiter 3 $outputcelfpp"/flowst_"$mc".txt" $outputcelfpp"/spread.txt"
done


for iter in 1 2 5 10 25 50 75 100 250 500 750 1000
#for iter in 1
do
	for seeds in 1 2 5 10 25 50 75 100 125 150 175 200
	do
		$execfileeasyim $pathtograph 2 $seeds 1 $iter 3 $outputeasyim
		
		echo $execFlowSeeds $pathtograph $outputeasyim"/seeds_seeds="$seeds"_iter="$iter"_depth=3.txt" $seeds 1 $flowiter 3 $outputeasyim"/flowstat_"$iter".txt" $outputeasyim"/spread.txt"
		$execFlowSeeds $pathtograph $outputeasyim"/seeds_seeds="$seeds"_iter="$iter"_depth=3.txt" $seeds 1 $flowiter 3 $outputeasyim"/flowstat_"$iter".txt" $outputeasyim"/spread.txt"
	done
done

mkdir -p ../Results/freshRuns/processed/fig15c
python ../Codes/convergenceCheck.py celf $outputcelf"/flowst_10000.txt_300" $outputcelf"/flowst_"  ../Results/freshRuns/processed/fig15c/celf.txt

python ../Codes/convergenceCheck.py celf $outputcelfpp"/flowst_10000.txt_300" $outputcelfpp"/flowst_"  ../Results/freshRuns/processed/fig15c/celfpp.txt

python ../Codes/convergenceCheck.py easyim $outputeasyim"/flowstat_1000.txt_300" $outputcelf"/flowstat_"  ../Results/freshRuns/processed/fig15c/easyim.txt



