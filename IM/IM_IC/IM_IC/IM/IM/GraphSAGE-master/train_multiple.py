import os
import random
for j in range(0,2):
    for i in range(0,5):
        print('Training on Multiple Graphs =========== ',i)
     #   random_ind = random.randint(110,120)
#        command = "sh ./example_supervised.sh ./graph_data/graph" + str(random_ind) + "/graph" + str(random_ind)
        command = "sh ./example_supervised.sh ./real_data/youtube/IC/train/large_graph"

        print(command)
        os.system(command)
pass