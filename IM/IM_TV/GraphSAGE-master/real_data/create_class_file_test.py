import os
import random
import json
from networkx.readwrite import json_graph
import time
import sys
base_path = "stack/TV/"

graph_path = base_path+'/test/'
print(graph_path)

#G_data = json.load(open( graph_path +"large_graph" + "-G.json"))
#G = json_graph.node_link_graph(G_data)
#stack 2601977

total_nodes = 2601977#23649822#1079712#2601977#len(G.nodes())
print("total nodes ", total_nodes)

class_map_file = graph_path + "large_graph" +"-class_map.json"
class_map = {}
f2 = open(class_map_file, 'w')


for node in range(0, total_nodes):
	class_map[str(node)] = [0]


classdata = json.dumps(class_map)
f2.write(classdata)
f2.close()
