#!/bin/bash
execfiletim="../Executables/tim"
pathtograph="../Dataset/hepph/graph_lt.inf"
model="LT"
outputtim="../Results/freshRuns/algorithmOutput/fig14i/tim"
mkdir -p ../Results/freshRuns/algorithmOutput/fig14i
mkdir -p $outputtim

execFlowSeeds="../Executables/ltFlowSeeds"
flowMC=10000
#flowMC=400 #to test use this line instead of above for quick result check

num=$(awk 'BEGIN{for(i=0.05;i<=1.05;i+=0.05)print i}')
#num=1.0 #to test use this line instead of above for quick result check

echo "Running TIM+"
for eps in $num #to test use this line instead of above for quick result check
do
	for seeds in 1 50 100 150 200
	do
		echo $execfiletim" -dataset "$pathtograph" -k "$seeds" -model "$model" -epsilon "$eps" -outdir "$outputtim
		$execfiletim -dataset $pathtograph -k $seeds -model $model -epsilon $eps -outdir $outputtim

		echo $execFlowSeeds $pathtograph $outputtim"/seeds_TIM_LT_"$seeds"_"$eps".txt "$seeds 1 $flowMC 3 $outputtim"/flowstat_"$eps $outputtim"/spread_"$eps
		$execFlowSeeds $pathtograph $outputtim"/seeds_TIM_LT_"$seeds"_"$eps".txt" $seeds 1 $flowMC 3 $outputtim"/flowstat_"$eps $outputtim"/spread_"$eps
	done
done
mkdir -p ../Results/freshRuns/processed/fig14i
python ../Codes/process5.py imm $outputimm"/flowstat_" ../Results/freshRuns/processed/fig14i/output.txt
