#!/bin/bash
execfileimm="../Executables/imm"
pathtograph="../Dataset/youtube/graph_lt.inf"
model="LT"
outputimm="../Results/freshRuns/algorithmOutput/fig12l/imm"
mkdir -p ../Results/freshRuns/algorithmOutput/fig12l
mkdir -p $outputimm

execFlowSeeds="../Executables/ltFlowSeeds"

echo "Running IMM"
eps=0.1
echo $execfileimm" -dataset "$pathtograph" -k 200 -model "$model" -epsilon "$eps" -output "$outputimm
$execfileimm -dataset $pathtograph -k 200 -model $model -epsilon $eps -output $outputimm

for flowMC in 1000 2500 5000 7500 10000 15000 20000
#for flowMC in 1000 #to test use this line instead of above for quick result check
do
	echo $execFlowSeeds $pathtograph $outputimm"/seeds_IMM_LT_200_"$eps".txt 200 1 "$flowMC" 3 "$outputimm"/flowstat_"$eps"_"$flowMC $outputimm"/spread_"$eps"_"$flowMC
	$execFlowSeeds $pathtograph $outputimm"/seeds_IMM_LT_200_"$eps".txt" 200 1 $flowMC 3 $outputimm"/flowstat_"$eps"_"$flowMC $outputimm"/spread_"$eps"_"$flowMC
done
mkdir -p ../Results/freshRuns/processed/fig12l
python ../Codes/process12.py $outputimm"/flowstat_"$eps"_" ../Results/freshRuns/processed/fig12l/imm.txt
