mkdir -p IC
mkdir -p WC
mkdir -p LT
sort -k 1,1n -k 2,2n graph_ic.inf > IC/graph_ic.inf
sort -k 1,1n -k 2,2n graph_wc.inf > WC/graph_ic.inf
sort -k 1,1n -k 2,2n graph_lt.inf > LT/graph_lt.inf
cp attribute.txt IC/
cp attribute.txt WC/
cp attribute.txt LT/
