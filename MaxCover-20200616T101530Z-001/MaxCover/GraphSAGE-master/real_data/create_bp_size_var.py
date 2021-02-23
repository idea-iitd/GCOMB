import os
#
import networkx as nx
import json
from networkx.readwrite import json_graph
import sys
import networkx as nx
from networkx.exception import NetworkXError
from networkx.utils import not_implemented_for
import json
from networkx.readwrite import json_graph
import numpy as np
from networkx.algorithms import bipartite
from pathlib import Path
import sys
import random
from random import shuffle
import os

def gen_setcover_inst(G):

    constant_add = len(G)
    print(" const add ", constant_add)
    node_list_A= list(set(G.nodes()))
    node_list_B=[x + constant_add  for x in list(set(G.nodes())) ]

    g=nx.Graph()
    g.add_nodes_from(node_list_A, bipartite=0)
    g.add_nodes_from(node_list_B, bipartite=1)

 #   for i in range(total_nodes):
    for (v1,v2) in G.edges():
        if(v1==v2):
            continue
        g.add_edge(v1,v2+constant_add)
        g.add_edge(v1+ constant_add , v2)
   # g.remove_nodes_from(list(nx.isolates(g)))

    set_nodes, element_nodes=bipartite.sets(g)
    set_nodes=list(set_nodes)
    element_nodes=list(element_nodes)
    print(len(set_nodes), len(element_nodes))
    return g


#num_k=15#400

dataset=sys.argv[1]#"large_twt_snap"#""gowallah_bfs"#"large_graph_youtube"
train_or_test= "train"

#size_var =sys.argv[2]
for size_var in [50,80,90,99]:
    #train_or_test+=str(size_var)
    print("size_var ", size_var)
    input_file_path =dataset+"/"+train_or_test+"{}/large_graph-G.json".format(size_var)
    dir_name=""

    graph_name = dir_name+dataset+"/"+train_or_test+"{}/".format(size_var)+"_bp"
    print(graph_name)
    G_data = json.load(open( input_file_path ))
    G = json_graph.node_link_graph(G_data)

    G=gen_setcover_inst(G)
    #
    # # page_rank = nx.pagerank(UG)
    # # div_rank = divrank(UG)

    degree_of_nodes=G.degree()

    all_nodes=nx.nodes(G)

    total_nodes = len(all_nodes)
    print(" total nodes ", total_nodes)
    features=[]

    sum_degree=0
    #
    # for node_i in all_nodes:
    #     features.append([])
    #     sum_degree=sum_degree + degree_of_nodes[node_i]
    #
    # for node_i in all_nodes:
    #     # features[node_i].append(page_rank[node_i])
    #     # features[node_i].append(div_rank[node_i])
    #     norm_value=degree_of_nodes[node_i] * 1.0 / sum_degree
    #     features[node_i].append(norm_value)

    validation_set=(0.99 * total_nodes)
    test_set=(0.99 * total_nodes)
    random_list=[i for i in range(total_nodes)]
    #shuffle(random_list)

    for node in range(0, total_nodes):
        if node < validation_set:
            G.node[random_list[node]]['val']=False
            G.node[random_list[node]]['test']=False
        elif node < test_set:
            G.node[random_list[node]]['val']=True
            G.node[random_list[node]]['test']=False
        else:
            G.node[random_list[node]]['val']=False
            G.node[random_list[node]]['test']=True

    nx.set_edge_attributes(G, 'test_removed', False)
    nx.set_edge_attributes(G, 'train_removed', False)

    json_graph_name=graph_name + "-G.json"
    json_id_map_name=graph_name + "-id_map.json"
    feats_file_name=graph_name + "-feats.npy"
    #
    #np.save(feats_file_name, features)
    data=json_graph.node_link_data(G)
    graphjson=json.dumps(data)
    f1=open(json_graph_name, 'w')
    f1.write(graphjson)
    f1.close()

    #exit()


    print("here")
    id_map={}
    #
    for node in range(0, total_nodes):
        id_map[str(node)]=node

    iddata=json.dumps(id_map)
    f2=open(json_id_map_name, 'w')
    f2.write(iddata)
    f2.close()

    nodes=[n for n in G.nodes() if not G.node[n]["val"] and not G.node[n]["test"]]
    G=G#.subgraph(nodes)
    #
    class_map_file=graph_name + "-class_map.json"

    class_map={}
    #
    # os.chdir("../../greedy_baseline")
    #
    # graph_file_name=json_graph_name
    #
    # command="sh ./find_greedy.sh " + graph_file_name + " " + str(num_k)
    # print(command)
    # os.system(command)
    #
    # solution_file_name=graph_file_name + ".greedySol"
    #
    # solution_file=open(solution_file_name, "r")
    # #os.chdir("../")
    # greedy_nodes=solution_file.readlines()
    # temp_selected_nodes=greedy_nodes[0].strip().split(' ')
    #
    # os.system("pwd")
    # os.chdir("../random_baseline")
    #
    # graph_file_name=  json_graph_name
    #
    # command="sh ./find_random.sh " + graph_file_name + " " + str(num_k)
    #
    # os.system(command)
    # os.system("pwd")
    #
    # #os.chdir("../")
    #
    # os.chdir("../top-k_baseline")
    #
    # graph_file_name= json_graph_name
    #
    # command="sh ./find_top-k.sh " + graph_file_name + " " + str(num_k)
    #
    # os.system(command)
    # os.chdir("../")
    # #
    for node in range(0, total_nodes):
        class_map[str(node)]=[0]#[float(temp_selected_nodes[node])]

    classdata=json.dumps(class_map)
    f2=open(class_map_file, 'w')
    f2.write(classdata)
    f2.close()
