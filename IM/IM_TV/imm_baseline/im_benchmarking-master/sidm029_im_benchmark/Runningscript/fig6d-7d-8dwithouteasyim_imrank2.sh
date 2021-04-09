#!/bin/bash
execfilecelf="../Executables/celf"
execfilecelfpp="../Executables/celfpp"
execfileeasyim="../Executables/easyimIC"
execfileimm="../Executables/imm"
execfiletim="../Executables/tim"
execfilepmc="../Executables/pmc"
execfileimrank="../Executables/imrank"
execfilesg="../Executables/sg"
execfileirie="../Executables/irie"
#execfilesimpath="../Executables/simpath"
#execfileldag="../Executables/ldag"

pathtograph="../Dataset/youtube/graph_ic.inf"
model="IC"
#ldagmodel="LT"
#simpathModel="LTNew"

outputcelf="../Results/freshRuns/algorithmOutput/fig6d-7d-8d/celf"
outputcelfpp="../Results/freshRuns/algorithmOutput/fig6d-7d-8d/celfpp"
outputeasyim="../Results/freshRuns/algorithmOutput/fig6d-7d-8d/easyim"
outputimm="../Results/freshRuns/algorithmOutput/fig6d-7d-8d/imm"
outputtim="../Results/freshRuns/algorithmOutput/fig6d-7d-8d/tim"
outputpmc="../Results/freshRuns/algorithmOutput/fig6d-7d-8d/pmc"
outputimrank="../Results/freshRuns/algorithmOutput/fig6d-7d-8d/imrank"
outputsg="../Results/freshRuns/algorithmOutput/fig6d-7d-8d/sg"
outputirie="../Results/freshRuns/algorithmOutput/fig6d-7d-8d/irie"
#outputsimpath="../Results/freshRuns/algorithmOutput/fig6d-7d-8d/simpath"
#outputldag="../Results/freshRuns/algorithmOutput/fig6d-7d-8d/ldag"

mkdir -p ../Results/freshRuns/processed/fig6d-7d-8d
mkdir -p ../Results/freshRuns/algorithmOutput/fig6d-7d-8d
mkdir -p $outputcelf
mkdir -p $outputcelfpp
mkdir -p $outputeasyim
mkdir -p $outputimm
mkdir -p $outputtim
mkdir -p $outputpmc
mkdir -p $outputimrank
mkdir -p $outputsg
mkdir -p $outputirie
#mkdir -p $outputsimpath
#mkdir -p $outputldag

execFlow="../Executables/icFlow"
execFlowSeeds="../Executables/icFlowSeeds"

flowiter=10000
celf_mc=10000
celfpp_mc=7500
pmc_r=200
sg_r=250
easyim_iter=50
imrank_rounds=10
tim_eps=0.05
imm_eps=0.05
#simpath_mc=10000
#ldag_mc=10000
#simpath_phase=17
#ldag_phase=15

#for l in 1 2
for l in 1
do
	echo "Running IMRank"$l
	for seeds in 1 2 5 10 25 50 75 100 125 150 175 200 
	do
		echo "imrank k="$seeds "rounds="$imrank_rounds
		cd ../Codes/imrank
		g++  -g -std=c++11  -g -o mymain limit.h general_cascade.cpp graph.cpp random.cpp degree.cpp greedy.cpp degreediscount_ic.cpp weightedegree.cpp SPM_gc.cpp SP1M_gc.cpp pmia.cpp pagerank.cpp mia.cpp IMRank.cpp main.cpp -w "-DIMRank_LOOP="$imrank_rounds "-DSET_SIZE="$seeds -DMAX_NODE=1134890 -DMAX_EDGE=5975248
		cp mymain ../../Executables/imrank
		cd ..
		echo $execfileimrank -imrank $l "IC" $pathtograph $outputimrank
		$execfileimrank -imrank $l "IC" $pathtograph $outputimrank
		cat $outputimrank"/statIMRank_k"$seeds"_l"$l"_LOOP10_irDegree.txt" >> ../Results/freshRuns/processed/fig6d-7d-8d/imrank$l"time.txt"	
		echo $execFlowSeeds $pathtograph $outputimrank"/seedIMRank_k"$seeds"_l"$l"_LOOP"$imrank_rounds"_irDegree.txt" $seeds 1 $flowiter 3 $outputimrank"/flowstat_l="$l"_rounds="$imrank_rounds".txt" $outputimrank"/spreadl="$l"_rounds="$imrank_rounds".txt"
		$execFlowSeeds $pathtograph $outputimrank"/seedIMRank_k"$seeds"_l"$l"_LOOP"$imrank_rounds"_irDegree.txt" $seeds 1 $flowiter 3 $outputimrank"/flowstat_l="$l"_rounds="$imrank_rounds".txt" $outputimrank"/spreadl="$l"_rounds="$imrank_rounds".txt"
	done
	cp $outputimrank"/flowstat_l="$l"_rounds="$imrank_rounds".txt_300" ../Results/freshRuns/processed/fig6d-7d-8d/imrank$l".txt"
done
cd ../Runningscript

echo "Running IRIE"
for seeds in 1 2 5 10 25 50 75 100 125 150 175 200
do
	echo "IRIE k="$seeds
	cd ../Codes/irie
	g++  -g -std=c++11  -g -o mymain *.cpp -DMAX_NODE=1134890 -DMAX_EDGE=11950496
	cp mymain ../../Executables/irie
	cd ..
	echo $execfileirie" -irie "$seeds" 0.7 0.7 "$outputirie $pathtograph	
	$execfileirie -irie $seeds 0.7 0.7 $outputirie $pathtograph	
	cat $outputirie"/statFileseeds-"$seeds".txt" >> ../Results/freshRuns/processed/fig6d-7d-8d/irietime.txt
	echo $execFlowSeeds $pathtograph $outputirie"/seedFileseeds-"$seeds".txt" $seeds 1 $flowiter 3 $outputirie"/flowstat_alpha=0.7" $outputirie"/spread_alpha=0.7.txt"
	$execFlowSeeds $pathtograph $outputirie"/seedFileseeds-"$seeds".txt" $seeds 1 $flowiter 3 $outputirie"/flowstat_alpha=0.7" $outputirie"/spread_alpha=0.7.txt"
done
cp $outputirie"/flowstat_alpha=0.7_300" ../Results/freshRuns/processed/fig6d-7d-8d/irie.txt
cd ../Runningscript

echo "Running PMC"
echo $execfilepmc $pathtograph 200 $pmc_r $outputpmc #graph k R outdir 
$execfilepmc $pathtograph 200 $pmc_r $outputpmc #graph k R outdir 
cp $outputpmc"/stats_PMC_IC_"$pmc_r".txt" ../Results/freshRuns/processed/fig6d-7d-8d/pmctime.txt

#[sainyam@swarm2 Runningscript]$ less ../Results/freshRuns/algorithmOutput/fig4f/pmc/seeds_PMC_IC_1.txt 
echo $execFlow $pathtograph $outputpmc"/seeds_PMC_IC_"$pmc_r".txt" 200 1 $flowiter 3 $outputpmc"/flowstat_"$pmc_r".txt" $outputpmc"/spread_"$pmc_r".txt"
$execFlow $pathtograph $outputpmc"/seeds_PMC_IC_"$pmc_r".txt" 200 1 $flowiter 3 $outputpmc"/flowstat_"$pmc_r".txt" $outputpmc"/spread_"$pmc_r".txt"
cp $outputpmc"/flowstat_"$pmc_r".txt_300" ../Results/freshRuns/processed/fig6d-7d-8d/pmc.txt

echo "Running SG"
for seeds in 1 2 5 10 25 50 75 100 125 150 175 200
do
	echo "static greedy k="$seeds
	cd ../Codes/static_greedy
	g++  -g -std=c++11  -g -o mymain *.cpp -DMAX_NODE=1134890 -DMAX_EDGE=11950496
	cp mymain ../../Executables/sg
	cd ..
	echo $execfilesg $pathtograph $seeds $sg_r" sgd "$outputsg	
	$execfilesg $pathtograph $seeds $sg_r sgd $outputsg	
	cat $outputsg"/statStaticGreedyUD_SGD_R"$sg_r"_k"$seeds".txt" >> ../Results/freshRuns/processed/fig6d-7d-8d/sgtime.txt

	echo $execFlowSeeds $pathtograph $outputsg"/seedStaticGreedyUD_SGD_R"$sg_r"_k"$seeds".txt"  $seeds 1 $flowiter 3 $outputsg"/flowstat_snapshots="$sg_r $outputsg"/spreadsnapshots="$sg_r".txt"
	$execFlowSeeds $pathtograph $outputsg"/seedStaticGreedyUD_SGD_R"$sg_r"_k"$seeds".txt"  $seeds 1 $flowiter 3 $outputsg"/flowstat_snapshots="$sg_r $outputsg"/spreadsnapshots="$sg_r".txt"
done
cp $outputsg"/flowstat_snapshots="$sg_r"_300"  ../Results/freshRuns/processed/fig6d-7d-8d/sg.txt

cd ../Runningscript

echo "Running IMM"
for seeds in 1 2 5 10 25 50 75 100 125 150 175 200
do
	echo $execfileimm" -dataset "$pathtograph" -k "$seeds" -model "$model" -epsilon "$imm_eps" -output "$outputimm
	$execfileimm -dataset $pathtograph -k $seeds -model $model -epsilon $imm_eps -output $outputimm
	cat $outputimm"/stat_IMM_"$model"_"$seeds"_"$imm_eps".txt"  >> ../Results/freshRuns/processed/fig6d-7d-8d/immtime.txt
	echo $execFlowSeeds $pathtograph $outputimm"/seeds_IMM_IC_"$seeds"_"$imm_eps".txt "$seeds 1 $flowiter 3 $outputimm"/flowstat_"$imm_eps $outputimm"/spread_"$imm_eps
	$execFlowSeeds $pathtograph $outputimm"/seeds_IMM_IC_"$seeds"_"$imm_eps".txt" $seeds 1 $flowiter 3 $outputimm"/flowstat_"$imm_eps $outputimm"/spread_"$imm_eps
done
cp $outputimm"/flowstat_"$imm_eps"_300" ../Results/freshRuns/processed/fig6d-7d-8d/imm.txt

echo "Running TIM+"
for seeds in 1 2 5 10 25 50 75 100 125 150 175 200
do
	echo $execfiletim" -dataset "$pathtograph" -k "$seeds" -model "$model" -epsilon "$tim_eps" -outdir "$outputtim
	$execfiletim -dataset $pathtograph -k $seeds -model $model -epsilon $tim_eps -outdir $outputtim
	cat $outputtim"/stat_TIM_"$model"_"$seeds"_"$tim_eps".txt" >> ../Results/freshRuns/processed/fig6d-7d-8d/timtime.txt
	echo $execFlowSeeds $pathtograph $outputtim"/seeds_TIM_IC_"$seeds"_"$tim_eps".txt "$seeds 1 $flowiter 3 $outputtim"/flowstat_"$tim_eps $outputtim"/spread_"$tim_eps
	$execFlowSeeds $pathtograph $outputtim"/seeds_TIM_IC_"$seeds"_"$tim_eps".txt" $seeds 1 $flowiter 3 $outputtim"/flowstat_"$tim_eps $outputtim"/spread_"$tim_eps
done
cp $outputtim"/flowstat_"$tim_eps"_300" ../Results/freshRuns/processed/fig6d-7d-8d/tim.txt

#echo "Running SIMPATH" 
#echo $execfilesimpath" -propModel "$simpathmodel" -mcruns "$simpath_mc" -budget 200 -celfPlus 1 -phase "$simpathPhase" -cutoff 0.001 -topl 4 -probGraphFile "$pathtograph" -outdir "$outputsimpath
#$execfilesimpath -propModel $simpathmodel -mcruns $simpath_mc -budget 200 -celfPlus 1 -phase $simpathPhase -cutoff 0.001 -topl 4 -probGraphFile $pathtograph -outdir $outputsimpath

#echo $execFlow $pathtograph $outputsimpath"/seeds_IC_"$simpath_mc".txt" 200 1 $flowiter 3 $outputsimpath"/flowstat_"$simpath_mc".txt" $outputsimpath"/spread.txt"
#$execFlow $pathtograph $outputsimpath"/seeds_IC_"$simpath_mc".txt" 200 1 $flowiter 3 $outputsimpath"/flowstat_"$simpath_mc".txt" $outputsimpath"/spread.txt"

#echo "Running LDAG" 
#echo $execfileldag" -propModel "$ldagmodel" -mcruns "$ldag_mc" -budget 200 -phase "$ldagPhase" -tol_ldag 0.003125 -probGraphFile "$pathtograph" -outdir "$outputldag
#$execfileldag -propModel $ldagmodel -mcruns $ldag_mc -budget 200 -phase $ldagPhase -tol_ldag 0.003125 -probGraphFile $pathtograph -outdir $outputldag

#echo $execFlow $pathtograph $outputldag"/seeds_IC_"$ldag_mc".txt" 200 1 $flowiter 3 $outputldag"/flowstat_"$ldag_mc".txt" $outputldag"/spread.txt"
#$execFlow $pathtograph $outputldag"/seeds_IC_"$ldag_mc".txt" 200 1 $flowiter 3 $outputldag"/flowstat_"$ldag_mc".txt" $outputldag"/spread.txt"
