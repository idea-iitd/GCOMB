import networkx as nx
import random
import sys

def calculate_spread(main_graph,seed_nodes,new_node):
	if len(seed_nodes)==0:
		return 0
	for node in seed_nodes:
		main_graph.add_edge(new_node,node)

	total_nodes = len(nx.descendants(main_graph,new_node))
	main_graph.remove_node(new_node)
	return total_nodes


seedFile = open(sys.argv[2])
seedNodes = seedFile.readlines()
seed_nodes = []
for seedNode in seedNodes:
	seed_nodes.append(int(seedNode))

new_node_to_be_added = int(sys.argv[3]) + 1
number_simulations = int(sys.argv[4])

fileHandle = open(sys.argv[1])
content = fileHandle.readlines()
spread = 0.0
for i in range(0,number_simulations):
	G = nx.DiGraph()
	for line in content:
		sample_prob = random.uniform(0.0,1.0)
		list_edge = line.split()
		if float(list_edge[2])>=sample_prob:
			G.add_edge(int(list_edge[0]), int(list_edge[1]))

	spread += calculate_spread(G,seed_nodes,new_node_to_be_added)

print('spread = ',spread*1.0/number_simulations)