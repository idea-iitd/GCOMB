#!/bin/bash

cd ..
mkdir -p Dataset
cp ./Runningscript/sim.sh ./Dataset
cp ./Runningscript/sort.sh ./Dataset
cp ./Runningscript/run_tim_sim_format.sh ./Dataset

echo "Downloading NetHept!"
cd Dataset
mkdir -p nethept
cd nethept
curl -O https://www.cse.iitk.ac.in/users/aarora/datasets/nethept.txt
curl -O https://www.cse.iitk.ac.in/users/aarora/datasets/nethept_lt_parallel.txt
cp ../sim.sh .
cp ../sort.sh .
cd ..

echo "Downloading HepPh!"
mkdir -p hepph
cd hepph
#curl -O http://snap.stanford.edu/data/ca-HepPh.txt.gz
#gunzip -c ca-HepPh.txt.gz > hepph.txt
curl -O https://www.cse.iitk.ac.in/users/aarora/datasets/hepph.txt
cp ../sim.sh .
cp ../sort.sh .
cd ..

echo "Downloading DBLP!"
mkdir -p dblp
cd dblp
curl -O http://snap.stanford.edu/data/bigdata/communities/com-dblp.ungraph.txt.gz
gunzip -c com-dblp.ungraph.txt.gz > dblp.txt
curl -O https://www.cse.iitk.ac.in/users/aarora/datasets/dblp_lt_parallel.txt
cp ../sim.sh .
cp ../sort.sh .
cd ..

echo "Downloading YouTube!"
mkdir -p youtube
cd youtube
curl -O http://snap.stanford.edu/data/bigdata/communities/com-youtube.ungraph.txt.gz
gunzip -c com-youtube.ungraph.txt.gz > youtube.txt
cp ../sim.sh .
cp ../sort.sh .
cd ..

echo "Downloading LiveJournal!"
mkdir -p soclive
cd soclive
curl -O http://snap.stanford.edu/data/soc-LiveJournal1.txt.gz
gunzip -c soc-LiveJournal1.txt.gz > livejournal.txt
cp ../sim.sh .
cp ../sort.sh .
cd ..

echo "Downloading Orkut!"
mkdir -p orkut
cd orkut
curl -O http://snap.stanford.edu/data/bigdata/communities/com-orkut.ungraph.txt.gz
gunzip -c com-orkut.ungraph.txt.gz > orkut.txt
cp ../sim.sh .
cp ../sort.sh .
cd ..

echo "Downloading Twitter!"
mkdir -p twitter
cd twitter
curl -O http://an.kaist.ac.kr/~haewoon/release/twitter_social_graph/twitter_rv.tar.gz
tar -zxvf twitter_rv.tar.gz
cp ../sim.sh .
cp ../sort.sh .
cd ..

echo "Downloading Friendster!"
mkdir -p friendster
cd friendster
curl -O http://snap.stanford.edu/data/bigdata/communities/com-friendster.ungraph.txt.gz
gunzip -c com-friendster.ungraph.txt.gz > friendster.txt
mv ../sim.sh .
mv ../sort.sh .
cd ..

echo "Preparing All Datasets for Execution!"
bash run_tim_sim_format.sh
echo "Data preparation done"
cd ../Runningscript
