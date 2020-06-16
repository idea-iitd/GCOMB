wget https://snap.stanford.edu/data/bigdata/communities/com-youtube.ungraph.txt.gz
mkdir large_graph_youtube
mkdir large_graph_youtube/test
gunzip com-youtube.ungraph.txt.gz
mv com-youtube.ungraph.txt large_graph_youtube/test/large_graph-G.json.edge


wget https://snap.stanford.edu/data/loc-gowalla_edges.txt.gz
mkdir large_gowallah/
mkdir large_gowallah/test
gunzip loc-gowalla_edges.txt.gz
mv loc-gowalla_edges.txt large_gowallah/test/large_graph-G.json.edge



wget https://snap.stanford.edu/data/twitter_combined.txt.gz
mkdir large_twt_snap/
mkdir large_twt_snap/test
gunzip twitter_combined.txt.gz
mv twitter_combined.txt large_twt_snap/test/large_graph-G.json.edge

