import sys
from networkx.readwrite import json_graph
import json
import networkx as nx

inputFile = sys.argv[1]

filehandle = open("graph_in_edge_format.txt","w")

def read_json_file(filename):
	with open(filename) as f:
		js_graph = json.load(f)
	return json_graph.node_link_graph(js_graph)

# graph in network x format
main_graph = read_json_file(inputFile)
adj = main_graph.adjacency_list()

print(main_graph.number_of_nodes())

v = 0
for nbrs in adj:
	for nbr in nbrs:
		filehandle.write(v, nbr)
	v = v + 1

filehandle.write("-1", "-1")
