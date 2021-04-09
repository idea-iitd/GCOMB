#!/bin/bash
execfiletim="./tim"
pathtograph="../../Dataset/hepph/graph_wc.inf"
model="IC"
outputtim="tim_op_test"

#execFlowSeeds="../Executables/wcFlowSeeds"
#flowMC=10000
#num=$(awk 'BEGIN{for(i=0.05;i<=1.05;i+=0.05)print i}')
num=0.05
for i in {1..20}
do
	echo "Running TIM+, Iteration: "$i
	mkdir -p $outputtim$i
	for eps in $num #to test use this line instead of above for quick result check
	do
		#for seeds in 1 2 5 10 25 50 75 100 125 150 175 200
		for seeds in 200
		do
			echo $execfiletim" -dataset "$pathtograph" -k "$seeds" -model WC -epsilon "$eps" -outdir "$outputtim$i
			$execfiletim -dataset $pathtograph -k $seeds -model $model -epsilon $eps -outdir $outputtim$i

			#echo $execFlowSeeds $pathtograph $outputtim"/seeds_TIM_IC_"$seeds"_"$eps".txt "$seeds 1 $flowMC 3 $outputtim"/flowstat_"$eps $outputtim"/spread_"$eps
			#$execFlowSeeds $pathtograph $outputtim"/seeds_TIM_IC_"$seeds"_"$eps".txt" $seeds 1 $flowMC 3 $outputtim"/flowstat_"$eps $outputtim"/spread_"$eps
		done
	done

done

#python ../Codes/convergenceCheck.py tim ../Results/freshRuns/algorithmOutput/fig4i/tim$1"/flowstat_0.05_300" ../Results/freshRuns/algorithmOutput/fig4i/tim$1"/flowstat_" ../Results/freshRuns/processed/fig4i/tim$1".txt"

