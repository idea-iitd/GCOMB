#
import networkx as nx
import json
from networkx.readwrite import json_graph
import sys

dataset=sys.argv[1]#"large_twt_snap"
train_or_test= "test"
input_file_path =dataset+"/"+train_or_test+"/large_graph-G.json.edge"


edges_out_path = dataset +'/' + train_or_test+'/' +'edges.txt'
print("edges_out_path ", edges_out_path)

file_write = open(edges_out_path ,'w')

id_map ={}
json_id_map_name =dataset +'/' + train_or_test+"/large_graph"+"-id_map.json"

UG = nx.Graph(name=dataset)

node_dic = {}
count = 0
print("json file name ", json_id_map_name)

set_processed = set()
input_file_name = input_file_path
print("input file of edges ", input_file_name
	  )
file = open(input_file_name, 'r')

print (len(set_processed))
print(input_file_name)
# lines = 2000
counter = 0
while True:
	counter+=1

	if counter %1000 ==0:
		print ("completed ", counter , " lines")
	# for line in file_handle.readline():
	line = file.readline()
	if len(line)<2:
		break

	if(line[0]=='#'):
		continue

	edges = line.strip()
	# if len(edges)==0:
	# continue
	edge = edges.split()
	# print(edge)
	source = int(edge[0].rstrip('\x00'))
	target = int(edge[1].rstrip('\x00'))
	if (source ==target):
		continue

	if source not in node_dic:
		node_dic[source] = count
		count = count + 1

	if target not in node_dic:
		node_dic[target] = count
		count = count + 1

	id_map[str(node_dic[source])] = node_dic[source]
	id_map[str(node_dic[target])] = node_dic[target]

	#check_string = str(node_dic[source])+ ' '+str(node_dic[target])
	#if check_string in set_processed:
#		print (check_string)
	#	continue
	#set_processed.add(check_string)

	inf_prob = 0.1  # float(edge[2].rstrip('\x00'))
	file_write.write( str(node_dic[source]) + ' ' + str(node_dic[target] )+'\n')
	#id_map[str(node_dic[source])] = node_dic[source]
	#id_map[str(node_dic[target])] = node_dic[target]

	UG.add_edge(node_dic[source], node_dic[target])



data1 = json_graph.node_link_data(UG)
s1 = json.dumps(data1)
print("dumping to ", dataset +'/' + train_or_test+"/large_graph-G.json")

file1 = open(dataset +'/' + train_or_test+"/large_graph-G.json","w")

file1.write(s1)
file1.close()

#print >>file1, s1
#ith open("train/large_graph-G.json") as f:
#	js_graph = json.load(f)

#H = json_graph.node_link_graph(js_graph)


iddata = json.dumps(id_map)
f2 = open(json_id_map_name, 'w')
f2.write(iddata)
f2.close()
print(json_id_map_name)



















