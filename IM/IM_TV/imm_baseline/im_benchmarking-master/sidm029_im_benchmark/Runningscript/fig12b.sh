#!/bin/bash
execfileimm="../Executables/imm"
pathtograph="../Dataset/nethept/graph_wc.inf"
model="IC"
outputimm="../Results/freshRuns/algorithmOutput/fig12b/imm"
mkdir -p ../Results/freshRuns/algorithmOutput/fig12b
mkdir -p $outputimm

execFlowSeeds="../Executables/wcFlowSeeds"

echo "Running IMM"
eps=0.1
echo $execfileimm" -dataset "$pathtograph" -k 200 -model WC -epsilon "$eps" -output "$outputimm
$execfileimm -dataset $pathtograph -k 200 -model $model -epsilon $eps -output $outputimm

for flowMC in 1000 2500 5000 7500 10000 15000 20000
#for flowMC in 1000 #to test use this line instead of above for quick result check
do
	echo $execFlowSeeds $pathtograph $outputimm"/seeds_IMM_IC_200_"$eps".txt 200 1 "$flowMC" 3 "$outputimm"/flowstat_"$eps"_"$flowMC $outputimm"/spread_"$eps"_"$flowMC
	$execFlowSeeds $pathtograph $outputimm"/seeds_IMM_IC_200_"$eps".txt" 200 1 $flowMC 3 $outputimm"/flowstat_"$eps"_"$flowMC $outputimm"/spread_"$eps"_"$flowMC
done
mkdir -p ../Results/freshRuns/processed/fig12b
python ../Codes/process12.py $outputimm"/flowstat_"$eps"_" ../Results/freshRuns/processed/fig12b/imm.txt
cp ../Results/Alreadypresent/fig12b/plotnew.p ../Results/freshRuns/processed/fig12b/
cd ../Results/freshRuns/processed/fig12b/
gnuplot plotnew.p
epspdf output.eps
