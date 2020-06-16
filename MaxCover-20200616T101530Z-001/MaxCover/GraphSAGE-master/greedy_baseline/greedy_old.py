import random
import sys
from networkx.readwrite import json_graph
from networkx.algorithms import bipartite
import json
import networkx as nx
import evaluate
import time

inputFileName = sys.argv[1]
k = int(sys.argv[2])
#outputFileName = sys.argv[3]
topKFileName = sys.argv[3]
rewardFileName = sys.argv[4]
num_itr = 1 # Number of times to run greedy

def read_json_file(filename):
	with open(filename) as f:
		js_graph = json.load(f)
	return json_graph.node_link_graph(js_graph)

def calculate_degree(node, neighbors, isCovered):
	degree = 0
	for nbr in neighbors:
		if (isCovered[nbr] == False):
			degree += 1
	return degree

def select_node(node, neighbors, isCovered):
	isCovered[node] = True
	for nbr in neighbors:
		isCovered[nbr] = True

def actual_greedy(main_graph, set_nodes):
	num_nodes = main_graph.number_of_nodes()
	
	isCovered = [False for _ in range(0,num_nodes)]

	solution_set = []

	for itr in range(0,k):
		denom = 0.0
	
		gains = [0 for _ in range(0,num_nodes)]
		for nd in set_nodes:
			gain = calculate_degree(nd, main_graph.neighbors(nd), isCovered)
			gains[nd] = gain
			denom += gain
		
		if (denom <= 1e-7):
			print('Graph covered before k steps')
			break

		selection = -1
		max_gain = -1
		for nd in set_nodes:
			if (gains[nd] >= max_gain and not isCovered[nd]):
				selection = nd
				max_gain = gains[nd]
		solution_set.append(selection)
		select_node(selection, main_graph.neighbors(selection), isCovered)
	return solution_set


main_graph = read_json_file(inputFileName)
set_nodes, element_nodes = bipartite.sets(main_graph)
set_nodes = list(set_nodes)
element_nodes = list(element_nodes)


num_nodes = main_graph.number_of_nodes()


import numpy as np 
start_time = time.time()
topk = actual_greedy(main_graph,set_nodes)
end_time = time.time()
time_elapsed = end_time - start_time
topKFile = open(topKFileName,'w')
topKFile.write(str(len(topk)) + '\n')
for node in topk:
	topKFile.write(str(node) + ' ')
topKFile.close()
print(rewardFileName)

rewardFile = open(rewardFileName, 'w')
rewardFile.write(str(evaluate.evaluate(main_graph, topk)))
rewardFile.write('\n' + str(time_elapsed))
rewardFile.close()
