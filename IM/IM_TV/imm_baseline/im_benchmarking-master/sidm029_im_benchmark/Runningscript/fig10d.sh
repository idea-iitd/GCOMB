#!/bin/bash
execfileimm="../Executables/imm"
execfiletim="../Executables/tim"
pathtograph="../Dataset/dblp/graph_wc.inf"
model="IC"
outputimm="../Results/freshRuns/algorithmOutput/fig10d/imm"
outputtim="../Results/freshRuns/algorithmOutput/fig10d/tim"
mkdir -p ../Results/freshRuns/algorithmOutput/fig10d
mkdir -p $outputimm
mkdir -p $outputtim
mkdir -p ../Results/freshRuns/processed/fig10d

execFlowSeeds="../Executables/wcFlowSeeds"
flowMC=10000
#flowMC=400 #to test use this line instead of above for quick result check

num=$(awk 'BEGIN{for(i=0.05;i<=1.05;i+=0.05)print i}')
#num=1.0 #to test use this line instead of above for quick result check
echo "Running IMM"
for eps in $num 
do
	#for seeds in 1 2 5 10 25 50 75 100 125 150 175 200
	for seeds in 200
	do
		echo $execfileimm" -dataset "$pathtograph" -k "$seeds" -model WC -epsilon "$eps" -output "$outputimm
		$execfileimm -dataset $pathtograph -k $seeds -model $model -epsilon $eps -output $outputimm
		algoout=$(cat $outputimm"/stat_IMM_IC_200_"$eps".txt")
		echo $execFlowSeeds $pathtograph $outputimm"/seeds_IMM_IC_"$seeds"_"$eps".txt "$seeds 1 $flowMC 3 $outputimm"/flowstat_"$eps $outputimm"/spread_"$eps
		$execFlowSeeds $pathtograph $outputimm"/seeds_IMM_IC_"$seeds"_"$eps".txt" $seeds 1 $flowMC 3 $outputimm"/flowstat_"$eps $outputimm"/spread_"$eps
		flowout=$(cat $outputimm"/flowstat_"$eps"_300")
                echo $eps" "$flowout" "$algoout >> ../Results/freshRuns/processed/fig10d/imm.txt
	done
done

echo "Running TIM+"
for eps in $num #to test use this line instead of above for quick result check
do
	#for seeds in 1 2 5 10 25 50 75 100 125 150 175 200
	for seeds in 200
	do
		echo $execfiletim" -dataset "$pathtograph" -k "$seeds" -model WC -epsilon "$eps" -outdir "$outputtim
		$execfiletim -dataset $pathtograph -k $seeds -model $model -epsilon $eps -outdir $outputtim
		algoout=$(cat $outputtim"/stat_TIM_IC_200_"$eps".txt")

		echo $execFlowSeeds $pathtograph $outputtim"/seeds_TIM_IC_"$seeds"_"$eps".txt "$seeds 1 $flowMC 3 $outputtim"/flowstat_"$eps $outputtim"/spread_"$eps
		$execFlowSeeds $pathtograph $outputtim"/seeds_TIM_IC_"$seeds"_"$eps".txt" $seeds 1 $flowMC 3 $outputtim"/flowstat_"$eps $outputtim"/spread_"$eps
		flowout=$(cat $outputtim"/flowstat_"$eps"_300")
                echo $eps" "$flowout" "$algoout >> ../Results/freshRuns/processed/fig10d/tim.txt
	done
done
cp ../Results/Alreadypresent/fig10d/plotnew.p ../Results/freshRuns/processed/fig10d/
cd ../Results/freshRuns/processed/fig10d/
gnuplot plotnew.p
epspdf output.eps
