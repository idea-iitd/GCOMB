import os
for dataset in ['youtube']:


    for sampling_freq in [0.003]:#[0.005,0.007, 0.009]:#[0.003]:# [0.0005,0.001,0.003, 0.005,0.007,0.010,0.05,0.1]:

        for budget in [10, 20, 50, 100, 150, 200]:#, 20,50,100,150,200]:#[50,100,200]:#,150,100,50,20,10]:#[10,20,50,100,150,200]:#,50,100,150,200]:#[10,20,50,100,150,200]:#,20,50,100,150,200]:#,50,100,150,200]:#[100,150]:#[10,20,50,100,150,200]:#,150,200]:#[20,50,100]:#100,150,200]:#, 10,15,20,25,50,100]:
         #   command = "python get_output.py ./GraphSAGE-master/graph_data/graph" + str(val) + "/graph" + str(val)
            command = "python get_output.py ./GraphSAGE-master/real_data/"+dataset+"/TV/test/large_graph " + str(budget) +" "+ str(sampling_freq)+" "+" Notnone"

            os.system(command)
            pass
    pass
pass
pass