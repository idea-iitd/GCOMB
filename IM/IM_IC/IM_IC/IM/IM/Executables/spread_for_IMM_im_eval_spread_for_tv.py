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
		"GraphSAGE-master/real_data/youtube/IC/test/large_graph"]

for graph_path in graphs_to_eval:
            for iter in range(0,1):
                for budget in [10,20,50,100,150,200]:

                    #os.chdir(" Executables; ")
                    command ="python eval_anything.py GraphSAGE-master/real_data/youtube/IC/test/" \
                             "large_graph_ic_imm_sol_eps0.5_num_k_{}_iter_{}.txt {} imm_reward_large_graph_ic_imm_sol_eps0.5_num_k_{}_iter_{}.txt test youtube".format(
                        budget, iter, budget, budget, iter,
                    )
                    os.system("cd Executables;pwd; "+ command)
                 #   print(command,"\n")
                  ##  os.system("ls")
