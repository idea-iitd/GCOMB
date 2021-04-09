#!/bin/bash
execTimFormat="../Executables/timFormat"
# echo "Preparing Nethept"
# $execTimFormat ./nethept/nethept.txt 1 0.1
# cd nethept
# ./sort.sh
# ./sim.sh
# cd ..

# echo "Preparing HepPh"
# $execTimFormat ./hepph/hepph.txt 1 0.1
# cd hepph
# ./sort.sh
# ./sim.sh
# cd ..

# echo "Preparing DBLP"
# size=$(wc -l < ./dblp/dblp.txt)
# newSize=`expr $size - 4`
# tail -$newSize ./dblp/dblp.txt > ./dblp/dblp_clean.txt
# cp ./dblp/dblp.txt ./dblp/dblp_orig.txt
# mv ./dblp/dblp_clean.txt ./dblp/dblp.txt
# $execTimFormat ./dblp/dblp.txt 1 0.1
# cd dblp
# ./sort.sh
# ./sim.sh
# cd ..

echo "Preparing YouTube"
size=$(wc -l < ./youtube/youtube.txt)
newSize=`expr $size - 4`
tail -$newSize ./youtube/youtube.txt > ./youtube/youtube_clean.txt
cp ./youtube/youtube.txt ./youtube/youtube_orig.txt
mv ./youtube/youtube_clean.txt ./youtube/youtube.txt
$execTimFormat ./youtube/youtube.txt 1 0.1
cd youtube
./sort.sh
./sim.sh
cd ..
