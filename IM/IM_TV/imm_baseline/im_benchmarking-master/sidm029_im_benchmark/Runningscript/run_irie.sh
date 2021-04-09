execfileirie="../Executables/irie"
pathtograph="../Dataset/nethept/graph_ic.inf"
model="IC"
outputirie="../Results/freshRuns/algorithmOutput/fig6a-7a-8a/irie"
execFlowSeeds="../Executables/icFlowSeeds"
flowiter=10000
mkdir -p ../Results/freshRuns/algorithmOutput/fig6a-7a-8a
mkdir -p $outputirie

echo "Running IRIE"
for seeds in 1 2 5 10 25 50 75 100 125 150 175 200
do
	echo "IRIE k="$seeds
	cd ../Codes/irie
	g++  -g -std=c++11  -g -o mymain *.cpp -DMAX_NODE=15229 -DMAX_EDGE=125504
	cp mymain ../../Executables/irie
	cd ..
	echo $execfileirie" -irie "$seeds" 0.7 0.7 "$outputirie $pathtograph	
	$execfileirie -irie $seeds 0.7 0.7 $outputirie $pathtograph	
	cat $outputirie"/statFileseeds-"$seeds".txt" >> ../Results/freshRuns/processed/fig6a-7a-8a/irietime.txt

	echo $execFlowSeeds $pathtograph $outputirie"/seedFileseeds-"$seeds".txt" $seeds 1 $flowiter 3 $outputirie"/flowstat_alpha=0.7" $outputirie"/spread_alpha=0.7.txt"
	$execFlowSeeds $pathtograph $outputirie"/seedFileseeds-"$seeds".txt" $seeds 1 $flowiter 3 $outputirie"/flowstat_alpha=0.7" $outputirie"/spread_alpha=0.7.txt"
done
cp $outputirie"/flowstat_alpha=0.7_300" ../Results/freshRuns/processed/fig6a-7a-8a/irie.txt
cd ../Runningscript

