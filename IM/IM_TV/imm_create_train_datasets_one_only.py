import os
import random
import evaluate
import json
from networkx.readwrite import json_graph
import time
import sys
import pickle

model = sys.argv[1]
base_path = sys.argv[2]
train_or_test = sys.argv[3]

num_k=10#500#

graph_path = base_path + '/' + model + '/' + train_or_test + '/'
print(graph_path)

G_data = json.load(open(graph_path + "large_graph" + "-G.json"))
G = json_graph.node_link_graph(G_data)
total_nodes = len(G.nodes())

epsilon = 0.5
#total_nodes = 1079712
print(" total nodes ", total_nodes)
#epsilon = 0.5
print(os.getcwd())
print(graph_path)

os.chdir("./imm_baseline/im_benchmarking-master/sidm029_im_benchmark/Codes/IMM")

#total_nodes = len(G.nodes())
#print("total nodes ", total_nodes)

dataset_path = "../../../../../" +  graph_path +"sorted_edges.txt"
print(dataset_path)



num_of_iterations_imm =1#30
print(os.getcwd())
class_map_file = "../../../../../" + graph_path + "large_graph" +"-class_map.json"
class_map = {}
f2 = open(class_map_file, 'w')
#for num_k in [5,10,15,20,25,30,50,100,200]:
for num_k in [10,20,50,100,150,200]:
	int_selected_nodes=[]

	gain_dict={}
	total_gain = 0
	print(" GENERATING IMM REG ")
	for iter in range(0, num_of_iterations_imm):
		print(" imm iteratin #", iter)
		seed_random = random.randint(1, 100000000)
		out_file_path = "../../../../../" + graph_path +'/multi_iter/'+ "large_graph_ic_imm_sol_eps" + str(epsilon) + "_num_k_" + str(
			num_k) +"_iter_"+str(iter)

		print(out_file_path)
		result_time_file_name = "../../../../../" + graph_path + '/multi_iter/'+ "large_graph_ic_imm_time_eps" + str(
			epsilon) + "_num_k_" + str(num_k) +"_iter_"+str(iter) + ".txt"
		print(result_time_file_name)
		solution_file_name = out_file_path + ".txt"
		print(solution_file_name)

		#
		command = "time ./imm_discrete -dataset " + dataset_path + " -k " + str(
			num_k) + " -model IC -epsilon " + str(epsilon)+" -output " + out_file_path + " -seed_random " + str(seed_random)+ " -training_for_gain 1"
		print("command imm ", command)
		os.system(command)

		solution_file_name = out_file_path + ".txt"
		file_node_gain_name = out_file_path + "_dict_node_gain.txt"
		file_node_gain = open(file_node_gain_name,'r')
	#	solution_file = open(solution_file_name, "r")
    #
	# #	optimal_nodes = solution_file.readlines()
	# 	#print("optimal nodes ", optimal_nodes)
	# 	#print("len optimal nodes ", len(optimal_nodes))
	# 	pass
	# 	node_gain_list_tuples = file_node_gain.readlines()
	# 	for line in node_gain_list_tuples:
	# 		(node,gain ) = line.split()
	# 		gain= int(gain)
	# 		node = int(node)
	# 		total_gain+= int(gain)
	# 		if( node not in gain_dict):
	# 			gain_dict[node] = 0
	# 		gain_dict[node] += gain
    #
	# 		print(node, gain)
    #
    #
    #
    #
	# for node in range(0, total_nodes):
	# 	node = int(node)
	# 	# if node in int_selected_nodes:
	# 	if node not in gain_dict:
	# 		class_map[str(node)] = [0]
	# 	else:
	# 		class_map[str(node)] = [gain_dict[node]*1.0/(total_gain*1.0)]  # [1,0]
    #
	# classdata = json.dumps(class_map)
	# f2.write(classdata)
	# f2.close()
	# print(class_map['283812'])
	# #
	# # print(class_map['18718'])
	# print(dict)
		# for node in optimal_nodes:
		# 	int_node = int(node)
		# 	if int_node not in dict_togetherness:
		# 		dict_togetherness[int_node] ={}
		#
		# 	for node_dict in optimal_nodes:
		# 		int_node_dict = int(node_dict)
		#
		# 		if(int_node_dict == int_node):
		# 			continue
		#
		# 		if int_node_dict not in dict_togetherness[int_node]:
		# 			dict_togetherness[int_node][int_node_dict]=0
		#
		#
		# 		dict_togetherness[int_node][int_node_dict]+=1

	#	print(dict_togetherness)

	#

	print("len dict", len(gain_dict))

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
