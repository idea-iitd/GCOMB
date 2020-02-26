#
from networkx.readwrite import json_graph

#
import os
import sys
model_list = ["IC"]
#model ="WC"
#k_list=[10,20,50,100, 150,200]
dataset_list= ["youtube"]#,"orkut"]
sol_file_line_wise= sys.argv[1]
budget = sys.argv[2]
temp_reward_file=  sys.argv[3]

for dataset in dataset_list:
    for model in model_list:
        graph_path = "../GraphSAGE-master/real_data/" + dataset + "/" + model + "/train/"
        edges_file_path = graph_path+"sorted_edges.txt"

        graph_path= graph_path+"large_graph"

        command = "bash ./eval_spread_im.sh " + dataset + " " + model + " " + str(budget) + " " + edges_file_path + " " + sol_file_line_wise + " " + temp_reward_file
        print(command)
        os.system(command)
        pass