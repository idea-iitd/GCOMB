import os
import random
import evaluate
import json
from networkx.readwrite import json_graph
import time
import sys
import networkx as nx


num_k = 500
model = sys.argv[1]
base_path = sys.argv[2]
train_or_test = sys.argv[3]
graph_path = base_path + '/' + model + '/' + train_or_test + '/'
print(graph_path)

G_data = json.load(open(graph_path + "large_graph" + "-G.json"))
G = json_graph.node_link_graph(G_data)
total_nodes = len(G)

epsilon = 0.5

#epsilon = 0.5
print(os.getcwd())
print(graph_path)

#total_nodes = len(G.nodes())
#print("total nodes ", total_nodes)

dataset_path = "../../../../../" +  graph_path +"edges.txt"
print(dataset_path)

int_selected_nodes = []

num_of_iterations_imm = 25
appeared_count = {}
max_appear_count = 0
print(os.getcwd())
class_map_file =  graph_path + "large_graph" +"_num_k_"+str(num_k)+"-class_map.json"
class_map = {}
f2 = open(class_map_file, 'w')


dict_marginal_gain = {}


for iter in range(0, num_of_iterations_imm):


	print(" imm iteratin for class map new #", iter)

	out_file_path =  graph_path +'/multi_iter/'+ "large_graph_ic_imm_sol_eps" + str(epsilon) + "_num_k_" + str(
		num_k) +"_iter_"+str(iter)+"_dict_node_gain"

	print(out_file_path)

	solution_file_name = out_file_path + ".txt"
	print(solution_file_name)

	solution_file = open(solution_file_name, "r")

	optimal_nodes_gain = solution_file.readlines()
	for line in optimal_nodes_gain:
		line=line.replace("\n","")
		node,gain= line.split(" ")
		node,gain = int(node), int(gain)
		if node not in dict_marginal_gain:
			dict_marginal_gain[node] = 0
		
		dict_marginal_gain[node] += gain

	pass

sum_of_marginal_gains = 0


for node, marg_gain in dict_marginal_gain.items():
	print("node marg_gain ", node, marg_gain)

	sum_of_marginal_gains+= marg_gain
print("sum of marginal gains ", sum_of_marginal_gains)
dict_marginal_gain_normalized = {}

for node, marg_gain in dict_marginal_gain.items():
	dict_marginal_gain_normalized[node] = [marg_gain*1.0/sum_of_marginal_gains]


print("dict_marginal_gain_normalized" , dict_marginal_gain_normalized)
for node in range(0, total_nodes):
	# if node in int_selected_nodes:
	if node not in dict_marginal_gain_normalized:
		dict_marginal_gain_normalized[str(node)] = [0]
classdata = json.dumps(dict_marginal_gain_normalized)
f2.write(classdata)
f2.close()


	#int_selected_nodes = []
#
# 	for i in range(0, num_k):
# 		int_selected_nodes.append(int(optimal_nodes[i]))
# 		if int(optimal_nodes[i]) not in appeared_count:
# 			appeared_count[int(optimal_nodes[i])] = 0
#
# 		appeared_count[int(optimal_nodes[i])] += 1
#
# 		if appeared_count[int(optimal_nodes[i])] > max_appear_count:
# 			max_appear_count = appeared_count[int(optimal_nodes[i])]
#
# # print( appeared_count)
#
# appeared_count_normalized = {}
# for node_id, node_freq in appeared_count.items():
# 	appeared_count_normalized[node_id] = node_freq / (max_appear_count * 1.0)
# #  print( appeared_count_normalized)
#
# os.chdir("../../../../../")
#
# # print(appeared_count_normalized)
# print("Writing  to class map ")
# print(appeared_count_normalized)
#
# for node in range(0, total_nodes):
# 	# if node in int_selected_nodes:
# 	if node not in appeared_count_normalized:
# 		class_map[str(node)] = [0]
# 	else:
# 		class_map[str(node)] = [appeared_count_normalized[node]]  # [1,0]
#
#
# classdata = json.dumps(class_map)
# f2.write(classdata)
# f2.close()
#
#
# # else:
# ##     3     class_map[str(node)] = # [0,1]
# # print(class_map)
#
# #	num_mc_simulation = 100
#
# #	spread = 0
#	print(" loading graph")
#	G_data = json.load(open("GraphSAGE-master/real_data/large_youtube/large_graph" + "-G.json"))
#	G = json_graph.node_link_graph(G_data)
#	print(" running mc simulation")
#	for i in range(0, num_mc_simulation):
#		UG_Copy = G.copy()
#		temp_spread = evaluate.evaluate(UG_Copy, int_selected_nodes)
#		print(" iter {} spread {}".format(i, temp_spread))
#		spread = spread + temp_spread

#	spread = spread * 1.0 / num_mc_simulation
#	print('Spread = ', spread)
#	reward_file_name = "GraphSAGE-master/real_data/large_youtube/large_graph"+"_reward_imm_eps"+ str(epsilon)
#	reward_file = open(reward_file_name, 'w')
#	reward_file.write(str(spread))
#	reward_file.close()
