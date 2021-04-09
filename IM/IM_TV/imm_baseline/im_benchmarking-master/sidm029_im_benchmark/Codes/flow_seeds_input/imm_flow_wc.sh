#numCores=$1
numCores=1
./flow_wc.sh ../../TIM/nethept/sim/sim_ic.inf ../nethept_seeds/wc/ nethept 12 $numCores
./flow_wc.sh ../../TIM/hepph/sim/sim_ic.inf ../hepph_seeds/wc/ hepph 12 $numCores
./flow_wc.sh ../../TIM/dblp/sim/sim_ic.inf ../dblp_seeds/wc/ dblp 12 $numCores
./flow_wc.sh ../../TIM/youtube/sim/sim_ic.inf ../youtube_seeds/wc/ youtube 12 $numCores
#./flow_wc.sh ../../TIM/soclive/sim/sim_ic.inf ../soclive_seeds/wc/ soclive 12 $numCores
#./flow_wc.sh ../../TIM/orkut/sim/sim_ic.inf ../orkut_seeds/wc/ orkut 12 $numCores
