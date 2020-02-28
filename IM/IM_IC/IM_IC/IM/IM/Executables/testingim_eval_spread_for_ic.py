# import os
# import random
# #import evaluate
# import json
# from networkx.readwrite import json_graph
# import time
# from networkx.readwrite import  json_graph
# #import evaluate_spread
# import sys


import os
# import random
# #import evaluate
# import json
# from networkx.readwrite import json_graph
# import time
# from networkx.readwrite import  json_graph
# #import evaluate_spread
# import sys

num_mc_simulation = 10000
graphs_to_eval = [

		#   "GraphSAGE-master/real_data/large_youtube2/large_graph",
		#"GraphSAGE-master/real_data/large_youtube/large_graph"]
		"GraphSAGE-master/real_data/youtube/IC/test/large_graph"]

for graph_path in graphs_to_eval:
    for sampling_freq in [0.05]:
                for budget in [10,20,50,100,150,200]:#,20,50,100,150,200]:
                    graph_rl_file=graph_path + "-result_RL_{}_nbs_{}".format(budget, sampling_freq)

                    #os.chdir(" Executables; ")
                    command ="python eval_anything.py " +'../'+graph_rl_file+ " {} rl_reward_num_k_{}_nbs_{}.txt test youtube".format(
                        budget,budget,sampling_freq
                    )
                    print(command+"\n")
                    os.system("cd Executables;pwd; "+ command)
                 #   print(command,"\n")
                  ##  os.system("ls")


