import random
import sys
from networkx.readwrite import json_graph
from networkx.algorithms import bipartite
import json
import networkx as nx
import evaluate
import time
import numpy as np
from scipy import stats
from operator import itemgetter


inputFileName = sys.argv[1]
#k = int(sys.argv[2])
outputFileName = sys.argv[3]
#topKFileName = sys.argv[4]
#rewardFileName = sys.argv[5]
num_itr = 200#100  # Number of times to run greedy
#k_fixed=15
#top_mth_node_for_pairwise = 25
USING_TOP_M_NODES_FOR_GREEDY =2000#max(200, 200+ k)





#multiSolgreedy = sys.argv[6]
#multiSolgreedyNeg = sys.argv[7]

min_per =0.008

print("greedy")
def read_json_file(filename):
	with open(filename) as f:
		js_graph = json.load(f)
	return json_graph.node_link_graph(js_graph)


def calculate_degree(node, neighbors, isCovered):
	degree = 0
	for nbr in neighbors:
		if (isCovered[nbr] == False):
			degree += 1
	return degree


def select_node(node, neighbors, isCovered):
	isCovered[node] = True
	for nbr in neighbors:
		isCovered[nbr] = True

#
# def actual_greedy(main_graph, set_nodes):
#     num_nodes = main_graph.number_of_nodes()
#
#     isCovered = [False for _ in range(0, num_nodes)]
#
#     solution_set = []
#
#     for itr in range(0, k_fixed):
#         denom = 0.0
#
#         gains = [0 for _ in range(0, num_nodes)]
#         for nd in set_nodes:
#             gain = calculate_degree(nd, main_graph.neighbors(nd), isCovered)
#             gains[nd] = gain
#             denom += gain
#
#         if (denom <= 1e-7):
#             print('Graph covered before k steps')
#             break
#
#         selection = -1
#         max_gain = -1
#         for nd in set_nodes:
#             if (gains[nd] >= max_gain and not isCovered[nd]):
#                 selection = nd
#                 max_gain = gains[nd]
#         solution_set.append(selection)
#         select_node(selection, main_graph.neighbors(selection), isCovered)
#     print(" eval ", evaluate.evaluate(main_graph.copy(), solution_set))
#     return solution_set


def run_greedy(main_graph, set_nodes ,gain_sum_for_each_node ):
	global STOP_AT_GAIN
	num_nodes = main_graph.number_of_nodes()

	isCovered = [False for _ in range(0, num_nodes)]
   # res_fixed = []
	res =[]

	#marginal_gains_sum=[0 for _ in range(0, len(set_nodes))]
	for itr in range(0, len(set_nodes)):
		denom = 0.0

		gains = [0 for _ in range(0, len(set_nodes))]
		for nd in set_nodes:
			gain = calculate_degree(nd, main_graph.neighbors(nd), isCovered)
			gains[nd] = gain
			denom += gain
		if (denom <= 1e-7):
			print('Graph covered before k steps')
			break


		norm_gains = [x * 1.0 / denom for x in gains][0:len(set_nodes)]
		selection_samples = np.random.choice(len(set_nodes), 10000, replace=True, p=norm_gains)
		selection = stats.mode(selection_samples)[0][0]
		gain_sum_for_each_node[selection] +=gains[selection]


		select_node(selection, main_graph.neighbors(selection), isCovered)

		print(itr, "gain vec ",selection, gains[selection])
		if(gains[selection] < STOP_AT_GAIN):
			print(" STOP gains, sel", selection, gains[selection])
			break
		else:
			res.append(selection)

#        if(itr<k_fixed):
 #           count_fixed_budget_selected[selection]+=1
  #      res_fixed.append(selection)

	total_gain_of_this_solution =evaluate.evaluate(main_graph.copy(), res)
	#gain_of_fixed_budget  = evaluate.evaluate(main_graph.copy(), res_fixed)

	print("Res ", res)
	#print("Res_fixed", res_fixed)

	print(" eval full", total_gain_of_this_solution)

	#print(" eval budget", gain_of_fixed_budget)
	return total_gain_of_this_solution

main_graph = read_json_file(inputFileName)
set_nodes, element_nodes = bipartite.sets(main_graph)
set_nodes = list(set_nodes)
element_nodes = list(element_nodes)
print(" num of set nodes ", len(set_nodes))
print(" num of element nodes ", len(element_nodes))

STOP_AT_GAIN = int(0.01*len(element_nodes))

print(" STOP AT GAIN ", STOP_AT_GAIN)

left_nodes_sorted_by_degree =sorted(main_graph.degree_iter(),key=itemgetter(1),reverse=True)
top_deg_set_nodes = [x for (x,y) in left_nodes_sorted_by_degree if x in set_nodes][0:USING_TOP_M_NODES_FOR_GREEDY]
print(" top used ", top_deg_set_nodes)
num_nodes = main_graph.number_of_nodes()
#count_selected = [0 for _ in range(0, num_nodes)]
count_fixed_budget_selected = [0 for _ in range(0,num_nodes)]


total_gain_of_all_solutions=0
gain_sum_for_each_node = [0 for x in range(0, num_nodes)]


for itr in range(0, num_itr):
	print("iter ", itr)
	print("total gain ", total_gain_of_all_solutions)
	gain_of_all_solutions = run_greedy(main_graph, set_nodes, gain_sum_for_each_node )
	total_gain_of_all_solutions+=gain_of_all_solutions


ratio_for_each_node = [0 for x in range(0, num_nodes)]

outFile = open(outputFileName, 'w')
for node in range(0, num_nodes):
	#print(node,gain_sum_for_each_node[node]*1.0 ,(total_gain_of_all_solutions))
	ratio_for_each_node[node] = gain_sum_for_each_node[node]*1.0/(total_gain_of_all_solutions)
	outFile.write(str(ratio_for_each_node[node]) + ' ')
outFile.close()

outFile2 = open(outputFileName + "analysis", 'w')
for node in range(0, num_nodes):
	outFile2.write(str(node) + " " + str(ratio_for_each_node[node]) + ' \n')
outFile2.close()
print("done greedy")
#
# import numpy as np
#
# start_time = time.time()
# topk = actual_greedy(main_graph, set_nodes)
# end_time = time.time()
# time_elapsed = end_time - start_time
# topKFile = open(topKFileName, 'w')
# topKFile.write(str(len(topk)) + '\n')
# for node in topk:
# 	topKFile.write(str(node) + ' ')
# 	print(node , top_deg_set_nodes.index(node))
# topKFile.close()
#
# rewardFile = open(rewardFileName, 'w')
# rewardFile.write(str(evaluate.evaluate(main_graph, topk)))
# rewardFile.write('\n' + str(time_elapsed))
# rewardFile.close()
#
#
#
#
