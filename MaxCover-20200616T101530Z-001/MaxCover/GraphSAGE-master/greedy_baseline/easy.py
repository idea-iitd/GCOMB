#
import os

for budget in  [100]:#[15, 25, 50, 150, 200]:#:[2,4,5,6,8,10,15,20,25,30,35,40]:#range(10,205,5):#[10, 15, 25, 50, 150, 200]:

    for dataset in ['large_twt_snap']:#['large_gowallah','large_graph_youtube']:#['large_kite']:# ['large_gowallah','large_graph_youtube']:#['large_kite']:
        type="test"
        dataset = dataset+"/"+type+"/"

        folder=' ..//real_data/'
        command=" python greedy_old.py "+folder+"/{}/_bp".format(dataset)+ "-G.json " \
                                     +str(budget)+folder +"{}/_bp".format(dataset)+ "-G.json.greedy" +str(budget) \
                                     + folder+"/{}/_bp".format(dataset)+  "-G.json.greedy_reward"+str(budget)

        os.system(command)
        print(command)
