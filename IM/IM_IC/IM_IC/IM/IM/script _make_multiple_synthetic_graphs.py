import sys
import gen_graph_Synthetic


num_graphs = 23
num_nodes = 1000
for i in range(22, num_graphs):
    gen_graph_Synthetic.genNewGraph(i,num_nodes ,0.15,15)