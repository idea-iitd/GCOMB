#
import os

for budget in range(10,205,10):#[100]:#range(10,205,5):#[10, 15, 25, 50, 150, 200]:

    for dataset_ in ['large_kite']:#'['large_gowallah','large_graph_youtube']:#['large_kite']:# ['large_gowallah','large_graph_youtube']:#['large_kite']:
        for size_var in [5,10,20,25,30,40,50,60,70,80,90,100]:
            type="train"+str(size_var)
            dataset = dataset_+"/"+type+"/"

            folder=' ../GraphSAGE-master/real_data/'
            command=" python greedy_old.py "+folder+"/{}/_bp".format(dataset)+ "-G.json " \
                                         +str(budget)+folder +"{}/_bp".format(dataset)+ "-G.json.greedy" +str(budget) \
                                         + folder+"/{}/_bp".format(dataset)+  "-G.json.greedy_reward"+str(budget)

            os.system(command)
            print(command)
