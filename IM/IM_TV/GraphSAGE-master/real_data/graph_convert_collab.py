import networkx as nx
import json
from networkx.readwrite import json_graph
import sys

fileHandle = open(sys.argv[1])
fileData = fileHandle.readlines()

data_stats = fileData[0].split('\t')

total_nodes = int(data_stats[0])
total_edges = int(data_stats[1])

# print("total_nodes = ",total_nodes)
# print("total_edges = ",total_edges)

UG = nx.Graph(name="collab_data")
SG = nx.Graph(name="sampled_collab_data")
visited = []
for node in range(0,total_nodes):
	UG.add_node(node)
	visited.append(0)

total_lines = len(fileData)
dict_node_mapping = {}
count_node = 0
# print(total_lines)
for i in range(1,total_lines):
	edges = fileData[i].strip()
	# if len(edges)==0:
		# continue
	edge = edges.split('\t')
	# print(edge)
	source = int(edge[0].rstrip('\x00'))
	target = int(edge[1].rstrip('\x00'))
	# if source>=total_nodes or target>=total_nodes:
	# 	continue
	if source not in dict_node_mapping.keys():
		dict_node_mapping[source] = count_node
		count_node = count_node + 1

	if target not in dict_node_mapping.keys():
		dict_node_mapping[target] = count_node
		count_node = count_node + 1

	UG.add_edge(dict_node_mapping[source],dict_node_mapping[target])


print("starting -------bfs-------")
start_vertex = 0
num_vertices = 1
vertices_list = [start_vertex]
mapping = {}
count = 0
while len(vertices_list)!=0 and num_vertices<100:
	vertex_opened = vertices_list.pop(0)
	if visited[vertex_opened]==1:
		continue
	new_neighbors = UG.neighbors(vertex_opened)
	print(num_vertices, vertex_opened, len(new_neighbors))
	visited[vertex_opened] = 1
	num_vertices = num_vertices + 1
	if vertex_opened not in mapping.keys():
		mapping[vertex_opened] = count
		count = count + 1
	SG.add_node(mapping[vertex_opened])
	for nbr in new_neighbors:
		if nbr not in mapping.keys():
			mapping[nbr] = count
			count = count + 1
		SG.add_node(mapping[nbr])
		SG.add_edge(mapping[vertex_opened],mapping[nbr])
	vertices_list.extend(new_neighbors)

print("bfs-----------end")
data1 = json_graph.node_link_data(UG)
s1 = json.dumps(data1)
file1 = open("large_graph.json","w")
print >>file1, s1

data2 = json_graph.node_link_data(SG)
s2 = json.dumps(data2)
file2 = open("small_graph.json","w")
print >>file2, s2


print("total_nodes = ",len(SG.nodes()))