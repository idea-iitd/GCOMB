import os
import random
#for j in range(0,20):
types =["test"]#,"train","val"]
import pickle
#file = open('interpolate_budget_percentage_real_budget_YT.pkl','rb')
nodes_len_dict= {"large_gowallah":196591,"large_graph_youtube":1134890,"large_orkut":3072441,"large_twt_snap":82000}


dataset_list = ["large_graph_youtube","large_gowallah","large_graph_youtube","large_twt_snap"]#:#,"large_graph_youtube","large_twt_snap"]#:##,"large_graph_youtube"]

for type in types:

    for sampling_neighborhood in [0.75]:#, 0.9,0.1, 0.3, 0.5,0.7]:
        for dataset in dataset_list:
            # for budget in [15,25,50,100,150,200]:#100,50,15,25,150,200]:#[15,25,50,100,150,200]:
            for budget in [20,50,100,150,200]:#100,50,15,25,150,200]:#[15,25,50,100,150,200]:

                file=open('interpolate_budget_percentage_real_budget_{}{}.pkl'.format(dataset, budget), 'rb')
                dict_interpolate=pickle.load(file)

                # for i in [2000,5000, 10000,20000]:#10000, 20000]:#,5000,10000]:
            #    print('Testing on Multiple Graphs =========== ',i)
                #random_ind = 12#random.randint(0,9)
                # command = "sh ./supervisedPredict.sh ./graph_data/graph" + str(i) + "/graph" + str(i)

                # command = "sh ./supervisedPredict.sh ./real_data/large_kite/"+str(type)+"/_bp"
                # os.system(command)
                # # # #
                # command="sh ./supervisedPredict.sh ./real_data/large_gowallah/" + str(type) + "/_bp"
                # os.system(command)
                #  #
                nodes_in_graph=nodes_len_dict[dataset]
                print("nodes in graph", nodes_in_graph)
                use_upto=dict_interpolate(nodes_in_graph)
                print("budget , use upto ", budget, use_upto
                )
                command="sh ./supervisedPredict.sh ./real_data/{}/".format(dataset) + str(type) + "/_bp" +" " + str(budget) +" " + str(use_upto) + " " +str(sampling_neighborhood)
                print(command)
                os.system(command)

            pass
