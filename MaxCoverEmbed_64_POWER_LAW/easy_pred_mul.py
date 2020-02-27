import os
for i in range(0,1):
    dataset_list=["large_gowallah"]#:##],"large_graph_youtube"]#["large_graph_youtube"]#,"large_graph_youtube"]

    model_iter = 9
    #read_rl_time_bestModel='./trained_model_MC_0/1581495538open('timeElapsed_bestRlModel_'+str(model_iter)+'.txt', 'r').readlines();
    #print(",",read_rl_time_bestModel)
    #time, model_path, _=read_rl_time_bestModel[-1].split('\t')
    #model_path ='./trained_model_MC_9/1578961068'
    model_path = open('bestRlModel.txt','r').read()

    for dataset in dataset_list:
     #   out_rl_time_bestModel_performance=open('GCOMB_RESULTS/{}'.format(dataset) + '.txt', 'w')

       # for line in read_rl_time_bestModel[-2:-1]:
        #print(time, model_path)
        for sampling_neighborhood in [0.999]:##, 0.9,0.1, 0.3, 0.5,0.7]:#0.1, 0.3, 0.5, 0.7,
            for budget in [15,25,50,100,150,200]:#150,200]:#15,25,50,100,
                graph_path = "./GraphSAGE-master/real_data/{}/test/_bp".format(dataset)
                command = "python get_output.py " + graph_path + " " +model_path.replace("\n","") + " " + str(budget) +" " + str(sampling_neighborhood) + " None"
                print(command)
        #        command="python get_output.py " + graph_path
                os.system(command)
                #
                # command = "python get_output.py ./GraphSAGE-master/real_data/large_graph_youtube/test/_bp"
    #
    #
    #
    #             read_rl_reward=graph_path + "-reward_RL"
    #             print(read_rl_reward)
    #             file_reward_destroy=open(read_rl_reward, 'w')
    #             file_reward_destroy.close()
    #
    #             os.system(command)
    #
    #             read_rl_reward = graph_path +"-reward_RL"
    #             print("RL reward File", read_rl_reward)
    #             reward_value = open(read_rl_reward,'r').readline()
    #             print(time, model_path,reward_value,read_rl_reward)
    #             out_rl_time_bestModel_performance.write(time +"\t" + str(reward_value))
    #             out_rl_time_bestModel_performance.flush()
    #
    #     pass
    #     pass
    # pass
    # pass