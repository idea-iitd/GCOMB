import numpy as np
import nn
from random import randint
import networkx as nx
import random
from sklearn import preprocessing
from networkx.readwrite import json_graph
import evaluate
import copy
import json
from networkx.algorithms import bipartite
import pickle

from sklearn import preprocessing
from networkx.readwrite import json_graph
import json
import pickle
from sklearn.preprocessing import StandardScaler
import copy
from sklearn.decomposition import PCA



def read_json_file(filename):
	with open(filename) as f:
		js_graph = json.load(f)
	return json_graph.node_link_graph(js_graph)
	
def getnewgraph(graph_dir, num_k, sampling_freq):
	top_ten_file = open(graph_dir + "_top_ten_percent.txt"+"_"+ str(num_k)+"_nbs_"+str(sampling_freq))
	top_ten_file_content = top_ten_file.read()
	top_tenpct_nodes = list(map(int,top_ten_file_content.split()))
	print(top_tenpct_nodes)
	# embedding_file = graph_dir + "_embeddings.npy"
	# embeddings_ordered = np.load(embedding_file)
	# embeddings_ordered = np.array(embeddings_ordered,dtype='float64')
	# m,n = embeddings_ordered.shape
	#
	# graph_json_filename = graph_dir + "-G.json"
	# main_graph = read_json_file(graph_json_filename)


	embedding_file=graph_dir + "_embeddings.npy"+"_"+ str(num_k)+"_nbs_"+str(sampling_freq)+".pickle"
	#	embeddings_ordered = np.load(embedding_file)
	# embeddings_ordered = np.array(embeddings_ordered,dtype='float64')
	# m,n = embeddings_ordered.shape
	#
	embeddings_dict={}
	with open(embedding_file, 'rb') as handle:
		embeddings_dict=pickle.load(handle)

	# embeddings_ordered = np.append(empty_column_for_cover, embeddings_ordered, axis=1)

	graph_json_filename=graph_dir + "-G.json"
	main_graph=read_json_file(graph_json_filename)
	m=len(main_graph)
	empty_column_for_cover=np.array([2], dtype='float64')

	dict_node_scores_file_name=graph_dir+ "_node_scores_supgs"+"_"+ str(num_k)+"_nbs_"+str(sampling_freq)+".pickle"
	with open(dict_node_scores_file_name, 'rb') as handle:
		dict_sup_gs_scores=pickle.load(handle)


	dict_node_sampled_neighbors_file_name=graph_dir + "-sampled_nbrs_for_rl.pickle"+"_"+ str(num_k)+"_nbs_"+str(sampling_freq)
	with open(dict_node_sampled_neighbors_file_name, 'rb') as handle:
		dict_node_sampled_neighbors =pickle.load(handle)

	print("deg")
	#out_deg_wt_graph  = main_graph.out_degree( weight='weight')

	for k, v in embeddings_dict.items():
		embeddings_dict[k]=np.array([1, 1],
									dtype='float64')  # np.concatenate((empty_column_for_cover, embeddings_dict[k]))#np.array([1,1], dtype='float64')#empty_column_for_cover#np.concatenate((empty_column_for_cover))#, empty_column_for_cover))
		embeddings_dict[k][0]=len( dict_node_sampled_neighbors[k])#main_graph.degree(k)
		#	embeddings_dict[k][1]= main_graph.degree(k)#dict_sup_gs_scores[k]#main_graph.degree(k)
		embeddings_dict[k][1]=dict_sup_gs_scores[k]#out_deg_wt_graph[k]#dict_sup_gs_scores[k]#main_graph.degree(k)#dict_sup_gs_scores[k]  # main_graph.degree(k)
	#print(embeddings_dict)
	scaler=StandardScaler()
	temp_column_for_cover=np.ones((len(embeddings_dict), 2), dtype='float64')

	i=0
	dict_map_i_key={}
	for key, value in embeddings_dict.items():
		temp_column_for_cover[i]=value
		dict_map_i_key[i]=key
		i+=1
	#print(embeddings_dict)
	scaler.fit(temp_column_for_cover)
	temp_column_for_cover_norm=None
	temp_column_for_cover_norm=scaler.transform(temp_column_for_cover)

	#
	# pca=PCA(n_components=2)
	# temp_column_for_cover_norm_pca = pca.fit_transform(temp_column_for_over_norm[:,1:])

	for index, value in enumerate(temp_column_for_cover_norm):
		true_node_id=dict_map_i_key[index]
		embeddings_dict[true_node_id][0]=value[
			0]  # np.concatenate((np.array([temp_column_for_cover_norm[index][0]]),value))
		embeddings_dict[true_node_id][1]=value[1]
	# embeddings_dict[true_node_id][2]=value[2]




	return (main_graph,embeddings_dict,m,top_tenpct_nodes,dict_node_sampled_neighbors )


def solve(graph_dir):
	top_ten_file=open(graph_dir + "_top_ten_percent.txt")
	top_ten_file_content=top_ten_file.read()
	top_tenpct_nodes=np.array(map(int, top_ten_file_content.split()))


	graph_json_filename=graph_dir + "-G.json"

	main_graph=read_json_file(graph_json_filename)
	m=len(main_graph)
	cov = {}#[ main_graph.degree(x) ]


	for node in top_tenpct_nodes:
		cov[node] = main_graph.degree(node)

	neighbors_chosen_till_now = set()
	sol = list()


	import operator

	stats={'a': 1000, 'b': 3000, 'c': 100}

	for step in range(0,15):
		chosen_node = max(cov.iteritems(), key=operator.itemgetter(1))[0]
		print("chosen node ", chosen_node)
		sol.append(chosen_node)

		neighbors_of_chosen_node=set(main_graph.neighbors(chosen_node))

	#	new_neighbors_not_in_solutions_neighbors=neighbors_of_chosen_node - neighbors_chosen_till_now

		neighbors_chosen_till_now= neighbors_chosen_till_now.union(neighbors_of_chosen_node)

		for node in top_tenpct_nodes:
			neighbors_of_node=set(main_graph.neighbors(node))
			new_neighbors_not_in_solutions_neighbors=neighbors_of_node - neighbors_chosen_till_now
			cov[node]=len(new_neighbors_not_in_solutions_neighbors)
		#	print(node, len(new_neighbors_not_in_solutions_neighbors))

	print(" solution ", sol)
	reward=evaluate.evaluate(copy.deepcopy(main_graph), sol)
	print("reward  rl", reward)
	print(" graph dir ", graph_dir)
	file_handle2=open(graph_dir + "-reward_rl15", "w")
	file_handle2.write(str(sol)+"\n"+str(reward))
	file_handle2.close()
	#
	# #
	# file_handlerdgrd=open(graph_dir + "-G.json.greedy_Sol", "r")
	# greedy_sol = map(int, file_handlerdgrd.readlines()[1].split())[0:15]
	# print(greedy_sol)
	#
	#
	# reward=evaluate.evaluate(copy.deepcopy(main_graph), greedy_sol)
	# print("rew greedy ", reward)
	# print(" graph dir ", graph_dir)
	# file_handle2=open(graph_dir + "-reward_greedy15", "w")
	# file_handle2.write(str(reward))
	# file_handle2.close()



	sorted_dict = sorted( main_graph.degree().items(), key=operator.itemgetter(1),reverse=True)


	bottom_nodes, top_nodes=bipartite.sets(main_graph)


	bottom_nodes=list(bottom_nodes)

	top_k_sol = []

	# for i in range(0, 15):
	# 	top_k_sol.append(sorted_dict[i][0])
	#

	count_sol=0
	i=0
	top_k_sol=[]
	print(sorted_dict)
	print(" bottom node ", bottom_nodes)
	while count_sol < 15:
		if sorted_dict[i][0] in bottom_nodes:
			print(" short ", sorted_dict[i][0])
			count_sol=count_sol + 1
			top_k_sol.append(sorted_dict[i][0])
		i+=1

	print(" top k", top_k_sol)
	reward=evaluate.evaluate(copy.deepcopy(main_graph), top_k_sol)
	print("rew topk  ", reward)
	print(" graph dir ", graph_dir)
	file_handle2=open(graph_dir + "-reward_topk_15", "w")
	file_handle2.write(str(top_k_sol)+"\n"+str(reward))
	file_handle2.close()


class Graph:
	def __init__(self,graph_dir, num_k, sampling_freq):
		#solve(graph_dir)
		#exit()
		(self.graphx, unscaled_embedding, self.numNodes, self.top_tenpct_nodes, self.dict_node_sampled_neighbors ) = getnewgraph(graph_dir, num_k, sampling_freq)
#		self.isselected = [-1 for _ in range(0, self.numnodes)]
		self.isSelected={}
		for key in self.top_tenpct_nodes:
			self.isSelected[key]=-1
	#	self.iscounted = [False for _ in range(0, self.numnodes)]
		self.isCounted={}
		for x in range(0, self.numNodes):
			self.isCounted[x]=False
		self.state = [] # sequence of nodes selected
		self.cumulativereward = []
		self.embedding = unscaled_embedding
		self.embedding_time={}
		for i in range(0, num_k + 1):
			self.embedding_time[i]=copy.deepcopy(self.embedding)

		#print(self.embedding)
		self.neighbors_chosen_till_now=set()
