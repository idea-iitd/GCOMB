import os
for i in range(0,1):
    val_graphs=[110]
    #val_graphs = [20000,1000, 2000, 3000, 4000, 5000, 10000]
 #   val_graphs = [5000, 10000, 20000, ]
  #  val_graphs=[0,1,2,3,4,5,6]
   # val_graphs=[x for x in range(0,10)]
    val_graphs=[-1]#5000,10000,

    for model_iter in range(9,10):
        read_rl_time_bestModel=open('timeElapsed_bestRlModel_'+str(model_iter)+'.txt', 'r').readlines();
        print(",",read_rl_time_bestModel)

        for val in val_graphs[0:5]:
            out_rl_time_bestModel_performance=open('timeElapseReward_' + str(model_iter) + '.txt', 'w')

            dict_model_id_perf = {}
            for line in read_rl_time_bestModel[-1:]:
                time, model_path, _= line.split('\t')
                print(time, model_path)

                #val_graphs[0:]:

                # graph_path = "./GraphSAGE-master/graph_data/graph" + str(val) + "/graph" + str(val)


                # command = "python get_output.py " + graph_path + " " +model_path.replace("\n","") + " None"
                # print(command)
                #
                graph_path = "./GraphSAGE-master/real_data/large_graph_youtube/test/_bp"
                command = "python get_output.py " + graph_path + " " +model_path.replace("\n","") + " None"

        #        command="python get_output.py " + graph_path
              #  os.system(command)
                #
                # command = "python get_output.py ./GraphSAGE-master/real_data/large_graph_youtube/test/_bp"


                if(model_path in dict_model_id_perf):
                    pass
                    # continue
                else:
                    read_rl_reward=graph_path + "-reward_RL"
                    print(read_rl_reward)
                    file_reward_destroy=open(read_rl_reward, 'w')
                    file_reward_destroy.close()

                    os.system(command)

                read_rl_reward = graph_path +"-reward_RL"
                print("RL reward File", read_rl_reward)
                reward_value = open(read_rl_reward,'r').readline()
                print(time, model_path,reward_value,read_rl_reward)
                out_rl_time_bestModel_performance.write(time +"\t" + str(reward_value))
                out_rl_time_bestModel_performance.flush()
                dict_model_id_perf[model_path] =  reward_value

        pass
        pass
    pass
pass