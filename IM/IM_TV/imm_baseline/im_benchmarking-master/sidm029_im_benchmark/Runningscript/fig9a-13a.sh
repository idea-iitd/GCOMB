#!/bin/bash
execfilecelf="../Executables/celfRandom"
execfilecelfpp="../Executables/celfppRandom"
pathtograph="../Dataset/nethept/graph_wc.inf"
model="IC"
outputcelf="../Results/freshRuns/algorithmOutput/fig9a-13a/celf"
outputcelfpp="../Results/freshRuns/algorithmOutput/fig9a-13a/celfpp"
mkdir -p ../Results/freshRuns/algorithmOutput/fig9a-13a
for i in `seq 1 12`;
do
	mkdir -p $outputcelf"_run"$i
	mkdir -p $outputcelfpp"_run"$i
done

mkdir -p ../Results/freshRuns/processed/fig9a
mkdir -p ../Results/freshRuns/processed/fig13a
mc=10000
#mc=1 #to test use this line instead of above for quick result check
for i in `seq 1 12`; 
do
	echo "CELF Run: "$i 
	$execfilecelf -propModel $model -mcruns $mc -budget 50 -celfPlus 0 -probGraphFile $pathtograph -outdir $outputcelf"_run"$i
	celfout="$(tail -1 $outputcelf"_run"$i"/stats_CELF_IC_"$mc".txt")"	
	echo "CELF++ Run: "$i 
	$execfilecelfpp -propModel $model -mcruns $mc -budget 50 -celfPlus 1 -probGraphFile $pathtograph -outdir $outputcelfpp"_run"$i
	celfppout="$(tail -1 $outputcelfpp"_run"$i"/stats_CELF++_IC_"$mc".txt")"
	echo $i" "$celfout" "$celfppout >> ../Results/freshRuns/processed/fig9a/"data.txt"
done

echo "data CELF CELF++" > ../Results/freshRuns/processed/fig9a/"time.txt"
echo "data CELF CELF++" > ../Results/freshRuns/processed/fig13a/"lookup.txt"
cut -d' ' -f1,4,9 ../Results/freshRuns/processed/fig9a/data.txt  >> ../Results/freshRuns/processed/fig9a/"time.txt"
cut -d' ' -f1,6,11 ../Results/freshRuns/processed/fig9a/data.txt  >> ../Results/freshRuns/processed/fig13a/"lookup.txt"

cp ../Results/Alreadypresent/fig9a/plotnew.p ../Results/freshRuns/processed/fig9a/
cd  ../Results/freshRuns/processed/fig9a/
gnuplot plotnew.p
epspdf output.eps

cd ../../../../Runningscript/
cp ../Results/Alreadypresent/fig13a/plotnew.p ../Results/freshRuns/processed/fig13a/
cd  ../Results/freshRuns/processed/fig13a/
gnuplot plotnew.p
epspdf output.eps
