#!/bin/bash
execfilecelf="../Executables/celf"
execfilecelfpp="../Executables/celfpp"
execfileeasyim="../Executables/easyimLT"
execfileimm="../Executables/imm"
execfiletim="../Executables/tim"
execfilepmc="../Executables/pmc"
execfileimrank="../Executables/imrank"
execfilesg="../Executables/sg"
execfileirie="../Executables/irie"
execfilesimpath="../Executables/simpath"
execfileldag="../Executables/ldag"

pathtograph="../Dataset/youtube/graph_lt.inf"
model="LT"
ldagmodel="LT"
simpathModel="LTNew"

outputcelf="../Results/freshRuns/algorithmOutput/fig6l-7l-8l/celf"
outputcelfpp="../Results/freshRuns/algorithmOutput/fig6l-7l-8l/celfpp"
outputeasyim="../Results/freshRuns/algorithmOutput/fig6l-7l-8l/easyim"
outputimm="../Results/freshRuns/algorithmOutput/fig6l-7l-8l/imm"
outputtim="../Results/freshRuns/algorithmOutput/fig6l-7l-8l/tim"
outputpmc="../Results/freshRuns/algorithmOutput/fig6l-7l-8l/pmc"
outputimrank="../Results/freshRuns/algorithmOutput/fig6l-7l-8l/imrank"
outputsg="../Results/freshRuns/algorithmOutput/fig6l-7l-8l/sg"
outputirie="../Results/freshRuns/algorithmOutput/fig6l-7l-8l/irie"
outputsimpath="../Results/freshRuns/algorithmOutput/fig6l-7l-8l/simpath"
outputldag="../Results/freshRuns/algorithmOutput/fig6l-7l-8l/ldag"

mkdir -p ../Results/freshRuns/algorithmOutput/fig6l-7l-8l
mkdir -p ../Results/freshRuns/processed/fig6l-7l-8l
mkdir -p $outputcelf
mkdir -p $outputcelfpp
mkdir -p $outputeasyim
mkdir -p $outputimm
mkdir -p $outputtim
mkdir -p $outputpmc
mkdir -p $outputimrank
mkdir -p $outputsg
mkdir -p $outputirie
mkdir -p $outputsimpath
mkdir -p $outputldag

execFlow="../Executables/ltFlow"
execFlowSeeds="../Executables/ltFlowSeeds"

flowiter=10000
celf_mc=10000
celfpp_mc=10000
pmc_r=200
sg_r=250
easyim_iter=25
imrank_rounds=10
tim_eps=0.35
imm_eps=0.1
simpath_mc=10000
ldag_mc=10000
simpath_phase=17
ldag_phase=15

#for l in 1 2
#do
#	echo "Running IMRank"$l
#	for seeds in 1 2 5 10 25 50 75 100 125 150 175 200 
#	do
#		echo "imrank k="$seeds "rounds="$imrank_rounds
#		cd ../Codes/imrank
#		g++  -g -std=c++11  -g -o mymain limit.h general_cascade.cpp graph.cpp random.cpp degree.cpp greedy.cpp degreediscount_ic.cpp weightedegree.cpp SPM_gc.cpp SP1M_gc.cpp pmia.cpp pagerank.cpp mia.cpp IMRank.cpp main.cpp -w "-DIMRank_LOOP="$imrank_rounds "-DSET_SIZE="$seeds -DMAX_NODE=15229 -DMAX_EDGE=62752
#		cp mymain ../../Executables/imrank
#		cd ..
#		echo $execfileimrank -imrank $l "IC" $pathtograph $outputimrank
#		$execfileimrank -imrank $l "IC" $pathtograph $outputimrank
#		
#		echo $execFlowSeeds $pathtograph $outputimrank"/seedIMRank_k"$seeds"_l"$l"_LOOP"$imrank_rounds"_irDegree.txt" $seeds 1 $flowiter 3 $outputimrank"/flowstat_l="$l"_rounds="$imrank_rounds".txt" $outputimrank"/spreadl="$l"_rounds="$imrank_rounds".txt"
#		$execFlowSeeds $pathtograph $outputimrank"/seedIMRank_k"$seeds"_l"$l"_LOOP"$imrank_rounds"_irDegree.txt" $seeds 1 $flowiter 3 $outputimrank"/flowstat_l="$l"_rounds="$imrank_rounds".txt" $outputimrank"/spreadl="$l"_rounds="$imrank_rounds".txt"
#	done
#done
#cd ../Runningscript
#
#echo "Running IRIE"
##for seeds in 1 2 5 10 25 50 75 100 125 150 175 200
##do
#	echo "IRIE k="$seeds
#	cd ../Codes/irie
#	g++  -g -std=c++11  -g -o mymain *.cpp -DMAX_NODE=15229 -DMAX_EDGE=125504
#	cp mymain ../../Executables/irie
#	cd ..
#	echo $execfileirie" -irie "$seeds" 0.7 0.7 "$outputirie $pathtograph	
#	$execfileirie -irie $seeds 0.7 0.7 $outputirie $pathtograph	
#
#	echo $execFlowSeeds $pathtograph $outputirie"/seedFileseeds-"$seeds".txt" $seeds 1 $flowiter 3 $outputirie"/flowstat_alpha=0.7" $outputirie"/spread_alpha=0.7.txt"
#	$execFlowSeeds $pathtograph $outputirie"/seedFileseeds-"$seeds".txt" $seeds 1 $flowiter 3 $outputirie"/flowstat_alpha=0.7" $outputirie"/spread_alpha=0.7.txt"
#done
#cd ../Runningscript
#
#echo "Running PMC"
#echo $execfilepmc $pathtograph 200 $pmc_r $outputpmc #graph k R outdir 
#$execfilepmc $pathtograph 200 $pmc_r $outputpmc #graph k R outdir 
##[sainyam@swarm2 Runningscript]$ less ../Results/freshRuns/algorithmOutput/fig4f/pmc/seeds_PMC_IC_1.txt 
#echo $execFlow $pathtograph $outputpmc"/seeds_PMC_IC_"$pmc_r".txt" 200 1 $flowiter 3 $outputpmc"/flowstat_"$pmc_r".txt" $outputpmc"/spread_"$pmc_r".txt"
#$execFlow $pathtograph $outputpmc"/seeds_PMC_IC_"$pmc_r".txt" 200 1 $flowiter 3 $outputpmc"/flowstat_"$pmc_r".txt" $outputpmc"/spread_"$pmc_r".txt"
#
#echo "Running SG"
#for seeds in 1 2 5 10 25 50 75 100 125 150 175 200
#do
#	echo "static greedy k="$seeds
#	cd ../Codes/static_greedy
#	g++  -g -std=c++11  -g -o mymain *.cpp -DMAX_NODE=15229 -DMAX_EDGE=125504
#	cp mymain ../../Executables/sg
#	cd ..
#	echo $execfilesg $pathtograph $seeds $sg_r" sgd "$outputsg	
#	$execfilesg $pathtograph $seeds $sg_r sgd $outputsg	
#
#	echo $execFlowSeeds $pathtograph $outputsg"/seedStaticGreedyUD_SGD_R"$sg_r"_k"$seeds".txt"  $seeds 1 $flowiter 3 $outputsg"/flowstat_snapshots="$sg_r $outputsg"/spreadsnapshots="$sg_r".txt"
#	$execFlowSeeds $pathtograph $outputsg"/seedStaticGreedyUD_SGD_R"$sg_r"_k"$seeds".txt"  $seeds 1 $flowiter 3 $outputsg"/flowstat_snapshots="$sg_r $outputsg"/spreadsnapshots="$sg_r".txt"
#done
#cd ../Runningscript

echo "Running IMM"
for seeds in 1 2 5 10 25 50 75 100 125 150 175 200
do
	echo $execfileimm" -dataset "$pathtograph" -k "$seeds" -model "$model" -epsilon "$imm_eps" -output "$outputimm
	$execfileimm -dataset $pathtograph -k $seeds -model $model -epsilon $imm_eps -output $outputimm
	cat $outputimm"/stat_IMM_"$model"_"$seeds"_"$imm_eps".txt"  >> ../Results/freshRuns/processed/fig6l-7l-8l/immtime.txt
	echo $execFlowSeeds $pathtograph $outputimm"/seeds_IMM_LT_"$seeds"_"$imm_eps".txt "$seeds 1 $flowiter 3 $outputimm"/flowstat_"$imm_eps $outputimm"/spread_"$imm_eps
	$execFlowSeeds $pathtograph $outputimm"/seeds_IMM_LT_"$seeds"_"$imm_eps".txt" $seeds 1 $flowiter 3 $outputimm"/flowstat_"$imm_eps $outputimm"/spread_"$imm_eps
done
cp $outputimm"/flowstat_"$imm_eps"_300" ../Results/freshRuns/processed/fig6l-7l-8l/imm.txt

echo "Running TIM+"
for seeds in 1 2 5 10 25 50 75 100 125 150 175 200
do
	echo $execfiletim" -dataset "$pathtograph" -k "$seeds" -model "$model" -epsilon "$tim_eps" -outdir "$outputtim
	$execfiletim -dataset $pathtograph -k $seeds -model $model -epsilon $tim_eps -outdir $outputtim
	cat $outputtim"/stat_TIM_"$model"_"$seeds"_"$tim_eps".txt" >> ../Results/freshRuns/processed/fig6l-7l-8l/timtime.txt
	echo $execFlowSeeds $pathtograph $outputtim"/seeds_TIM_LT_"$seeds"_"$tim_eps".txt "$seeds 1 $flowiter 3 $outputtim"/flowstat_"$tim_eps $outputtim"/spread_"$tim_eps
	$execFlowSeeds $pathtograph $outputtim"/seeds_TIM_LT_"$seeds"_"$tim_eps".txt" $seeds 1 $flowiter 3 $outputtim"/flowstat_"$tim_eps $outputtim"/spread_"$tim_eps
done
cp $outputtim"/flowstat_"$tim_eps"_300" ../Results/freshRuns/processed/fig6l-7l-8l/tim.txt

echo "Running SIMPATH" 
echo $execfilesimpath" -propModel "$simpathmodel" -mcruns "$simpath_mc" -budget 200 -celfPlus 1 -phase "$simpathPhase" -cutoff 0.001 -topl 4 -probGraphFile "$pathtograph" -outdir "$outputsimpath
$execfilesimpath -propModel $simpathmodel -mcruns $simpath_mc -budget 200 -celfPlus 1 -phase $simpathPhase -cutoff 0.001 -topl 4 -probGraphFile $pathtograph -outdir $outputsimpath
cp $outputcelf"/stats_SIMPATH_LT_"$simpath_mc".txt" ../Results/freshRuns/processed/fig6l-7l-8l/simpathtime.txt

echo $execFlow $pathtograph $outputsimpath"/seeds_LT_"$simpath_mc".txt" 200 1 $flowiter 3 $outputsimpath"/flowstat_"$simpath_mc".txt" $outputsimpath"/spread.txt"
$execFlow $pathtograph $outputsimpath"/seeds_LT_"$simpath_mc".txt" 200 1 $flowiter 3 $outputsimpath"/flowstat_"$simpath_mc".txt" $outputsimpath"/spread.txt"
cp $outputcelf"/flowstat_"$simpath_mc".txt_300" ../Results/freshRuns/processed/fig6l-7l-8l/simpath.txt

