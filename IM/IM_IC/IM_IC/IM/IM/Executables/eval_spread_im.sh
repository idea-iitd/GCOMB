#!/bin/bash
dataset=$1
model=$2
budget=$3
pathtograph=$4 #"../GraphSAGE-master/_large_datasets_eval/"$dataset"/WC/test/edges.txt"
echo $pathtograph
seed_file=$5 #"../GraphSAGE-master/_large_datasets_eval/"$dataset"/WC/test/large_graph_ic_imm_sol_eps0.5"
sol_file_name_only="temp.txt"
echo $seed_file
echo $sol_file_name_only
echo "seeds of file are"
head -n 5 $pathtograph
cat $seed_file
reward_file_name_only=$6
echo reward_file_name_only


echo $dataset
echo $model
outputimm="../TEMP/"$dataset"/"$model
echo $outputimm
execFlow="./"${model,,}"Flow"
execFlowSeeds="./"${model,,}"FlowSeeds"
echo "hree"
mkdir -p $outputimm
flowiter=1000

imm_eps=0.5
#simpath_mc=10000
#ldag_mc=10000
#simpath_phase=17
#ldag_phase=15

rm -f  $outputimm"/spread_"$sol_file_name_only
rm -f $outputimm"/spread_"$sol_file_name_only"_"$budget
rm -f  $reward_file_name_only

#cd ../Runningscript
#10 20 50 100 150 200
for seeds in 20
do
    cat $seed_file
	echo $execFlowSeeds $pathtograph $seed_file $budget 1 $flowiter 3 $outputimm"/flowstat_"$imm_eps $outputimm"/spread_"$sol_file_name_only

  #  echo "spread_avg" $avg_spread
	$execFlowSeeds $pathtograph $seed_file $budget 1 $flowiter 3 $outputimm"/flowstat_"$imm_eps $outputimm"/spread_"$sol_file_name_only

    awk_command="awk '{ total += \$1; count++ } END { print total/count }' $outputimm""/spread_"$sol_file_name_only"_"$budget
    echo $awk_command
    avg_spread=$(eval $awk_command)
    echo $avg_spread
    echo $avg_spread > $reward_file_name_only


done






