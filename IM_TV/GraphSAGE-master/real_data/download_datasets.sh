wget https://snap.stanford.edu/data/bigdata/communities/com-orkut.ungraph.txt.gz
mkdir orkut
gunzip com-orkut.ungraph.txt.gz
mv com-orkut.ungraph.txt orkut/edges.txt

wget https://snap.stanford.edu/data/bigdata/communities/com-friendster.ungraph.txt.gz
mkdir friendster
gunzip com-friendster.ungraph.txt.gz
mv com-friendster.ungraph.txt friendster/edges.txt

