import graphGenerator
import numpy as np
import nn
from random import randint
import networkx as nx
import random
from sklearn import preprocessing
from networkx.readwrite import json_graph
import json
import graphEnv
import pickle
from sklearn.preprocessing import StandardScaler
import copy
from sklearn.decomposition import PCA

def read_json_file(filename):
	print("graph file ", filename)
	with open(filename) as f:
		js_graph = json.load(f)
	return json_graph.node_link_graph(js_graph)

def getNewGraph(graph_dir, num_k, neighborhood_sampling):
	top_ten_file = open(graph_dir + "_top_ten_percent{}_nbs{}.txt".format(num_k, neighborhood_sampling))
	top_ten_file_content = top_ten_file.read()
	top_tenpct_nodes = list(map(int,top_ten_file_content.split()))

	embedding_file=graph_dir + "_embeddings{}_nbs{}.npy.pickle".format(num_k, neighborhood_sampling)
#	embeddings_ordered = np.load(embedding_file)
	#embeddings_ordered = np.array(embeddings_ordered,dtype='float64')
	# m,n = embeddings_ordered.shape
	#
	embeddings_dict = {}
	with open(embedding_file, 'rb') as handle:
		embeddings_dict = pickle.load(handle)

	#embeddings_ordered = np.append(empty_column_for_cover, embeddings_ordered, axis=1)

	graph_json_filename = graph_dir + "-G.json"
	main_graph = read_json_file(graph_json_filename)
	m = len(main_graph)
	empty_column_for_cover = np.array([2], dtype='float64')


	dict_node_scores_file_name=graph_dir + "_node_scores_supgs{}_nbs{}.pickle".format(num_k, neighborhood_sampling)
	with open(dict_node_scores_file_name, 'rb') as handle:
		dict_sup_gs_scores =pickle.load(handle)

	dict_node_sampled_neighbors_file_name=graph_dir + "-sampled_nbrs_for_rl{}_nbs{}.pickle".format(num_k,neighborhood_sampling)
	with open(dict_node_sampled_neighbors_file_name, 'rb') as handle:
		dict_node_sampled_neighbors =pickle.load(handle)

	for k, v in embeddings_dict.items():
			embeddings_dict[k]= np.array([1,1], dtype='float64')# np.concatenate((empty_column_for_cover, embeddings_dict[k]))#np.array([1,1], dtype='float64')#empty_column_for_cover#np.concatenate((empty_column_for_cover))#, empty_column_for_cover))
			embeddings_dict[k][0] = len( dict_node_sampled_neighbors[k])# main_graph.degree(k)#len( dict_node_sampled_neighbors[k])#main_graph.degree(k)
		#	embeddings_dict[k][1]= main_graph.degree(k)#dict_sup_gs_scores[k]#main_graph.degree(k)
			embeddings_dict[k][1]=dict_sup_gs_scores[k]#main_graph.degree(k)

	scaler=StandardScaler()
	temp_column_for_cover=np.ones((len(embeddings_dict), 2), dtype='float64')
	#print(embeddings_dict)

	# for k, v in embeddings_dict.items():
	# 		embeddings_dict[k]= np.array([1,1], dtype='float64')# np.concatenate((empty_column_for_cover, embeddings_dict[k]))#np.array([1,1], dtype='float64')#empty_column_for_cover#np.concatenate((empty_column_for_cover))#, empty_column_for_cover))
	# 		embeddings_dict[k][0] = len( dict_node_sampled_neighbors[k])#main_graph.degree(k)
	# 	#	embeddings_dict[k][1]= main_graph.degree(k)#dict_sup_gs_scores[k]#main_graph.degree(k)
	# 		embeddings_dict[k][1]=dict_sup_gs_scores[k]#main_graph.degree(k)

	scaler=StandardScaler()
	temp_column_for_cover=np.ones((len(embeddings_dict), 2), dtype='float64')

	i=0
	dict_map_i_key={}
	i=0
	dict_map_i_key={}
	for key, value in embeddings_dict.items():
		temp_column_for_cover[i]=value
		dict_map_i_key[i]=key
		i+=1

	scaler.fit(temp_column_for_cover)
	temp_column_for_cover_norm=None
	temp_column_for_cover_norm=scaler.transform(temp_column_for_cover)

	#
	# pca=PCA(n_components=2)
	# temp_column_for_cover_norm_pca = pca.fit_transform(temp_column_for_over_norm[:,1:])

	for index, value in enumerate(temp_column_for_cover_norm):
		true_node_id=dict_map_i_key[index]
		embeddings_dict[true_node_id][0]=value[0]#np.concatenate((np.array([temp_column_for_cover_norm[index][0]]),value))
		embeddings_dict[true_node_id][1]=value[1]
		#embeddings_dict[true_node_id][2]=value[2]





	return (main_graph,embeddings_dict ,m,top_tenpct_nodes, dict_node_sampled_neighbors)

class Graph:
	def __init__(self, dimEmbedding, episodeNum, num_k,nbs):
		cons_to_add = 0#episodeNum
		episodeNum = episodeNum%5
		# (self.graphX, unscaled_embedding, self.numNodes, self.top_tenpct_nodes) = graphGenerator.getNewGraph(dimEmbedding, episodeNum, num_k)
#		(self.graphX, unscaled_embedding, self.numNodes, self.top_tenpct_nodes) = getNewGraph("./GraphSAGE-master/real_data/large_kite/train/_bp")
		# (self.graphX, unscaled_embedding, self.numNodes, self.top_tenpct_nodes, self.dict_node_sampled_neighbors) = getNewGraph("./GraphSAGE-master/graph_data/graph" + str(episodeNum+cons_to_add) + "/graph" + str(episodeNum+cons_to_add))
		(self.graphX, unscaled_embedding, self.numNodes, self.top_tenpct_nodes, self.dict_node_sampled_neighbors) = getNewGraph("./GraphSAGE-master/real_data/large_kite/train/_bp",num_k, nbs)
	#	self.isSelected = [-1 for _ in range(0, len(self.top_tenpct_nodes))]
		self.isCounted = {}#[False for _ in range(0, len(self.top_tenpct_nodes))]

		self.isCounted={}
		for x in range(0, self.numNodes):
			self.isCounted[x] =False

		self.state = [] # sequence of nodes selected
		self.cumulativeReward = []
		self.embedding = unscaled_embedding
		self.neighbors_chosen_till_now = set()

		self.isSelected = {}
		for key in self.top_tenpct_nodes:
			self.isSelected[key] = -1

		print(self.isSelected)
		self.cumulativeReward = []
		self.embedding = unscaled_embedding

		self.embedding_time = {}
		for i in range(0, num_k+1):
			self.embedding_time[i] = copy.deepcopy(self.embedding)





def init(learningRate, numOfEpochs, batchSize, dimension,  model_iter):
	nn.init(learningRate, numOfEpochs, batchSize, dimension , model_iter)

# gives the index of random unselected node
def getRandomNode(graphid,stateIdx):
	print("graph id", graphid)
	# graph = graphEnv.graphEnvironment[graphid]
	# count_unselect = 0
	# for r_node in graph.top_tenpct_nodes:
	# 	if graph.isSelected[r_node]==-1:
	# 		count_unselect = count_unselect + 1
	# randomSelection = randint(1,count_unselect)
	# count_unselect = 0
	# for r_node in graph.top_tenpct_nodes:
	# 	if graph.isSelected[r_node]==-1:
	# 		count_unselect = count_unselect + 1
	# 		if count_unselect==randomSelection:
	# 			selection = r_node
	# 			break
	graph=graphEnv.graphEnvironment[graphid]
	for r_node in graph.top_tenpct_nodes:
		if graph.isSelected[r_node]==-1:
			selection = r_node
			break
	return selection

# returns the index of the selected node based on exploration and exploitation
def getNode(probOfRandomSelection, graphid, stateIdx):
	prob = random.uniform(0,1)
	if (prob <= probOfRandomSelection):
		print("Getting Random Node")
		nodeSelected = getRandomNode(graphid, stateIdx)
	else:
		print("Getting Best Node")
		(nodeSelected,_) = nn.getBestNode(graphid, stateIdx)
		if nodeSelected==-1:
			nodeSelected = getRandomNode(graphid,stateIdx)
	# graphEnv.graphEnvironment[graphid].top_tenpct_nodes.remove(nodeSelected)
	return nodeSelected

# net addition to the influence
# influence: Number
def getShortReward(nodeSelected, graphid):
	additionCount = 0
	list_of_neighbors = graphEnv.graphEnvironment[graphid].graphX.neighbors(nodeSelected)
	print(" neighors ", list_of_neighbors)
	for v in list_of_neighbors:
		if (graphEnv.graphEnvironment[graphid].isCounted[v] == False):
			additionCount += 1
			#print('[v] ' , v)
			graphEnv.graphEnvironment[graphid].isCounted[v] = True
	return additionCount

def updateParameters(historyOfTuples):
	nn.trainNeuralNet(historyOfTuples)

def createMuUtil(lastIdx, graphid):
	return nn.createMu(lastIdx, graphEnv.graphEnvironment[graphid])