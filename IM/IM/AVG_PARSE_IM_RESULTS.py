#
dataset_list=["youtube/TV/"]##large_gowallah"]#,
for dataset in dataset_list:

    for budget in [10, 20, 50, 100, 150, 200]:
      #  print(dataset)
        out_gcomb_quality_time_sample=open('GCOMB_BUDGET_RESULTS/{}_budget{}'.format(dataset,
                                                                                     budget) + 'sampling_quality_time_budgets.txt',
                                           'w')
        #sampling_freq= 0.003
        # for  in [0.003]:#,0.009]:#[0.0005, 0.001, 0.003, 0.005, 0.007, 0.010, 0.05, 0.1]:
        sum_budget=0
        sum_time = 0


        for iter in range(1,5):
               # f
               # print(" SAMPLING ", sampling_freq)
              #  print("BUDGET ", budget)
                result_path="./GCOMB_BUDGET_RESULTS/{}/result_model{}/iter{}/_budget{}sampling_quality_time_budgets.txt".format(dataset,iter,1, budget)
                result_line = open(result_path,'r').readline()
                sampling, reward, time_taken = result_line.split(":")
               # print(sampling,reward,time_taken)
                sum_budget+= float(reward)
                sum_time+=float(time_taken)
               # out_gcomb_quality_time_sample.write(str(sampling_freq) +":" + str(reward_gcomb)+":"+str(total_gcomb_time)+"\n")

        avg_budget = sum_budget/4
        avg_time = sum_time/4
        print(budget,avg_budget,avg_time)
    #  print(rl_data, greedy_data, sup_gs_data)
    #
    #+"\t"+str(total_gcomb_time)+"\t" +str(greedy_reward)+"\t"+ str(greedy_time) +"\t"+ str(sup_gs_reward)