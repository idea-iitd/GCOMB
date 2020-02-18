import os
import json
from networkx.readwrite import json_graph
import networkx as nx
import numpy as np
from scipy import interpolate
#graph_imm ='real_data/large_youtube_WC/large_graph'
#imm_sol = graph_imm +"_ic_imm_sol_eps"+ str(0.1)  +".txt"

#graph_alb ='graph_data_WC_E6_imm1000/graph80000/graph80000'
#alb_sol = graph_alb +"_imm_sol.txt"

budget_list =[10,20,50,100,150,200]#[x for x in range(10,205,10)]
num_k = None
#budget_list

for num_k  in budget_list[0:]:
    #num_k_list = [ 1,3,5, 10, 20, 50, 100]
    #
  #  size_var=[   5,10,15,20,40,50,60,70,80,90, 99]
    size_var=[   10,20,40,60,80]
    x_axis=[]
    y_axis=[]
    for size in size_var:
        #graph_imm ='twitter_temporal_data/WC/test/large_graph'

        graph_imm='./real_data/youtube/TV/train{}/'.format(size)
    #    graph_imm ='./real_data/test_graphs/graph1000/0/graph0'
        imm_sol=graph_imm + '/multi_iter/large_graph' + "_ic_imm_sol_eps" + str(0.5) + "_num_k_{}_iter_0.txt".format(
            num_k)
        #imm_sol = graph_imm +"_ic_imm_sol_eps"+ str(0.5)  +".txt"
        sup_gs_sol = graph_imm+ "_sup_GS_sol.txt"
        graph=graph_imm + 'large_graph'

        #GraphSAGE-master/real_data/large_kite/train/_bp-G.json.greedy10
        #graph_alb ='graph_data_tri_v_correct_data/graph100000/graph100000'
        #alb_sol = graph_alb +"_imm_sol.txt"

       # graph = graph_imm
        sol = imm_sol

        os.system("pwd")
        solution_file = open(sol, "r")
        solution_file.readline()
        optimal_nodes = solution_file.read().split()
        optimal_nodes = [ int(x.replace('\n','')) for x in optimal_nodes]
       # print(optimal_nodes)
        G_data = json.load(open(graph+ "-G.json"))
        G = json_graph.node_link_graph(G_data)
      #  print("loaded")
        degree_of_nodes = G.degree()


        all_nodes = nx.nodes(G)

        sorted_ids_deg_wt = sorted(range(len(degree_of_nodes)), key=lambda k:degree_of_nodes[k], reverse=True)

        # for index, ids in enumerate(sorted_ids_deg_wt[0:500]):
        #   print("debug" ,index, ids , degree_of_nodes[ids], degree_of_nodes[ids], "\n")

        min_score = 10000000
        max_rankn = -1000
        max_rank_id = -10000
        ranks_list = []

        for sol_id in optimal_nodes:
          #  print("rank of sol", sol_id,"is ", sorted_ids_deg_wt.index(sol_id), "with out_weight ", degree_of_nodes[sol_id],
           #      "degree ", degree_of_nodes[sol_id])
            ranks_list.append(sorted_ids_deg_wt.index(sol_id))
            if degree_of_nodes[sol_id] < min_score:
                min_score = degree_of_nodes[sol_id]
                max_rankn =  max(max_rankn, min(loc for loc, val in enumerate (sorted_ids_deg_wt) if degree_of_nodes[val ]== min_score))#.rindex(sol_id)
                max_rank_id = sol_id

        sorted_ranks_list = sorted(ranks_list)
        print(sorted_ranks_list)
        second_last_rank = sorted_ranks_list[-2]
        print("seconnd last ",second_last_rank)
    #    print("average rank", sum(ranks_list)/len(ranks_list))
        print("budget", num_k,"max rank id", max_rank_id, "max rank ", max_rankn, "min score" ,min_score)

        len_G= len(G)
        print(" nodes in graph", len_G)

        x_axis.append(len_G)
        y_axis.append(int(second_last_rank))

    print(x_axis)
    print(y_axis)
    f_interp= interpolate.interp1d(x_axis,sorted(y_axis), fill_value = "extrapolate")
    print(f_interp(33000))
    print(f_interp([193580,1100000,3100000]))


    #     pass
    #
    # print(y_axis)
    #
    # y_axis = [(y/len(G)) for y in y_axis]
    #
    # print(y_axis)
    # print(np.interp(budget_list,x_axis,y_axis))
    # y_axis = np.interp(budget_list,x_axis,y_axis)
    #
    # interpol_file = open('interpolate_budget_percentage_real.txt','w')
    # interpol_dict ={}
    file_dict = open('interpolate_budget_percentage_real_budget{}.pkl'.format(num_k),'wb')
    # for (x,y) in zip(x_axis, y_axis):
    #     interpol_file.write(str(x) +"\t" + str(y*1.1)+"\n")
    #     interpol_dict[x] =y*1.3
    #     print(x,y*1.3)
    #
    # interpol_file.close()
    import pickle
    pickle.dump(f_interp, file_dict)

    pass