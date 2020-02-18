import os
import random
import evaluate
import json
from networkx.readwrite import json_graph
import time
from networkx.readwrite import  json_graph
import evaluate_spread
import sys

num_mc_simulation = 10000
graphs_to_eval = [

		#   "GraphSAGE-master/real_data/large_youtube2/large_graph",
		#"GraphSAGE-master/real_data/large_youtube/large_graph"]
		"GraphSAGE-master/real_data/youtube/TV/test/large_graph"]

for graph_path in graphs_to_eval:
            for iter in range(1,5):
                for budget in [1]:
                    try:

                        print(budget)
                        graph_imm_file = graph_path+"_ic_imm_sol_eps0.5_num_k_{}_iter_{}.txt".format(budget, iter)
                        print(" graph", graph_imm_file)
                        solution_file = open(graph_imm_file, "r")

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

                        spread = evaluate_spread.evaluate_helper_without_mp(graph_path, None, int_selected_nodes, num_mc_simulation)
                        # print(" iter {}  spread {}".format(i, temp_spread))
                        # spread = spread + temp_spread
                        # spread = spread * 1.0 / num_mc_simulation
                        print('Average Spread = ', spread)

                        print('Spread = ', spread)
                        reward_file_name = graph_path+"_reward_imm_{}_budget_{}".format(iter,budget)
                        print(reward_file_name)
                        print(reward_file_name)
                        reward_file = open(reward_file_name, 'w')
                        reward_file.write(str(spread) )
                        reward_file.close()
                    except :
                        pass
