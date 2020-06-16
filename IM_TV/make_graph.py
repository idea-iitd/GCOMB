import sys
import gen_graph

num_Graphs = 10000
number_nodes =10000

for i in range(num_Graphs,num_Graphs+1 ):

    print("generating graph ", i)
    gen_graph.genNewGraph(i,number_nodes,0.2,55)