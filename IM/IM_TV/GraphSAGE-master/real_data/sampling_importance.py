#
import sys
import random
import numpy as np
import networkx as nx
from networkx.readwrite import json_graph

import json
import pickle

dataset="twitter/TV/test/"

full_graph_edge_file_path = dataset+"/edges.txt"


id_map ={}
json_id_map_name =dataset +'/' +"/large_graph"+"-id_map.json"


G = nx.DiGraph(name=dataset)

node_dic = {}



for size_var in [5]:

    test_sub_graph_edge_file_path = dataset+"/sampled_edges.txt".format(size_var)


    f_full_graph_edge = open(full_graph_edge_file_path,'r')
    f_test_sub_graph_edge = open(test_sub_graph_edge_file_path,'w')

    edges_weights = []
    edges_list = []
    #
    counter = 0
    #
    # dict_edge_processed = {}

    while True:
        #for line in file_handle.readline():
        line = f_full_graph_edge.readline()
       # lines-=1
     #   if lines <0:
      #      break
        if not line:
            break
        #print(line)
        n1, n2, wt= line.replace('\n','').split(' ')
        wt= float(wt)
        n1=int(n1)
        n2=int(n2)
        edges_weights.append(wt)
        edges_list.append(tuple((n1,n2,wt)))
        counter+=1

        if(counter%5000==0):
            print(counter)
#            break

   # print(edges_weights)
    edges_weights = np.array(edges_weights)
    edges_list = np.array(edges_list)
 #   print(edges_list)
    edges_weights=edges_weights / sum(edges_weights)
   # print(len(edges_list), len(edges_weights))
    index_sampled=np.random.choice(len(edges_list), int(size_var*0.01*counter),
                                   replace=False, p=edges_weights).tolist()
  #  print(index_sampled)
    edges_sampled_list=edges_list[index_sampled].tolist()
    print("edges asmpled ", edges_sampled_list)
    print("num edges ", len(edges_sampled_list))
    #  print("edges asmpled ", edges_sampled.tolist())


    mapped_samped_to_sorted ={}

    count=0
    for (source, target, weight) in edges_sampled_list:

        if source not in node_dic:
            node_dic[source] = count
            count = count + 1

        if target not in node_dic:
            node_dic[target] = count
            count = count + 1

        id_map[str(node_dic[source])] = node_dic[source]
        id_map[str(node_dic[target])] = node_dic[target]

        G.add_edge(node_dic[source], node_dic[target], weight=weight)
    #    print(node_dic[source], node_dic[target],weight)
        mapped_samped_to_sorted[node_dic[source]] = source
        mapped_samped_to_sorted[node_dic[target]]=target

    print("num nodes ", count)

    data1=json_graph.node_link_data(G)
    s1=json.dumps(data1)
    print("dumping to ", dataset + '/'  + "/large_graph-G.json")

    file1=open(dataset + '/' + "/large_graph-G.json", "w")

    file1.write(s1)
    file1.close()

    # print >>file1, s1
    # ith open("train/large_graph-G.json") as f:
    #	js_graph = json.load(f)

    # H = json_graph.node_link_graph(js_graph)

    iddata=json.dumps(id_map)
    f2=open(json_id_map_name, 'w')
    f2.write(iddata)
    f2.close()
    print(json_id_map_name)

    class_map_file=dataset + "large_graph" + "-class_map.json"
    class_map={}
    f2=open(class_map_file, 'w')

    for node in range(0, count):
        class_map[str(node)]=[0]

    classdata=json.dumps(class_map)
    f2.write(classdata)
    f2.close()

    map_file=dataset + "large_graph" + "-mapped.pkl"

    map=open(map_file, 'wb')

    pickle.dump(mapped_samped_to_sorted
                , map, protocol=pickle.HIGHEST_PROTOCOL)
