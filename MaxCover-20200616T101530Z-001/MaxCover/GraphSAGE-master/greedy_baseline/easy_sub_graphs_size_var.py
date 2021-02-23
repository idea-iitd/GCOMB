#
import os
import sys

for budget in [20,50,100,150,200]:#range(10,205,10):#[10, 15, 25, 50, 150, 200]:

    for dataset_ in [sys.argv[1]]:#'['large_gowallah','large_graph_youtube']:#['large_kite']:# ['large_gowallah','large_graph_youtube']:#['large_kite']:
        for size_var in  [50,80,90,99]:#[90,99]:#[1,3,5,10,15,20, 40, 50, 80]:#[1,3,5,10, 20]:#,10,20,25]:#[5,10,20,25,30,40,50,60,70,80,90,100]:
            type="train"+str(size_var)
            dataset = dataset_+"/"+type+"/"

            folder=' ../real_data/'
            command=" python greedy_old.py "+folder+"/{}/_bp".format(dataset)+ "-G.json " \
                                         +str(budget)+folder +"{}/_bp".format(dataset)+ "-G.json.greedy" +str(budget) \
                                         + folder+"/{}/_bp".format(dataset)+  "-G.json.greedy_reward"+str(budget)

            os.system(command)
            #os.system("ls")
            print(command)

