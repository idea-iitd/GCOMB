import os
import random
#for j in range(0,20):
types =["train","val"]#,"train","val"]
for type in types:
    for i in range(0,1):
    # for i in [2000,5000, 10000,20000]:#10000, 20000]:#,5000,10000]:
        print('Training on Multiple Graphs =========== ',i)
        #random_ind = 12#random.randint(0,9)
        # command = "sh ./supervisedPredict.sh ./graph_data/graph" + str(i) + "/graph" + str(i)
        budget=15
        use_upto =5000
        nb =0.75
        command = "sh ./supervisedPredict.sh ./real_data/large_kite/"+str(type)+"/_bp" +" "+ str(budget) +" " + str(use_upto) + " " +str(nb)

        print(command)
        # os.system(command)
        # # # #
        # command="sh ./supervisedPredict.sh ./real_data/large_gowallah/" + str(type) + "/_bp"
        # os.system(command)
        #  #
     #   command="sh ./supervisedPredict.sh ./real_data/large_graph_youtube/" + str(type) + "/_bp"
        os.system(command)

    pass


