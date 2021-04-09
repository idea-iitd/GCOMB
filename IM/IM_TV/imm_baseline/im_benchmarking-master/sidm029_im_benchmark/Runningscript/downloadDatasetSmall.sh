#!/bin/bash

cd ..
mkdir -p Dataset
cp ./Runningscript/sim.sh ./Dataset
cp ./Runningscript/sort.sh ./Dataset
cp ./Runningscript/run_tim_sim_format_small.sh ./Dataset
cd Dataset

# echo "Downloading NetHept!"

# mkdir -p nethept
# cd nethept
# curl -O https://www.cse.iitk.ac.in/users/aarora/datasets/nethept.txt
# curl -O https://www.cse.iitk.ac.in/users/aarora/datasets/nethept_lt_parallel.txt
# cp ../sim.sh .
# cp ../sort.sh .
# cd ..

# echo "Downloading HepPh!"
# mkdir -p hepph
# cd hepph
# #curl -O http://snap.stanford.edu/data/ca-HepPh.txt.gz
# #gunzip -c ca-HepPh.txt.gz > hepph.txt
# curl -O https://www.cse.iitk.ac.in/users/aarora/datasets/hepph.txt
# cp ../sim.sh .
# cp ../sort.sh .
# cd ..

# echo "Downloading DBLP!"
# mkdir -p dblp
# cd dblp
# curl -O http://snap.stanford.edu/data/bigdata/communities/com-dblp.ungraph.txt.gz
# gunzip -c com-dblp.ungraph.txt.gz > dblp.txt
# curl -O https://www.cse.iitk.ac.in/users/aarora/datasets/dblp_lt_parallel.txt
# cp ../sim.sh .
# cp ../sort.sh .
# cd ..

# echo "Downloading YouTube!"
# mkdir -p youtube
# cd youtube
# curl -O http://snap.stanford.edu/data/bigdata/communities/com-youtube.ungraph.txt.gz
# gunzip -c com-youtube.ungraph.txt.gz > youtube.txt
# mv ../sim.sh .
# mv ../sort.sh .
# cd ..

echo "Preparing All (Small) Datasets for Execution!"
bash run_tim_sim_format_small.sh
echo "Data preparation done"
cd ../Runningscript
