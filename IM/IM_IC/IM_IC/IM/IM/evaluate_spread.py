import networkx as nx
import random
from functools import partial
import time
import json
import multiprocessing as mp
from networkx.readwrite import  json_graph


def calculate_spread(sampled_G, seed_nodes,new_node):
    beg_time = time.time()
    if len(seed_nodes)==0:
        return 0
    for node in seed_nodes:
        sampled_G.add_edge(new_node,node,weight=1.0)

    total_nodes = len(nx.descendants(sampled_G,new_node))
    sampled_G.remove_node(new_node)
    end_time = time.time()
    print("spread time ", end_time- beg_time)
    return total_nodes


def evaluate(sampled_G, num_nodes, selected_nodes):
#	num_nodes = main_graph.number_of_nodes()
    new_node_to_be_added = num_nodes + 1
    spread = calculate_spread(sampled_G, selected_nodes,new_node_to_be_added)
    print(" graph spread ", spread)
    #del main_graph
    return spread
#
def mp_pool_format(mc_sim_graphs_dir, num_nodes , selected_nodes, mc_id ):
    print("selected_nodes", selected_nodes)
    sampled_graph_json_path = mc_sim_graphs_dir + str(mc_id) +"-G.json"
    print("sampled_graph_json_path ", sampled_graph_json_path )
    sampled_G_data = json.load(open(sampled_graph_json_path))
    sampled_G = json_graph.node_link_graph(sampled_G_data)
    num_nodes=999999999999

    print("loaded",num_nodes)
    spread = evaluate(sampled_G, num_nodes, selected_nodes)
    print("SPREAD VAL", spread, sampled_graph_json_path)
    return spread


def evaluate_helper_mp(graph_dir, main_graph, selected_nodes, num_mc_sim):

    mc_sim_graphs_dir = graph_dir+'/mc_sim_graphs/'
    print(" mc_sim_graphs dir ", mc_sim_graphs_dir  )
    result_list = []
    mc_sim = [x for x in range(0, num_mc_sim)]
    print(mc_sim)
    pool = mp.Pool(processes=4)
    pool_args = partial(mp_pool_format,mc_sim_graphs_dir, main_graph, selected_nodes)
    print(pool_args)
    for iter, res in enumerate(pool.map(pool_args,mc_sim, chunksize=25
                                        )):
        print("iter, res", iter, res)
        result_list.append(res)
        avg_spread = round(sum(result_list) * 1.0 / len(result_list) * 1.0, 4)
        print("len result list", len(result_list))
        print("Avg_spread", avg_spread)
    print("final results")
    avg_spread = round(sum(result_list) * 1.0 / len(result_list) * 1.0, 4)
    print("len result list", len(result_list))
    print("Avg_spread", avg_spread)
    return avg_spread


def evaluate_helper_without_mp(graph_dir, main_graph, selected_nodes, num_mc_sim):

    mc_sim_graphs_dir = graph_dir+'/mc_sim_graphs/'
    print(" mc_sim_graphs dir ", mc_sim_graphs_dir  )
    result_list = []
    mc_sim = [x for x in range(0, num_mc_sim)]
    num_nodes = 999999999999
    for sim in mc_sim:
        res = mp_pool_format(mc_sim_graphs_dir, num_nodes, selected_nodes, sim)
        result_list.append(res)
        avg_spread = round(sum(result_list) * 1.0 / len(result_list) * 1.0, 4)
        print("len result list", len(result_list))
        print("Avg_spread", avg_spread)
    # for iter, res in enumerate(pool.map(pool_args,mc_sim, chunksize=1)):
    # 	print("iter, res", iter, res)
    # 	result_list.append(res)
    # 	avg_spread = round(sum(result_list) * 1.0 / len(result_list) * 1.0, 4)
    # 	print("len result list", len(result_list))
    # 	print("Avg_spread", avg_spread)
    print("final results")
    avg_spread = round(sum(result_list) * 1.0 / len(result_list) * 1.0, 4)
    print("len result list", len(result_list))
    print("Avg_spread", avg_spread)
    return avg_spread
