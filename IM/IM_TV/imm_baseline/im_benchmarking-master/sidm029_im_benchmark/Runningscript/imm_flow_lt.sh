#numCores=$1
numCores=1
./flow_lt.sh ../../TIM/nethept/sim/sim_ic.inf ../nethept_seeds/lt/ nethept 12 $numCores
./flow_lt.sh ../../TIM/hepph/sim/sim_ic.inf ../hepph_seeds/lt/ hepph 12 $numCores
./flow_lt.sh ../../TIM/dblp/sim/sim_ic.inf ../dblp_seeds/lt/ dblp 12 $numCores
./flow_lt.sh ../../TIM/youtube/sim/sim_ic.inf ../youtube_seeds/lt/ youtube 12 $numCores
#./flow_lt.sh ../../TIM/soclive/sim/sim_ic.inf ../soclive_seeds/lt/ soclive 12 $numCores
#./flow_lt.sh ../../TIM/orkut/sim/sim_ic.inf ../orkut_seeds/lt/ orkut 12 $numCores
