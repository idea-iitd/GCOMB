import os
import random
import evaluate
import json
from networkx.readwrite import json_graph
import time
from networkx.readwrite import  json_graph
import evaluate_spread
import sys

num_mc_simulation = 1000
graphs_to_eval = [

		#   "GraphSAGE-master/real_data/large_youtube2/large_graph",
		#"GraphSAGE-master/real_data/large_youtube/large_graph"]
		"GraphSAGE-master/real_data/youtube/TV/test/large_graph"]

for graph_path in graphs_to_eval:
		for sampling_freq in[0.003]:#[0.009]:# [0.0005,0.001,0.003, 0.005,0.007,0.010,0.05,0.1]:

			for budget in [200,150,100,50,20,10]:#[10,20,50,100,150,200]:
				try:
					print(sampling_freq)
					print(budget)
					graph_rl_file = graph_path+"-result_RL_{}_nbs_{}".format(budget, sampling_freq)
					print(" graph", graph_path)
					solution_file = open(graph_rl_file, "r")

					optimal_nodes = solution_file.readlines()
					int_selected_nodes = []

					for node_i in optimal_nodes:#range(0, budget):
						int_selected_nodes.append(int(node_i))
					# print( appeared_count)


					spread = 0
					print(" loading graph")
					graph_json_path = graph_path + "-G.json"
					# G_data = json.load(open(graph_json_path))

					# G = json_graph.node_link_graph(G_data)

					print("caculating spread, num of sim", num_mc_simulation)
					# for i in range(0, num_mc_simulation):
					#     G_Copy = G.copy()

					#spread = evaluate_spread.evaluate_helper_without_mp(graph_path, None, int_selected_nodes, num_mc_simulation)
					spread = evaluate_spread.evaluate_helper_mp(graph_path, None, int_selected_nodes, num_mc_simulation)
					# print(" iter {}  spread {}".format(i, temp_spread))
					# spread = spread + temp_spread
					# spread = spread * 1.0 / num_mc_simulation
					print('Average Spread = ', spread)

					print('Spread = ', spread)
					reward_file_name = graph_path+"_reward_RL_budget_"+str(budget)+"_nbs_"+str(sampling_freq)
					print(reward_file_name)
					print(reward_file_name)
					reward_file = open(reward_file_name, 'w')
					reward_file.write(str(spread) )
					reward_file.close()
				except :
					pass
