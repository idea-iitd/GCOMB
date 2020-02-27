import sys
import gen_graph

num_graphs =1001
num_nodes = 1000
for i in range(num_nodes, num_graphs):
    gen_graph.genNewGraph(i,num_nodes ,0.2,15)