#!/bin/bash
execfilecelf="../Executables/celf"
execfileimm="../Executables/imm"
pathtograph="../Dataset/nethept/graph_wc.inf"
model="IC"
outputcelf="../Results/freshRuns/algorithmOutput/fig9d/celf"
outputimm="../Results/freshRuns/algorithmOutput/fig9d/imm"
mkdir -p ../Results/freshRuns/algorithmOutput/fig9d
mkdir -p ../Results/freshRuns/processed/fig9d
mkdir -p $outputcelf
mkdir -p $outputimm

execFlow="../Executables/wcFlow"
execFlowSeeds="../Executables/wcFlowSeeds"
flowMC=10000
#flowMC=400 #to test use this line instead of above for quick result check
echo "Running CELF" 
for mc in 1000 10000 20000
#for mc in 1 #to test use this line instead of above for quick result check
do
	echo $execfilecelf" -propModel WC -mcruns "$mc" -budget 200 -celfPlus 0 -probGraphFile "$pathtograph" -outdir "$outputcelf
	$execfilecelf -propModel $model -mcruns $mc -budget 200 -celfPlus 0 -probGraphFile $pathtograph -outdir $outputcelf
	echo $execFlow $pathtograph $outputcelf"/seeds_CELF_IC_"$mc".txt 200 1 "$flowMC" 3 "$outputcelf"/flowstat_"$mc $outputcelf"/spread_"$mc
	$execFlow $pathtograph $outputcelf"/seeds_CELF_IC_"$mc".txt" 200 1 $flowMC 3 $outputcelf"/flowstat_"$mc $outputcelf"/spread_"$mc
	cp $outputcelf"/flowstat_"$mc"_300" ../Results/freshRuns/processed/fig9d/celf$mc".txt"
done

echo "Running IMM"
eps=0.05
for seeds in 1 2 5 10 25 50 75 100 125 150 175 200
do
	echo $execfileimm" -dataset "$pathtograph" -k "$seeds" -model WC -epsilon "$eps" -output "$outputimm
	$execfileimm -dataset $pathtograph -k $seeds -model $model -epsilon $eps -output $outputimm
	echo $execFlowSeeds $pathtograph $outputimm"/seeds_IMM_IC_"$seeds"_"$eps".txt "$seeds" 1 "$flowMC" 3 "$outputimm"/flowstat_"$eps $outputimm"/spread_"$eps
	$execFlowSeeds $pathtograph $outputimm"/seeds_IMM_IC_"$seeds"_"$eps".txt" $seeds 1 $flowMC 3 $outputimm"/flowstat_"$eps $outputimm"/spread_"$eps
	cp $outputimm"/flowstat_"$eps"_300" ../Results/freshRuns/processed/fig9d/imm.txt
done

cp ../Results/Alreadypresent/fig9d/plotnew.p ../Results/freshRuns/processed/fig9d/
cd ../Results/freshRuns/processed/fig9d/
gnuplot plotnew.p
epspdf output.eps
