#!/bin/bash

execfilepmc="../Executables/pmc"
execfileeasyimic="../Executables/easyimIC"
execfileeasyimwc="../Executables/easyimWC"
execfileeasyilt="../Executables/easyimLT"
execfileimm="../Executables/imm"
execfiletim="../Executables/tim"





output_ic_live_easyim="../Results/freshRuns/algorithmOutput/table3/easyim_ic_livejournal"
output_ic_live_pmc="../Results/freshRuns/algorithmOutput/table3/pmc_ic_livejournal"

output_wc_live_easyim="../Results/freshRuns/algorithmOutput/table3/easyim_wc_livejournal"
output_wc_live_imm="../Results/freshRuns/algorithmOutput/table3/imm_wc_livejournal"
output_wc_live_pmc="../Results/freshRuns/algorithmOutput/table3/pmc_wc_livejournal"

output_lt_live_imm="../Results/freshRuns/algorithmOutput/table3/imm_lt_livejournal"
output_lt_live_easyim="../Results/freshRuns/algorithmOutput/table3/easyim_lt_livejournal"


output_ic_orkut_easyim="../Results/freshRuns/algorithmOutput/table3/easyim_ic_orkut"
output_ic_orkut_pmc="../Results/freshRuns/algorithmOutput/table3/pmc_ic_orkut"

output_wc_orkut_easyim="../Results/freshRuns/algorithmOutput/table3/easyim_wc_orkut"
output_wc_orkut_imm="../Results/freshRuns/algorithmOutput/table3/imm_wc_orkut"
output_wc_orkut_pmc="../Results/freshRuns/algorithmOutput/table3/pmc_wc_orkut"

output_lt_orkut_imm="../Results/freshRuns/algorithmOutput/table3/imm_lt_orkut"
output_lt_orkut_easyim="../Results/freshRuns/algorithmOutput/table3/easyim_lt_orkut"


output_ic_twitter_easyim="../Results/freshRuns/algorithmOutput/table3/easyim_ic_twitter"
output_ic_twitter_pmc="../Results/freshRuns/algorithmOutput/table3/pmc_ic_twitter"

output_wc_twitter_easyim="../Results/freshRuns/algorithmOutput/table3/easyim_wc_twitter"
output_wc_twitter_imm="../Results/freshRuns/algorithmOutput/table3/imm_wc_twitter"
output_wc_twitter_pmc="../Results/freshRuns/algorithmOutput/table3/pmc_wc_twitter"

output_lt_twitter_imm="../Results/freshRuns/algorithmOutput/table3/imm_lt_twitter"
output_lt_twitter_easyim="../Results/freshRuns/algorithmOutput/table3/easyim_lt_twitter"


output_ic_friend_easyim="../Results/freshRuns/algorithmOutput/table3/easyim_ic_friend"
output_ic_friend_pmc="../Results/freshRuns/algorithmOutput/table3/pmc_ic_friend"

output_wc_friend_easyim="../Results/freshRuns/algorithmOutput/table3/easyim_wc_friend"
output_wc_friend_imm="../Results/freshRuns/algorithmOutput/table3/imm_wc_friend"
output_wc_friend_pmc="../Results/freshRuns/algorithmOutput/table3/pmc_wc_friend"

output_lt_friend_imm="../Results/freshRuns/algorithmOutput/table3/imm_lt_friend"
output_lt_friend_easyim="../Results/freshRuns/algorithmOutput/table3/easyim_lt_friend"

mkdir -p ../Results/freshRuns/algorithmOutput/table3

mkdir -p $output_ic_live_easyim
mkdir -p $output_ic_live_pmc
mkdir -p $output_wc_live_easyim
mkdir -p $output_wc_live_imm
mkdir -p $output_wc_live_pmc
mkdir -p $output_lt_live_easyim
mkdir -p $output_lt_live_imm

mkdir -p $output_ic_orkut_easyim
mkdir -p $output_ic_orkut_pmc
mkdir -p $output_wc_orkut_easyim
mkdir -p $output_wc_orkut_imm
mkdir -p $output_wc_orkut_pmc
mkdir -p $output_lt_orkut_easyim
mkdir -p $output_lt_orkut_imm


mkdir -p $output_ic_twitter_easyim
mkdir -p $output_ic_twitter_pmc
mkdir -p $output_wc_twitter_easyim
mkdir -p $output_wc_twitter_imm
mkdir -p $output_wc_twitter_pmc
mkdir -p $output_lt_twitter_easyim
mkdir -p $output_lt_twitter_imm

mkdir -p $output_ic_friend_easyim
mkdir -p $output_ic_friend_pmc
mkdir -p $output_wc_friend_easyim
mkdir -p $output_wc_friend_imm
mkdir -p $output_wc_friend_pmc
mkdir -p $output_lt_friend_easyim
mkdir -p $output_lt_friend_imm

pmc_ic_r=200
pmc_wc_r=250

execFlowic="../Executables/icFlow"
execFlowwc="../Executables/wcFlow"

execFlowSeedsic="../Executables/icFlowSeeds"
execFlowSeedswc="../Executables/wcFlowSeeds"
execFlowSeedslt="../Executables/ltFlowSeeds"

pathtographliveic="../Dataset/soclive/graph_ic.inf"
pathtographtwitteric="../Dataset/twitter/graph_ic.inf"
pathtographorkutic="../Dataset/orkut/graph_ic.inf"
pathtographfriendic="../Dataset/friendster/graph_ic.inf"

pathtographlivewc="../Dataset/soclive/graph_wc.inf"
pathtographtwitterwc="../Dataset/twitter/graph_wc.inf"
pathtographorkutwc="../Dataset/orkut/graph_wc.inf"
pathtographfriendwc="../Dataset/friendster/graph_wc.inf"

pathtographlivelt="../Dataset/soclive/graph_lt.inf"
pathtographtwitterlt="../Dataset/twitter/graph_lt.inf"
pathtographorkutlt="../Dataset/orkut/graph_lt.inf"
pathtographfriendlt="../Dataset/friendster/graph_lt.inf"
numiter=10000

$execfilepmc $pathtographliveic 200 $pmc_ic_r $output_ic_live_pmc
$execFlowic $pathtographliveic $output_ic_live_pmc"/seeds_PMC_IC_"$pmc_ic_r".txt" 200 1 $numiter 3 $output_ic_live_pmc"/flowst_"$pmc_ic_r".txt" $output_ic_live_pmc"/spread_"$pmc_ic_r".txt"

$execfilepmc $pathtographorkutic 200 $pmc_ic_r $output_ic_orkut_pmc
$execFlowic $pathtographorkutic $output_ic_orkut_pmc"/seeds_PMC_IC_"$pmc_ic_r".txt" 200 1 $numiter 3 $output_ic_orkut_pmc"/flowst_"$pmc_ic_r".txt" $output_ic_orkut_pmc"/spread_"$pmc_ic_r".txt"

$execfilepmc $pathtographtwitteric 200 $pmc_ic_r $output_ic_twitter_pmc
$execFlowic $pathtographtwitteric $output_ic_twitter_pmc"/seeds_PMC_IC_"$pmc_ic_r".txt" 200 1 $numiter 3 $output_ic_twitter_pmc"/flowst_"$pmc_ic_r".txt" $output_ic_twitter_pmc"/spread_"$pmc_ic_r".txt"

$execfilepmc $pathtographfriendic 200 $pmc_ic_r $output_ic_friend_pmc
$execFlowic $pathtographfriendic $output_ic_friend_pmc"/seeds_PMC_IC_"$pmc_ic_r".txt" 200 1 $numiter 3 $output_ic_friend_pmc"/flowst_"$pmc_ic_r".txt" $output_ic_friend_pmc"/spread_"$pmc_ic_r".txt"



$execfilepmc $pathtographlivewc 200 $pmc_wc_r $output_wc_live_pmc
$execFlowic $pathtographlivewc $output_wc_live_pmc"/seeds_PMC_IC_"$pmc_wc_r".txt" 200 1 $numiter 3 $output_wc_live_pmc"/flowst_"$pmc_wc_r".txt" $output_wc_live_pmc"/spread_"$pmc_wc_r".txt"

$execfilepmc $pathtographorkutwc 200 $pmc_wc_r $output_wc_orkut_pmc
$execFlowic $pathtographorkutwc $output_wc_orkut_pmc"/seeds_PMC_IC_"$pmc_wc_r".txt" 200 1 $numiter 3 $output_wc_orkut_pmc"/flowst_"$pmc_wc_r".txt" $output_wc_orkut_pmc"/spread_"$pmc_wc_r".txt"

$execfilepmc $pathtographtwitterwc 200 $pmc_wc_r $output_wc_twitter_pmc
$execFlowic $pathtographtwitterwc $output_wc_twitter_pmc"/seeds_PMC_IC_"$pmc_wc_r".txt" 200 1 $numiter 3 $output_wc_twitter_pmc"/flowst_"$pmc_wc_r".txt" $output_wc_twitter_pmc"/spread_"$pmc_wc_r".txt"

$execfilepmc $pathtographfriendwc 200 $pmc_wc_r $output_wc_friend_pmc
$execFlowic $pathtographfriendwc $output_wc_friend_pmc"/seeds_PMC_IC_"$pmc_wc_r".txt" 200 1 $numiter 3 $output_wc_friend_pmc"/flowst_"$pmc_wc_r".txt" $output_wc_friend_pmc"/spread_"$pmc_wc_r".txt"

seeds=200
iteric=50
iterwc=50
iterlt=25
flowiter=10000
echo "easyim IC"
$execfileeasyimic $pathtographliveic 2 $seeds 1 $iteric 3 $output_ic_live_easyim
$execFlowSeedsic $pathtographliveic $output_ic_live_easyim"/seeds_seeds="$seeds"_iter="$iteric"_depth=3.txt" $seeds 1 $flowiter 3 $output_ic_live_easyim"/flowstat_"$iteric".txt" $output_ic_live_easyim"/spread.txt"

$execfileeasyimic $pathtographorkutic 2 $seeds 1 $iteric 3 $output_ic_orkut_easyim
$execFlowSeedsic $pathtographorkutic $output_ic_orkut_easyim"/seeds_seeds="$seeds"_iter="$iteric"_depth=3.txt" $seeds 1 $flowiter 3 $output_ic_orkut_easyim"/flowstat_"$iteric".txt" $output_ic_orkut_easyim"/spread.txt"

$execfileeasyimic $pathtographtwitteric 2 $seeds 1 $iteric 3 $output_ic_twitter_easyim
$execFlowSeedsic $pathtographtwitteric $output_ic_twitter_easyim"/seeds_seeds="$seeds"_iter="$iteric"_depth=3.txt" $seeds 1 $flowiter 3 $output_ic_twitter_easyim"/flowstat_"$iteric".txt" $output_ic_twitter_easyim"/spread.txt"

$execfileeasyimic $pathtographfriendic 2 $seeds 1 $iteric 3 $output_ic_friend_easyim
$execFlowSeedsic $pathtographfriendic $output_ic_friend_easyim"/seeds_seeds="$seeds"_iter="$iteric"_depth=3.txt" $seeds 1 $flowiter 3 $output_ic_friend_easyim"/flowstat_"$iteric".txt" $output_ic_friend_easyim"/spread.txt"


echo "Easyim wc"
$execfileeasyimwc $pathtographlivewc 2 $seeds 1 $iterwc 3 $output_wc_live_easyim
$execFlowSeedswc $pathtographlivewc $output_wc_live_easyim"/seeds_seeds="$seeds"_iter="$iterwc"_depth=3.txt" $seeds 1 $flowiter 3 $output_wc_live_easyim"/flowstat_"$iterwc".txt" $output_wc_live_easyim"/spread.txt"

$execfileeasyimwc $pathtographorkutwc 2 $seeds 1 $iterwc 3 $output_wc_orkut_easyim
$execFlowSeedswc $pathtographorkutwc $output_wc_orkut_easyim"/seeds_seeds="$seeds"_iter="$iterwc"_depth=3.txt" $seeds 1 $flowiter 3 $output_wc_orkut_easyim"/flowstat_"$iterwc".txt" $output_wc_orkut_easyim"/spread.txt"

$execfileeasyimwc $pathtographtwitterwc 2 $seeds 1 $iterwc 3 $output_wc_twitter_easyim
$execFlowSeedswc $pathtographtwitterwc $output_wc_twitter_easyim"/seeds_seeds="$seeds"_iter="$iterwc"_depth=3.txt" $seeds 1 $flowiter 3 $output_wc_twitter_easyim"/flowstat_"$iterwc".txt" $output_wc_twitter_easyim"/spread.txt"

$execfileeasyimwc $pathtographfriendwc 2 $seeds 1 $iterwc 3 $output_wc_friend_easyim
$execFlowSeedswc $pathtographfriendwc $output_wc_friend_easyim"/seeds_seeds="$seeds"_iter="$iterwc"_depth=3.txt" $seeds 1 $flowiter 3 $output_wc_friend_easyim"/flowstat_"$iterwc".txt" $output_wc_friend_easyim"/spread.txt"


echo "Easyim lt"
$execfileeasyimlt $pathtographlivelt 2 $seeds 1 $iterlt 3 $output_lt_live_easyim
$execFlowSeedslt $pathtographlivelt $output_lt_live_easyim"/seeds_seeds="$seeds"_iter="$iterlt"_depth=3.txt" $seeds 1 $flowiter 3 $output_lt_live_easyim"/flowstat_"$iterlt".txt" $output_lt_live_easyim"/spread.txt"

$execfileeasyimlt $pathtographorkutlt 2 $seeds 1 $iterlt 3 $output_lt_orkut_easyim
$execFlowSeedslt $pathtographorkutlt $output_lt_orkut_easyim"/seeds_seeds="$seeds"_iter="$iterlt"_depth=3.txt" $seeds 1 $flowiter 3 $output_lt_orkut_easyim"/flowstat_"$iterlt".txt" $output_lt_orkut_easyim"/spread.txt"

$execfileeasyimlt $pathtographtwitterlt 2 $seeds 1 $iterlt 3 $output_lt_twitter_easyim
$execFlowSeedslt $pathtographtwitterlt $output_lt_twitter_easyim"/seeds_seeds="$seeds"_iter="$iterlt"_depth=3.txt" $seeds 1 $flowiter 3 $output_lt_twitter_easyim"/flowstat_"$iterlt".txt" $output_lt_twitter_easyim"/spread.txt"

$execfileeasyimlt $pathtographfriendlt 2 $seeds 1 $iterlt 3 $output_lt_friend_easyim
$execFlowSeedslt $pathtographfriendlt $output_lt_friend_easyim"/seeds_seeds="$seeds"_iter="$iterlt"_depth=3.txt" $seeds 1 $flowiter 3 $output_lt_friend_easyim"/flowstat_"$iterlt".txt" $output_lt_friend_easyim"/spread.txt"

immwc="0.1"
timlt="0.35"

echo "IMM WC"
$execfileimm -dataset $pathtographlivewc -k $seeds -model IC -epsilon $immwc -output $output_wc_live_imm
$execFlowSeedswc $pathtographlivewc $output_wc_live_imm"/seeds_IMM_IC_"$seeds"_"$immwc".txt" $seeds 1 $flowiter 3 $output_wc_live_imm"/flowstat_"$immic $output_wc_live_imm"/spread_"$immic

$execfileimm -dataset $pathtographorkutwc -k $seeds -model IC -epsilon $immwc -output $output_wc_orkut_imm
$execFlowSeedswc $pathtographorkutwc $output_wc_orkut_imm"/seeds_IMM_IC_"$seeds"_"$immwc".txt" $seeds 1 $flowiter 3 $output_wc_orkut_imm"/flowstat_"$immic $output_wc_orkut_imm"/spread_"$immic

$execfileimm -dataset $pathtographtwitterwc -k $seeds -model IC -epsilon $immwc -output $output_wc_twitter_imm
$execFlowSeedswc $pathtographtwitterwc $output_wc_twitter_imm"/seeds_IMM_IC_"$seeds"_"$immwc".txt" $seeds 1 $flowiter 3 $output_wc_twitter_imm"/flowstat_"$immic $output_wc_twitter_imm"/spread_"$immic

$execfileimm -dataset $pathtographfriendwc -k $seeds -model IC -epsilon $immwc -output $output_wc_friend_imm
$execFlowSeedswc $pathtographfriendwc $output_wc_friend_imm"/seeds_IMM_IC_"$seeds"_"$immwc".txt" $seeds 1 $flowiter 3 $output_wc_friend_imm"/flowstat_"$immic $output_wc_friend_imm"/spread_"$immic
echo "TIM LT"
$execfiletim -dataset $pathtographlivelt -k $seeds -model LT -epsilon $timlt -outdir $output_lt_live_tim
$execFlowSeedslt $pathtographlivelt $output_lt_live_tim"/seeds_TIM_LT_"$seeds"_"$timlt".txt" $seeds 1 $flowiter 3 $output_lt_live_tim"/flowstat_"$eps $output_lt_live_tim"/spread_"$eps

$execfiletim -dataset $pathtographorkutlt -k $seeds -model LT -epsilon $timlt -outdir $output_lt_orkut_tim
$execFlowSeedslt $pathtographorkutlt $output_lt_orkut_tim"/seeds_TIM_LT_"$seeds"_"$timlt".txt" $seeds 1 $flowiter 3 $output_lt_orkut_tim"/flowstat_"$eps $output_lt_orkut_tim"/spread_"$eps

$execfiletim -dataset $pathtographtwitterlt -k $seeds -model LT -epsilon $timlt -outdir $output_lt_twitter_tim
$execFlowSeedslt $pathtographtwitterlt $output_lt_twitter_tim"/seeds_TIM_LT_"$seeds"_"$timlt".txt" $seeds 1 $flowiter 3 $output_lt_twitter_tim"/flowstat_"$eps $output_lt_twitter_tim"/spread_"$eps
$execfiletim -dataset $pathtographfriendlt -k $seeds -model LT -epsilon $timlt -outdir $output_lt_friend_tim
$execFlowSeedslt $pathtographfriendlt $output_lt_friend_tim"/seeds_TIM_LT_"$seeds"_"$timlt".txt" $seeds 1 $flowiter 3 $output_lt_friend_tim"/flowstat_"$eps $output_lt_friend_tim"/spread_"$eps
