import os
for i in range(0,1):
    dataset_list=["large_graph_youtube", "large_gowallah"]#["large_graph_youtube", "large_gowallah","large_graph_youtube","large_twt_snap"]##,"large_graph_youtube"]#""large_gowallah"]#,""]#:##],"large_graph_youtube"]#["large_graph_youtube"]#,"large_graph_youtube"]

    model_path = open('bestRlModel.txt','r').read()

    for dataset in dataset_list:
     #   out_rl_time_bestModel_performance=open('GCOMB_RESULTS/{}'.format(dataset) + '.txt', 'w')

       # for line in read_rl_time_bestModel[-2:-1]:
        #print(time, model_path)
        for sampling_neighborhood in [0.75]:##, 0.9,0.1, 0.3, 0.5,0.7]:#0.1, 0.3, 0.5, 0.7,
            #for budget in [15, 25,  50,100,150,200]:# [100]:#[ 15, 25,  50,100,150,200]:#150,200]:#15,25,50,100,
            for budget in [20, 50, 100, 150, 200]:  # 100,50,15,25,150,200]:#[15,25,50,100,150,200]:

                graph_path = "./GraphSAGE-master/real_data/{}/test/_bp".format(dataset)
                command = "python get_output.py " + graph_path + " " +model_path.replace("\n","") + " " + str(budget) +" " + str(sampling_neighborhood) + " None"
                print(command)
        #        command="python get_output.py " + graph_path
                os.system(command)
                #
                # command = "python get_output.py ./GraphSAGE-master/real_data/large_graph_youtube/test/_bp"

