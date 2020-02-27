import os
from pathlib import Path

for i in range(0,1):
    val_graphs=[110]
    #val_graphs = [20000,1000, 2000, 3000, 4000, 5000, 10000]
 #   val_graphs = [5000, 10000, 20000, ]
  #  val_graphs=[0,1,2,3,4,5,6]
   # val_graphs=[x for x in range(0,10)]
    val_graphs=[5,6,7,8,9]#5000,10000,

    for model_iter in range(13,14):


#        read_rl_time_bestModel=open('timeElapsed_bestRlModel_'+str(model_iter)+'.txt', 'r').readlines();
 #       print(read_rl_time_bestModel)
        model_path = './trained_model_MC_'+ str(model_iter) +'/'
        sub_models_dirs = [x for x in Path(model_path).iterdir()
               if x.is_dir() and 'temp' not in str(x)]
        out_rl_time_bestModel_performance=open('timeElapseReward_' + str(model_iter) + '.txt', 'w')

        for model_path in sub_models_dirs[0:5]:
            print(model_path)
            model_path=str(model_path)
            # time, model_path, _= line.split('\t')
            # print(time, model_path)

            # val_graphs[0:]:
            reward_array = []
            for val in val_graphs[0:5]:


                graph_path = "./GraphSAGE-master/graph_data/graph" + str(val) + "/graph" + str(val)


                command = "python get_output.py " + graph_path + " " +model_path + " None"
                print(command)
                #
                # command="python get_output.py ./GraphSAGE-master/real_data/large_gowallah/test/_bp"
                #os.system(command)
                #
                # command = "python get_output.py ./GraphSAGE-master/real_data/large_graph_youtube/test/_bp"


              ##  if(model_path in dict_model_id_perf):
                model_creation_time = str( os.path.getctime(model_path+'/saved_model.pb'))
                print(model_creation_time)
                    # continue
         #       else:
                read_rl_reward=graph_path + "-reward_RL"
                file_reward_destroy=open(read_rl_reward, 'w')
                file_reward_destroy.close()

                os.system(command)

                read_rl_reward = graph_path +"-reward_RL"
                print(read_rl_reward)
                reward_value = open(read_rl_reward,'r').readline()
                reward_array.append(int(reward_value))

            average_val_reward = str(sum(reward_array)*1.0/len(reward_array))

            #print(time, model_path,reward_value,read_rl_reward)
            model_creation_time = model_path.split('/')[1]
            out_rl_time_bestModel_performance.write(model_creation_time +"\t"+ model_path +"\t" + str(average_val_reward) +"\n")
            out_rl_time_bestModel_performance.flush()
            print("analysis" + model_creation_time+"\t" +average_val_reward +"\t" + str(model_path))
           # dict_model_id_perf[model_path] =  reward_value
        out_rl_time_bestModel_performance.close()
        pass
        pass
    pass
pass