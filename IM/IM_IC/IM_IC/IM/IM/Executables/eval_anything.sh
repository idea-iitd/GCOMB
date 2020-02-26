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
train_or_test=$7

echo $dataset
echo $model
outputimm="../RESULTS/"$dataset"/"$model"/"$train_or_test
echo $outputimm
execFlow="./"${model,,}"Flow"
execFlowSeeds="./"${model,,}"FlowSeeds"
echo "hree"
mkdir -p $outputimm
flowiter=10000
reward_file_suffix=$6
echo $reward_file_suffix
reward_file_name_only=$outputimm"_"$reward_file_suffix
echo "reward file "$reward_file_name_only


imm_eps=0.5
#simpath_mc=10000
#ldag_mc=10000
#simpath_phase=17
echo "rm doing"

#ldag_phase=15
echo $outputimm"/spread_"$reward_file_suffix

rm -f  $outputimm"/spread_"$reward_file_suffix
rm -f $outputimm"/spread_"$reward_file_suffix"_"$budget
rm -f  $reward_file_name_only
echo "rm done"
#cd ../Runningscript
#10 20 50 100 150 200
for seeds in 20
do
    cat $seed_file
	echo $execFlowSeeds $pathtograph $seed_file $budget 1 $flowiter 3 $outputimm"/flowstat_"$imm_eps $outputimm"/spread_"$reward_file_suffix

    #echo "spread_avg" $avg_spread
	$execFlowSeeds $pathtograph $seed_file $budget 1 $flowiter 3 $outputimm"/flowstat_"$imm_eps $outputimm"/spread_"$reward_file_suffix

    awk_command="awk '{ total += \$1; count++ } END { print total/count }' $outputimm""/spread_"$reward_file_suffix"_"$budget
#   echo $awk_command
    avg_spread=$(eval $awk_command)
    echo $avg_spread
    echo $avg_spread > $reward_file_name_only


done