import numpy as np
import os
from networkx.readwrite import json_graph
import json
import networkx as nx
import gen_graph
from sklearn import preprocessing


# returns (embedding, numNodes):
	# adj: list : adjacency list of ints {assuming unweighted graphs}
	# embedding: np.array((numNodes,dimension))
	# numNodes: Integer

def read_json_file(filename):
	with open(filename) as f:
		js_graph = json.load(f)
	return json_graph.node_link_graph(js_graph)

def getNewGraph(dimEmbedding, episodeNum, num_k):
	#gen_graph.genNewGraph(episodeNum,2000,0.4,num_k)

	# os.chdir("./GraphSAGE-master")
	# command = "sh ./supervisedPredict.sh " + "./graph_data/graph" + str(episodeNum) + "/graph" + str(episodeNum)
	# print("getNewGraph: running command: ", command)
	# os.system(command)
	#os.chdir("../")

	cons_to_add = 110

	top_ten_file = open("./GraphSAGE-master/graph_data/graph" + str(episodeNum+cons_to_add) + "/graph" + str(episodeNum+cons_to_add) + "_top_ten_percent.txt")
	top_ten_file_content = top_ten_file.read()
	top_tenpct_nodes = list(map(int,top_ten_file_content.split()))

	embedding_file = "./GraphSAGE-master/graph_data/graph" + str(episodeNum+cons_to_add) + "/graph" + str(episodeNum+cons_to_add) + "_embeddings.npy"
	embeddings_ordered = np.load(embedding_file)
	embeddings_ordered = np.array(embeddings_ordered,dtype='float64')
	m,n = embeddings_ordered.shape

	graph_json_filename = "./GraphSAGE-master/graph_data/graph" + str(episodeNum+cons_to_add)+ "/graph" + str(episodeNum+cons_to_add) +"-G.json"
	main_graph = read_json_file(graph_json_filename)

	return (main_graph,embeddings_ordered,m,top_tenpct_nodes)