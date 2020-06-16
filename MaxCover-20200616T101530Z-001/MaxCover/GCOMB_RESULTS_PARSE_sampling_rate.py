#
dataset_list=["large_gowallah", "large_graph_youtube","large_twt_snap"]#,"large_graph_youtube"]#large_gowallah"]#,
for dataset in dataset_list:


    print(dataset)
    for budget in [15, 25, 50,100, 150, 200]:
        out_gcomb_quality_time_sample=open('GCOMB_BUDGET_RESULTS/{}_budget{}'.format(dataset,
                                                                                  budget) + 'sampling_quality_time_budgets.txt',
                                           'w')

        for sampling_neighborhood in [0.75]:#[  0.1, 0.3, 0.5, 0.7, 0.9, 0.999]:



            print(" SAMPLING ", sampling_neighborhood)
            print("BUDGET ", budget)
            graph_path="./GraphSAGE-master/real_data/{}/test/_bp".format(dataset)
            rl_data = open(graph_path+'-reward_RL{}_nbs{}'.format(budget, sampling_neighborhood),'r').readlines()
            reward_gcomb= rl_data[0].replace('\n','')
            rl_time_taken = float(rl_data[1].replace('\n','').split(':')[1])

            rl_prep_time= float(open(graph_path+'_num_k_{}_time_nbs{}.txt'.format(budget, sampling_neighborhood),'r').readlines()[0].split('RL_PREP_TIME_')[1].replace('\n',''))
        #    print("rl time" ,rl_prep_time)
            print(budget, "time",rl_prep_time,rl_time_taken)
            total_gcomb_time = rl_prep_time+rl_time_taken
            print("GCOMB",reward_gcomb, total_gcomb_time)
            out_gcomb_quality_time_sample.write(str(sampling_neighborhood)+ ":"+str(total_gcomb_time)+":" + str(reward_gcomb)+"\n")

#  print(rl_data, greedy_data, sup_gs_data)
#
#+"\t"+str(total_gcomb_time)+"\t" +str(greedy_reward)+"\t"+ str(greedy_time) +"\t"+ str(sup_gs_reward)