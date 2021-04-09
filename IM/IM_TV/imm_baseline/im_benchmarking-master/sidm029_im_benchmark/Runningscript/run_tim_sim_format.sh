!/bin/bash
execTimFormat="../Executables/timFormat"
echo "Preparing Nethept"
$execTimFormat ./nethept/nethept.txt 1 0.1
cd nethept
./sort.sh
./sim.sh
cd ..

echo "Preparing HepPh"
$execTimFormat ./hepph/hepph.txt 1 0.1
cd hepph
./sort.sh
./sim.sh
cd ..

echo "Preparing DBLP"
size=$(wc -l < ./dblp/dblp.txt)
newSize=`expr $size - 4`
tail -$newSize ./dblp/dblp.txt > ./dblp/dblp_clean.txt
cp ./dblp/dblp.txt ./dblp/dblp_orig.txt
mv ./dblp/dblp_clean.txt ./dblp/dblp.txt
$execTimFormat ./dblp/dblp.txt 1 0.1
cd dblp
./sort.sh
./sim.sh
cd ..

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

echo "Preparing SocLive"
size=$(wc -l < ./soclive/livejournal.txt)
newSize=`expr $size - 4`
tail -$newSize ./soclive/livejournal.txt > ./soclive/livejournal_clean.txt
cp ./soclive/livejournal.txt ./soclive/livejournal_orig.txt
mv ./soclive/livejournal_clean.txt ./soclive/livejournal.txt
$execTimFormat ./soclive/livejournal.txt 2 0.1
cd soclive
./sort.sh
./sim.sh
cd ..

echo "Preparing Orkut"
size=$(wc -l < ./orkut/orkut.txt)
newSize=`expr $size - 4`
tail -$newSize ./orkut/orkut.txt > ./orkut/orkut_clean.txt
cp ./orkut/orkut.txt ./orkut/orkut_orig.txt
mv ./orkut/orkut_clean.txt ./orkut/orkut.txt
$execTimFormat ./orkut/orkut.txt 1 0.1
cd orkut
./sort.sh
./sim.sh
cd ..

echo "Preparing Twitter"
$execTimFormat ./twitter/twitter.txt 2 0.1
cd twitter
./sort.sh
./sim.sh
cd ..

echo "Preparing Friendster"
size=$(wc -l < ./friendster/friendster.txt)
newSize=`expr $size - 4`
tail -$newSize ./friendster/friendster.txt > ./friendster/friendster_clean.txt
cp ./friendster/friendster.txt ./friendster/friendster_orig.txt
mv ./friendster/friendster_clean.txt ./friendster/friendster.txt
$execTimFormat ./friendster/friendster.txt 1 0.1
cd friendster
./sort.sh
./sim.sh
cd ..
