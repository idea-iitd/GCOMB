#
dataset_list=["youtube/TV/"]##large_gowallah"]#,
for dataset in dataset_list:

    for budget in [10, 20, 50, 100, 150, 200]:
        imm_average_quality=open('GCOMB_BUDGET_RESULTS/{}/IMM/_IMM_budget{}'.format(dataset,
                                                                                     budget),
                                           'w')
        imm_quality_array = []
        for iter in range(0,5):
            print(dataset)


          #  for sampling_freq in [0.003,0.005,0.007]:#,0.009]:#[0.0005, 0.001, 0.003, 0.005, 0.007, 0.010, 0.05, 0.1]:
          #  large_graph_reward_imm_3_budget_20
            print("BUDGET ", budget)
            graph_path="./GraphSAGE-master/real_data/{}/test/large_graph".format(dataset)
            imm_data = open(graph_path+'_reward_imm_{}_budget_{}'.format(iter, budget),'r').readlines()
            reward_imm= float(imm_data[0].replace('\n',''))
            print(reward_imm)
            imm_quality_array.append(reward_imm)

        imm_average = sum(imm_quality_array)/len(imm_quality_array)
        print(imm_quality_array, imm_average)
        imm_average_quality.write(str(budget) +":" + str(imm_average)+"\n")

    #  print(rl_data, greedy_data, sup_gs_data)
    #
    #+"\t"+str(total_gcomb_time)+"\t" +str(greedy_reward)+"\t"+ str(greedy_time) +"\t"+ str(sup_gs_reward)