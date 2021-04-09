#!/bin/bash
execfilecelf="../Executables/celfRandom"
execfilecelfpp="../Executables/celfppRandom"
pathtograph="../Dataset/nethept/graph_lt.inf"
model="LT"
outputcelf="../Results/freshRuns/algorithmOutput/fig9b-13b/celf"
outputcelfpp="../Results/freshRuns/algorithmOutput/fig9b-13b/celfpp"
mkdir -p ../Results/freshRuns/algorithmOutput/fig9b-13b
for i in `seq 1 12`;
do
	mkdir -p $outputcelf"_run"$i
	mkdir -p $outputcelfpp"_run"$i
done

mc=10000
#mc=1 #to test use this line instead of above for quick result check
for i in `seq 1 12`; 
do
	echo "CELF Run: "$i 
	$execfilecelf -propModel $model -mcruns $mc -budget 50 -celfPlus 0 -probGraphFile $pathtograph -outdir $outputcelf"_run"$i
	celfout="$(tail -1 $outputcelf"_run"$i"/stats_CELF_LT_"$mc".txt")"
        
	echo "CELF++ Run: "$i 
	$execfilecelfpp -propModel $model -mcruns $mc -budget 50 -celfPlus 1 -probGraphFile $pathtograph -outdir $outputcelfpp"_run"$i
	celfppout="$(tail -1 $outputcelfpp"_run"$i"/stats_CELF++_LT_"$mc".txt")"
        echo $i" "$celfout" "$celfppout >> ../Results/freshRuns/processed/fig9b/"data.txt"
done
echo "data CELF CELF++" > ../Results/freshRuns/processed/fig9b/"time.txt"
echo "data CELF CELF++" > ../Results/freshRuns/processed/fig13b/"lookup.txt"
cut -d' ' -f1,4,9 ../Results/freshRuns/processed/fig9b/data.txt  >> ../Results/freshRuns/processed/fig9b/"time.txt"
cut -d' ' -f1,6,11 ../Results/freshRuns/processed/fig9b/data.txt  >> ../Results/freshRuns/processed/fig13b/"lookup.txt"

cp ../Results/Alreadypresent/fig9b/plotnew.p ../Results/freshRuns/processed/fig9b/
cd  ../Results/freshRuns/processed/fig9b/
gnuplot plotnew.p
epspdf output.eps

cd ../../../../Runningscript/
cp ../Results/Alreadypresent/fig13b/plotnew.p ../Results/freshRuns/processed/fig13b/
cd  ../Results/freshRuns/processed/fig13b/
gnuplot plotnew.p
epspdf output.eps
