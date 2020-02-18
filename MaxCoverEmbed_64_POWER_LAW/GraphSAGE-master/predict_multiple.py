import os
import random
#for j in range(0,20):
types =["test"]#,"train","val"]
for type in types:
    for i in range(5,6):
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
        command="sh ./supervisedPredict.sh ./real_data/large_graph_youtube/" + str(type) + "/_bp"
        os.system(command)

    pass


