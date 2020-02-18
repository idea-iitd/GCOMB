import os
import random

import time

begin_time = time.time()
for j in range(0,4):
    for i in range(0,4):
        print('Training on Multiple Graphs =========== ',i)
     #   random_ind = random.randint(110,120)
     #    command = "sh ./example_supervised.sh ./graph_data/graph" + str(i) + "/graph" + str(i)
        command = "sh ./example_supervised.sh ./real_data/large_kite/train/_bp" #+ str(random_ind) + "/graph" + str(random_ind)

        print(command)
        os.system(command)

end_time = time.time()

time_taken = end_time- begin_time

fopen_sup_gs_time = open('time_sup_gs_train_powerlaw.txt','w')
fopen_sup_gs_time.write("TRAIN TIME : 2*5 " + str(time_taken))
fopen_sup_gs_time.close()


pass