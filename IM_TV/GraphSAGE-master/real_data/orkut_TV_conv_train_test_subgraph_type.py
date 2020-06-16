#
import networkx as nx
import json
from networkx.readwrite import json_graph
import sys
import numpy as np
from random import sample

dataset="orkut/"
model='TV'
train_or_test= "test"

input_file_path =dataset+'/'+"/"+train_or_test+"/edges.txt"
#input_file_path =dataset+"/"+train_or_test+"/edges.txt"


dataset=dataset+model
edges_out_path = dataset +'/' + train_or_test+'/' +'sorted_edges.txt'
print("edges_out_path ", edges_out_path)

file_write = open(edges_out_path ,'w')

id_map ={}
json_id_map_name =dataset +'/' + train_or_test+"/large_graph"+"-id_map.json"

G = nx.DiGraph(name=dataset)

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

    if counter %10000 ==0:
        print ("completed ", counter , " lines")
        #break
    # for line in file_handle.readline():
    line = file.readline()
    if len(line)<2:
        break

    edges = line.strip()
    # if len(edges)==0:
    # continue
    edge = edges.split()
    # print(edge)
    source = int(edge[0].rstrip('\x00'))
    target = int(edge[1].rstrip('\x00'))
    #weight = float(edge[2].rstrip('\x00'))
    weight_list = [0.1,0.01,0.001]

#	if (source ==target):
#		continue

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

    #inf_prob = 0.1  # float(edge[2].rstrip('\x00'))
    weight = sample(weight_list,1)[0]
    #file_write.write( str(node_dic[source]) + ' ' + str(node_dic[target]) +' ' + str(weight)+'\n')
    #id_map[str(node_dic[source])] = node_dic[source]
    #id_map[str(node_dic[target])] = node_dic[target]

    G.add_edge(node_dic[source], node_dic[target], weight =weight)

    weight = sample(weight_list,1)[0]
    #file_write.write( str(node_dic[target]) + ' ' + str(node_dic[source]) +' ' + str(weight)+'\n')
    #id_map[str(node_dic[source])] = node_dic[source]
    #id_map[str(node_dic[target])] = node_dic[target]

    G.add_edge(node_dic[target], node_dic[source], weight =weight)


degree_of_nodes_all=G.out_degree(weight='weight')
degree_of_nodes_all=[degree for node, degree in degree_of_nodes_all.items() ]

print(" beginning sampling ")
import time
time_sampling_beg=time.time()
total_top_degree_nodes=-1*int(30000)#int(-len(degree_of_nodes_all) * bud_mul_fac)  #

top_degree_nodes=G.nodes()#set(np.argpartition(degree_of_nodes_all, total_top_degree_nodes)[total_top_degree_nodes:])
print(top_degree_nodes)
#
# G_sub= G.subgraph(top_degree_nodes)
# del(G)

G_sub = nx.DiGraph()
node_dic = {}
count=0

edges_dict = {}

alll_edges = G.edges(top_degree_nodes)

#print(alll_edges)
for (n1,n2) in alll_edges:


    #if(n1 in top_degree_nodes or n2 in top_degree_nodes):


        if n1 not in node_dic:
            node_dic[n1] = count
            count = count + 1

        if n2 not in node_dic:
            node_dic[n2] = count
            count = count + 1

   #     weight_list =[0.1,0.01,0.001]

        if((node_dic[n1],node_dic[n2]) not in edges_dict):
            weight = G[n1][n2]['weight']
#            weight = sample(weight_list, 1)[0]

            edges_dict[(node_dic[n1],node_dic[n2])]=1
          #  print((node_dic[n1],node_dic[n2]))
        #    G_sub.add_edge(node_dic[n1], node_dic[n2], weight=weight)
            file_write.write(str(node_dic[n1]) + " " + str(node_dic[n2]) +" "+ str(weight)+"\n")#outfile_top_deg.write(str(n1) +"	"+ str(n2)+"\n")

       # else:
       #     print("already")

        # if ((node_dic[n2], node_dic[n1]) not in edges_dict):
        #     weight = sample(weight_list, 1)[0]
        #     #edges_dict[(node_dic[n2], node_dic[n1])] = 1
        #     edges_dict[(node_dic[n2], node_dic[n1])]=1
        # #    G_sub.add_edge(node_dic[n1], node_dic[n2], weight=weight)
        #     file_write.write(
        #          str(node_dic[n2]) + " " + str(node_dic[n1])+" " +str(weight)+ "\n")  # outfile_top_deg.write(str(n1) +"	"+ str(n2)+"\n")
        #

print(count)
print(len(edges_dict))

#   print(n1,n2)
# pass
#
#
# data1 = json_graph.node_link_data(G_sub)
# s1 = json.dumps(data1)
# print("dumping to ", dataset +'/' + train_or_test+"/large_graph-G.json")
#
# file1 = open(dataset +'/' + train_or_test+"/large_graph-G.json","w")
#
# file1.write(s1)
# file1.close()
#
# #print >>file1, s1
# #ith open("train/large_graph-G.json") as f:
# #	js_graph = json.load(f)
#
# #H = json_graph.node_link_graph(js_graph)
#
#
# iddata = json.dumps(id_map)
# f2 = open(json_id_map_name, 'w')
# f2.write(iddata)
# f2.close()
# print(json_id_map_name)
# #
# # degree_of_nodes = G.out_degree(weight='weight')
#
# all_nodes=nx.nodes(G_sub)
# total_nodes=len(all_nodes)
# print("total nodes", total_nodes)
# features=[]
# # weight_list = [0.1,0.01,0.001]
#
# #   sum_degree = 0
# sum_weight_out=0.0
# #
# # for node_i in all_nodes:
# #     features.append([])
# #  #   sum_degree = sum_degree + degree_of_nodes[node_i]
# #     # sum_weight_in = sum_weight_in + node_weights_incoming[node_i]
# #     sum_weight_out = sum_weight_out + degree_of_nodes[node_i]
# #
# #
# # # print("sum in sum out", sum_weight_in, sum_weight_out)
# # print("sum out",  sum_weight_out)
# #
# # for node_i in all_nodes:
# #     # features[node_i].append(page_rank[node_i])
# #     # features[node_i].append(div_rank[node_i])
# #  #   norm_value = degree_of_nodes[node_i]*1.0/sum_degree
# #
# #     norm_value_out = degree_of_nodes[node_i]*1.0/sum_weight_out
# #     # norm_value_in = node_weights_incoming[node_i]*1.0/sum_weight_in
# #
# # #     features[node_i].append(norm_value)
# #     features[node_i].append(norm_value_out)
# #     # features[node_i].append(norm_value_in)
# #
# # print('Length of the features = ',len(features),total_nodes)
# # #
# #
#
# attribute_file_name=dataset +'/' + train_or_test+'/'  + 'attribute.txt'
# print("attribute ", attribute_file_name)
# attribute_file = open(attribute_file_name,'w')
# attribute_file.write(str(len(G_sub)) +"   " +str(G_sub.number_of_edges()))
# #np.save(feats_file_name, features)
#
#
#
#
#
#
#
#
#
#
#
#




