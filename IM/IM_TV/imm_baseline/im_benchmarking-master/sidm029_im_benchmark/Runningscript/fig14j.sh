#!/bin/bash
execfileimm="../Executables/imm"
pathtograph="../Dataset/hepph/graph_ic.inf"
model="IC"
outputimm="../Results/freshRuns/algorithmOutput/fig14j/imm"
mkdir -p ../Results/freshRuns/algorithmOutput/fig14j
mkdir -p $outputimm

execFlowSeeds="../Executables/icFlowSeeds"
flowMC=10000
#flowMC=400 #to test use this line instead of above for quick result check

num=$(awk 'BEGIN{for(i=0.05;i<=1.05;i+=0.05)print i}')
#num=1.0 #to test use this line instead of above for quick result check

echo "Running IMM"
for eps in $num 
do
	for seeds in 1 50 100 150 200
	do
		echo $execfileimm" -dataset "$pathtograph" -k "$seeds" -model "$model" -epsilon "$eps" -output "$outputimm
		$execfileimm -dataset $pathtograph -k $seeds -model $model -epsilon $eps -output $outputimm

		echo $execFlowSeeds $pathtograph $outputimm"/seeds_IMM_IC_"$seeds"_"$eps".txt "$seeds 1 $flowMC 3 $outputimm"/flowstat_"$eps $outputimm"/spread_"$eps
		$execFlowSeeds $pathtograph $outputimm"/seeds_IMM_IC_"$seeds"_"$eps".txt" $seeds 1 $flowMC 3 $outputimm"/flowstat_"$eps $outputimm"/spread_"$eps
	done
done
mkdir -p ../Results/freshRuns/processed/fig14j
python ../Codes/process5.py imm $outputimm"/flowstat_" ../Results/freshRuns/processed/fig14j/output.txt
