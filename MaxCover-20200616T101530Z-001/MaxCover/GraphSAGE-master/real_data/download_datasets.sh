wget --no-check-certificate https://snap.stanford.edu/data/bigdata/communities/com-youtube.ungraph.txt.gz
mkdir large_graph_youtube
mkdir large_graph_youtube/test
gunzip com-youtube.ungraph.txt.gz
mv com-youtube.ungraph.txt large_graph_youtube/large_graph-G.json.edge


wget --no-check-certificate https://snap.stanford.edu/data/loc-gowalla_edges.txt.gz
mkdir large_gowallah/
mkdir large_gowallah/test
gunzip loc-gowalla_edges.txt.gz
mv loc-gowalla_edges.txt large_gowallah/large_graph-G.json.edge