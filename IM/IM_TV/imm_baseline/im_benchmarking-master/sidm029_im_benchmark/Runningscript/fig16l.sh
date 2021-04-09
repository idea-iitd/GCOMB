#!/bin/bash
execfileimm="../Executables/imm"
execfiletim="../Executables/tim"
pathtograph="../Dataset/youtube/graph_wc.inf"
model="IC"
outputimm="../Results/freshRuns/algorithmOutput/fig16l/imm"
outputtim="../Results/freshRuns/algorithmOutput/fig16l/tim"
mkdir -p ../Results/freshRuns/algorithmOutput/fig16l
mkdir -p $outputimm
mkdir -p $outputtim

execFlowSeeds="../Executables/wcFlowSeeds"
flowMC=10000
#flowMC=400 #to test use this line instead of above for quick result check

num=$(awk 'BEGIN{for(i=0.05;i<=1.05;i+=0.05)print i}')
#num=1.0 #to test use this line instead of above for quick result check
echo "Running IMM"
for eps in $num 
do
	for seeds in 1 2 5 10 25 50 75 100 125 150 175 200
	do
		echo $execfileimm" -dataset "$pathtograph" -k "$seeds" -model WC -epsilon "$eps" -output "$outputimm
		$execfileimm -dataset $pathtograph -k $seeds -model $model -epsilon $eps -output $outputimm

		echo $execFlowSeeds $pathtograph $outputimm"/seeds_IMM_IC_"$seeds"_"$eps".txt "$seeds 1 $flowMC 3 $outputimm"/flowstat_"$eps $outputimm"/spread_"$eps
		$execFlowSeeds $pathtograph $outputimm"/seeds_IMM_IC_"$seeds"_"$eps".txt" $seeds 1 $flowMC 3 $outputimm"/flowstat_"$eps $outputimm"/spread_"$eps
	done
done
mkdir -p ../Results/freshRuns/processed/fig16l
python ../Codes/convergenceCheck.py imm ../Results/freshRuns/algorithmOutput/fig16l/imm/flowstat_0.05_300 ../Results/freshRuns/algorithmOutput/fig16l/imm/flowstat_ ../Results/freshRuns/processed/fig16l/imm.txt

echo "Running TIM+"
for eps in $num #to test use this line instead of above for quick result check
do
	for seeds in 1 2 5 10 25 50 75 100 125 150 175 200
	do
		echo $execfiletim" -dataset "$pathtograph" -k "$seeds" -model WC -epsilon "$eps" -outdir "$outputtim
		$execfiletim -dataset $pathtograph -k $seeds -model $model -epsilon $eps -outdir $outputtim

		echo $execFlowSeeds $pathtograph $outputtim"/seeds_TIM_IC_"$seeds"_"$eps".txt "$seeds 1 $flowMC 3 $outputtim"/flowstat_"$eps $outputtim"/spread_"$eps
		$execFlowSeeds $pathtograph $outputtim"/seeds_TIM_IC_"$seeds"_"$eps".txt" $seeds 1 $flowMC 3 $outputtim"/flowstat_"$eps $outputtim"/spread_"$eps
	done
done
python ../Codes/convergenceCheck.py tim ../Results/freshRuns/algorithmOutput/fig16l/tim/flowstat_0.05_300 ../Results/freshRuns/algorithmOutput/fig16l/tim/flowstat_ ../Results/freshRuns/processed/fig16l/tim.txt
