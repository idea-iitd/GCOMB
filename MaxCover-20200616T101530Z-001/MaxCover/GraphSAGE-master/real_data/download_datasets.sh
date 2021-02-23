wget --no-check-certificate https://snap.stanford.edu/data/bigdata/communities/com-youtube.ungraph.txt.gz
mkdir large_graph_youtube
mkdir large_graph_youtube/test
gunzip com-youtube.ungraph.txt.gz
mv com-youtube.ungraph.txt large_graph_youtube/large_graph-G.json.edge

