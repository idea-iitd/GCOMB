import os
import random
#for j in range(0,20):
types =["test"]#,"train","val"]
import pickle
file = open('interpolate_budget_percentage_real.pkl','rb')
nodes_len_dict= {"large_gowallah":196591,"large_graph_youtube":1134890,"large_orkut":3072441}


dataset_list = ["gowallah_bfs"]#,"large_graph_youtube"]

for type in types:
    for i in range(5,6):
        for sampling_neighborhood in [0.999]:#, 0.9,0.1, 0.3, 0.5,0.7]:
            for dataset in dataset_list:
                for budget in [2,4,5,6,8,10,12,15,20,25,30,35,40,50]:#[15,25,50,100,150,200]:#100,50,15,25,150,200]:#[15,25,50,100,150,200]:

                    file=open('interpolate_budget_percentage_real_budget10.pkl'.format(budget), 'rb')
                    dict_interpolate=pickle.load(file)

                    # for i in [2000,5000, 10000,20000]:#10000, 20000]:#,5000,10000]:
                    print('Training on Multiple Graphs =========== ',i)
                    #random_ind = 12#random.randint(0,9)
                    # command = "sh ./supervisedPredict.sh ./graph_data/graph" + str(i) + "/graph" + str(i)

                    # command = "sh ./supervisedPredict.sh ./real_data/large_kite/"+str(type)+"/_bp"
                    # os.system(command)
                    # # # #
                    # command="sh ./supervisedPredict.sh ./real_data/large_gowallah/" + str(type) + "/_bp"
                    # os.system(command)
                    #  #
                    nodes_in_graph=1000#nodes_len_dict[dataset]
                    use_upto=1000#dict_interpolate(nodes_in_graph) * 10
                    print("budget , use upto ", budget, use_upto
                    )
                    command="sh ./supervisedPredict.sh ./real_data/{}/".format(dataset) + str(type) + "/_bp" +" " + str(budget) +" " + str(use_upto) + " " +str(sampling_neighborhood)
                    print(command)
                    os.system(command)

                pass


