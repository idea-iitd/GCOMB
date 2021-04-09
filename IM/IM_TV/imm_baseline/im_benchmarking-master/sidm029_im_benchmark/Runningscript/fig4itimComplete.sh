#!/bin/bash
execfileimm="../Executables/imm"
execfiletim="../Executables/tim"
pathtograph="../Dataset/hepph/WC/graph_ic.inf"
model="IC"
outputtim="../Results/freshRuns/algorithmOutput/fig4i/tim"
mkdir -p ../Results/freshRuns/algorithmOutput/fig4i
mkdir -p $outputtim

execFlowSeeds="../Executables/wcFlowSeeds"$1
flowMC=10000
#flowMC=400 #to test use this line instead of above for quick result check

num=$(awk 'BEGIN{for(i=0.05;i<=1.05;i+=0.05)print i}')
#num=1.0 #to test use this line instead of above for quick result check

echo "Running TIM+"
for eps in $num #to test use this line instead of above for quick result check
do
	for seeds in 1 2 5 10 25 50 75 100 125 150 175 200
	do
		echo $execfiletim" -dataset "$pathtograph" -k "$seeds" -model WC -epsilon "$eps" -outdir "$outputtim
		$execfiletim -dataset $pathtograph -k $seeds -model $model -epsilon $eps -outdir $outputtim

		echo $execFlowSeeds $pathtograph $outputtim"/seeds_TIM_IC_"$seeds"_"$eps".txt "$seeds 1 $flowMC 3 $outputtim"/flowstat_"$eps $outputtim"/spread_"$eps
		$execFlowSeeds $pathtograph $outputtim"/seeds_TIM_IC_"$seeds"_"$eps".txt" $seeds 1 $flowMC 3 $outputtim"/flowstat_"$1$eps $outputtim"/spread_"$1$eps
	done
done
python ../Codes/convergenceCheck.py tim ../Results/freshRuns/algorithmOutput/fig4i/tim"/flowstat_"$1"0.05_300" ../Results/freshRuns/algorithmOutput/fig4i/tim"/flowstat_"$1 ../Results/freshRuns/processed/fig4i/tim$1".txt"

