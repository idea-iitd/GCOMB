#
dataset_list=["stack/IC/"]##large_gowallah"]#,
for dataset in dataset_list:

    for budget in [10, 20, 50, 100, 150, 200]:
        print(dataset)
        out_gcomb_quality_time_sample=open('GCOMB_BUDGET_RESULTS/{}_budget{}'.format(dataset,
                                                                                     budget) + 'sampling_quality_time_budgets.txt',
                                           'w')

        for sampling_freq in [0.05]:#,0.009]:#[0.0005, 0.001, 0.003, 0.005, 0.007, 0.010, 0.05, 0.1]:


            print(" SAMPLING ", sampling_freq)
            print("BUDGET ", budget)
            graph_path="./GraphSAGE-master/real_data/{}/test/large_graph".format(dataset)
            #rl_data = open(graph_path+'_reward_RL_budget_{}_nbs_{}'.format(budget, sampling_freq),'r').readlines()
            reward_gcomb= 0##rl_data[0].replace('\n','')
           # print(rl_data)
            rl_time_file = open(graph_path +'-time_RL_budget{}_nbs_{}'.format(budget,sampling_freq),'r')
            rl_time_taken = float(rl_time_file.read())
           # rl_time_taken = float(rl_data[1].replace('\n','').split(':')[1])
#large_graph_num_k_150_time.txt_150_nbs_0.0005
            rl_prep_file_path = graph_path+'_num_k_{}_time.txt_{}_nbs_{}'.format(budget,budget, sampling_freq)
            print(rl_prep_file_path)
            rl_prep_time= float(open(rl_prep_file_path,'r').readlines()[0].split('RL_PREP_TIME_')[1].replace('\n',''))
        #    print("rl time" ,rl_prep_time)
            print("time",rl_prep_time,rl_time_taken)
            total_gcomb_time = rl_prep_time+rl_time_taken
            print("GCOMB",reward_gcomb, total_gcomb_time)
            out_gcomb_quality_time_sample.write(str(sampling_freq) +":" + str(reward_gcomb)+":"+str(total_gcomb_time)+"\n")

#  print(rl_data, greedy_data, sup_gs_data)
#
#+"\t"+str(total_gcomb_time)+"\t" +str(greedy_reward)+"\t"+ str(greedy_time) +"\t"+ str(sup_gs_reward)