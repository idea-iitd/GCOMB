#
from networkx.readwrite import json_graph

#
import os
import sys
model_list = ["IC"]
#model ="WC"
#k_list=[10,20,50,100, 150,200]
sol_file_line_wise= sys.argv[1]
budget = sys.argv[2]
reward_file = sys.argv[3]
train_test =sys.argv[4] +"/"
dataset= sys.argv[5]#,"orkut"]

for i in range(0,1):
    for model in model_list:
        graph_path = "../GraphSAGE-master/real_data/" + dataset + "/" + model + "/"+train_test
        edges_file_path = graph_path+"sorted_edges.txt"

        graph_path= graph_path+"large_graph"

        command = "bash ./eval_anything.sh " + dataset + " " + model + " " + str(budget) + " " + edges_file_path + " " + sol_file_line_wise + " " + reward_file+" "+str(train_test)
        print(command)
        os.system(command)
        pass