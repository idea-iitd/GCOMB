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
import evaluate_spread
from sklearn.decomposition import PCA
import os

def read_json_file(filename):
	print("graph file ", filename)
	with open(filename) as f:
		js_graph = json.load(f)
	return json_graph.node_link_graph(js_graph)

def getNewGraph(graph_dir,num_k, sampling_freq):
	#budget_used =num_k
	#top_ten_file = open(graph_dir + "_top_ten_percent.txt_{}".format(budget_used))


	top_ten_file=open(graph_dir + "_top_ten_percent.txt" + "_" + str(num_k) + "_nbs_" + str(sampling_freq))

	top_ten_file_content = top_ten_file.read()
	top_tenpct_nodes = list(map(int,top_ten_file_content.split()))

	#embedding_file = graph_dir + "_embeddings.npy_{}.pickle".format(num_k)


	embedding_file=graph_dir + "_embeddings.npy" + "_" + str(num_k) + "_nbs_" + str(sampling_freq) + ".pickle"

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

	dict_node_scores_file_name=graph_dir+ "_node_scores_supgs"+"_"+ str(num_k)+"_nbs_"+str(sampling_freq)+".pickle"

#	dict_node_scores_file_name=graph_dir + "_node_scores_supgs_{}.pickle".format(budget_used)
	with open(dict_node_scores_file_name, 'rb') as handle:
		dict_sup_gs_scores =pickle.load(handle)

	dict_node_sampled_neighbors_file_name=graph_dir + "-sampled_nbrs_for_rl.pickle"+"_"+ str(num_k)+"_nbs_"+str(sampling_freq)
#	dict_node_sampled_neighbors_file_name=graph_dir + "-sampled_nbrs_for_rl.pickle_{}".format(budget_used)
	with open(dict_node_sampled_neighbors_file_name, 'rb') as handle:
		dict_node_sampled_neighbors =pickle.load(handle)


	print("deg")
	out_deg_wt_graph  = main_graph.out_degree( weight='weight')
	for k, v in embeddings_dict.items():
			print("k ", k)
			embeddings_dict[k]= np.array([1,1], dtype='float64')# np.concatenate((empty_column_for_cover, embeddings_dict[k]))#np.array([1,1], dtype='float64')#empty_column_for_cover#np.concatenate((empty_column_for_cover))#, empty_column_for_cover))
			embeddings_dict[k][0] =len( dict_node_sampled_neighbors[k])#main_graph.degree(k)
			embeddings_dict[k][1]= dict_sup_gs_scores[k]#main_graph.degree(k)#out_deg_wt_graph[k]#dict_sup_gs_scores[k]#main_graph.degree(k)
			print(k, out_deg_wt_graph[k])
	#		embeddings_dict[k][1]=dict_sup_gs_scores[k]#main_graph.degree(k)

	print(embeddings_dict)
	scaler=StandardScaler()
	temp_column_for_cover=np.ones((len(embeddings_dict), 2), dtype='float64')

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

 


	return (main_graph,embeddings_dict ,m,top_tenpct_nodes,dict_node_sampled_neighbors)

class Graph:
	def __init__(self, dimEmbedding, episodeNum, num_k):
		cons_to_add = 5000
		self.graph_dir = "./GraphSAGE-master/real_data/youtube/TV/train/large_graph"
		#(self.graphX, unscaled_embedding, self.numNodes, self.top_tenpct_nodes) = graphGenerator.getNewGraph(dimEmbedding, episodeNum, num_k)

		(self.graphX, unscaled_embedding, self.numNodes, self.top_tenpct_nodes,self.dict_node_sampled_neighbors) = getNewGraph(self.graph_dir, num_k, 0.003)
		self.graph_dir = self.graph_dir+"/"

		#(self.graphX, unscaled_embedding, self.numNodes, self.top_tenpct_nodes) = getNewGraph("./GraphSAGE-master/graph_data/graph" + str(episodeNum+cons_to_add) + "/graph" + str(episodeNum+cons_to_add))
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





def init(learningRate, numOfEpochs, batchSize, dimension):
	nn.init(learningRate, numOfEpochs, batchSize, dimension)

# gives the index of random unselected node
def getRandomNode(graphid,stateIdx):
	print("graph id", graphid)
	graph = graphEnv.graphEnvironment[graphid]
	count_unselect = 0
	for r_node in graph.top_tenpct_nodes:
		if graph.isSelected[r_node]==-1:
			count_unselect = count_unselect + 1
	randomSelection = randint(1,count_unselect)
	count_unselect = 0
	for r_node in graph.top_tenpct_nodes:
		if graph.isSelected[r_node]==-1:
			count_unselect = count_unselect + 1
			if count_unselect==randomSelection:
				selection = r_node
				break
	# graph=graphEnv.graphEnvironment[graphid]
	# for r_node in graph.top_tenpct_nodes:
	# 	if graph.isSelected[r_node]==-1:
	# 		selection = r_node
	# 		break
	return selection

# returns the index of the selected node based on exploration and exploitation
def getNode(probOfRandomSelection, graphid, stateIdx):
	prob = random.uniform(0,1)
	print("prob",prob)
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




def calculate_spread_mc_sim(graph_dir,seed_nodes,model):

	if len(seed_nodes)==0:
		return 0
	else:
		if(model =="TV"):

			mc_path=graph_dir
			num_mc_sim=5


			if len(seed_nodes) == 0:
				return 0
			else:
				spread=evaluate_spread.evaluate_helper_without_mp(mc_path, None, seed_nodes, num_mc_sim)
				return spread

		else:
			temp_seed_file_name = "temp_seed_select.txt"
			file_temp_seed_select = open(temp_seed_file_name,'w')
			print("seed nodes ", seed_nodes)
			seed_nodes = [str(i) for i in seed_nodes]
			print("joined ", "\n".join(seed_nodes))
			file_temp_seed_select.write("\n".join(seed_nodes))

			file_temp_seed_select.close()
			os.chdir("Executables/")
			os.system("python run_eval.py " + "../"+temp_seed_file_name +" " + str(len(seed_nodes)) +" " + "../temp_reward.txt  > /dev/null")
			file_result_reward_name = "temp_reward.txt"
			os.chdir("../")
			file_result_reward = open(file_result_reward_name,'r')
			spread = float(file_result_reward.read())


		#	spread = evaluate_spread.evaluate_helper_without_mp(mc_path,None, seed_nodes,num_mc_sim)
			return spread





# net addition to the influence
# influence: Number
def getShortReward(nodeSelected, graphid, previous_spread):
	# additionCount = 0
	# list_of_neighbors = graphEnv.graphEnvironment[graphid].graphX.neighbors(nodeSelected)
	# print(" neighors ", list_of_neighbors)
	# for v in list_of_neighbors:
	# 	if (graphEnv.graphEnvironment[graphid].isCounted[v] == False):
	# 		additionCount += 1
	# 		#print('[v] ' , v)
	# 		graphEnv.graphEnvironment[graphid].isCounted[v] = True
	#
	# return additionCount
	if(len(graphEnv.graphEnvironment[graphid].state)==0):
		print(" first step ")
		shortReward = calculate_spread_mc_sim(graphEnv.graphEnvironment[graphid].graph_dir, [nodeSelected],"TV")
		print(" short reward ", shortReward)
		return shortReward, shortReward

	seed_nodes_earlier = graphEnv.graphEnvironment[graphid].state[:]
	seed_nodes_later = seed_nodes_earlier[:]
	seed_nodes_later.append(nodeSelected)
	new_node_to_be_added = graphEnv.graphEnvironment[graphid].numNodes + 1
#	initial_spread = calculate_spread_mc_sim(graphEnv.graphEnvironment[graphid].graphX, seed_nodes_earlier, new_node_to_be_added)
#	print("before ",initial_spread, seed_nodes_earlier, new_node_to_be_added)

#	final_spread  = calculate_spread_mc_sim(graphEnv.graphEnvironment[graphid].graphX, seed_nodes_later, new_node_to_be_added)
	print("earlier " ,seed_nodes_earlier)
	print("later ", seed_nodes_later)
	#initial_spread = calculate_spread_mc_sim(graphEnv.graphEnvironment[graphid].graph_dir, seed_nodes_earlier,"TV")
#	print("before ",initial_spread, seed_nodes_earlier, new_node_to_be_added)

	final_spread  = calculate_spread_mc_sim(graphEnv.graphEnvironment[graphid].graph_dir, seed_nodes_later,"TV")

	#initial_spread = calculate_spread(graphEnv.graphE nvironment[graphid].graphX, seed_nodes_earlier,
	#								  new_node_to_be_added)
	print("before ", previous_spread, seed_nodes_earlier, new_node_to_be_added)

	#final_spread = calculate_spread(graphEnv.graphEnvironment[graphid].graphX, seed_nodes_later, new_node_to_be_added)
	print("after ", final_spread, seed_nodes_later, new_node_to_be_added)
	shortReward = final_spread - previous_spread
	print("sr, final", shortReward, final_spread)
	return shortReward, final_spread

def updateParameters(historyOfTuples):
	nn.trainNeuralNet(historyOfTuples)

def createMuUtil(lastIdx, graphid):
	return nn.createMu(lastIdx, graphEnv.graphEnvironment[graphid])