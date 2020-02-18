import networkx as nx
import random
from functools import partial
import time
from networkx.readwrite import json_graph
import json
import os

import multiprocessing as mp



def create_num_MC_sim_copies(main_graph, graph_dir, mc_iter):
	print("mc iter", mc_iter)
	print(" len of graph", main_graph.number_of_nodes())
	print("graph_dir", graph_dir)
	for edge in main_graph.edges(data=True):
		inf_prob = edge[2]['weight']
		sample_prob = random.uniform(0.0, 1.0)
		if sample_prob >= inf_prob:
			main_graph.remove_edge(*edge[:2])

	data = json_graph.node_link_data(main_graph)
	graphjson = json.dumps(data)

	json_graph_name = graph_dir +'/' +str(mc_iter )+ "-G.json"
	f1 = open(json_graph_name, 'w')
	f1.write(graphjson)
	f1.close()
	print(" written graph  to ", json_graph_name)
	return mc_iter

graphs_to_process = [
		"GraphSAGE-master/real_data/youtube/TV/test/large_graph",

#	"GraphSAGE-master/_large_datasets_eval/orkut/WC/test/large_graph",
#	"GraphSAGE-master/_large_datasets_eval/orkut/TV/test/large_graph",
#	"GraphSAGE-master/_large_datasets_eval/orkut/IC/test/large_graph"

]

NUM_MC_SIM = 10000

def mp_pool_format(G_data, graph_dir , mc_iter):
	return create_num_MC_sim_copies(json_graph.node_link_graph(G_data), graph_dir, mc_iter)


for graph_dir in graphs_to_process[0:]:
	pool = mp.Pool(processes=2)

	mc_sim_directory_path = graph_dir +'/mc_sim_graphs'
	print("mc_sim_dir pat", mc_sim_directory_path)
	G_data = json.load(open(graph_dir + "-G.json"))
	print(" graph loaded")
	if not os.path.exists(mc_sim_directory_path):
		os.makedirs(mc_sim_directory_path)



	pool_args = partial(mp_pool_format, G_data, mc_sim_directory_path)
	mc_sim = [x for x in range(0, NUM_MC_SIM)]
	print(" starting work")
	for iter, res in enumerate(pool.imap_unordered(pool_args,mc_sim, chunksize=2)):
		print("iter", res)
	# for mc_sim in range(0,NUM_MC_SIM)
	#
	# 	create_num_MC_sim_copies(G, graph_dir, mc_sim)
