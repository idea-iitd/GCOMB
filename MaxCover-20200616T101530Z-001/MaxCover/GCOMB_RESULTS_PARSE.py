#
dataset_list=["large_graph_youtube","large_gowallah"]#,
for dataset in dataset_list:


    print(dataset)
    for sampling_neighborhood in [0.999, 0.1, 0.3, 0.5, 0.7, 0.9]:
        out_gcomb_quality=open('GCOMB_BUDGET_RESULTS/{}_nbs{}'.format(dataset,sampling_neighborhood) + 'gcomb_quality_budgets.txt', 'w')

        out_gcomb_total_time=open('GCOMB_BUDGET_RESULTS/{}_nbs{}'.format(dataset,sampling_neighborhood) + '_gcomb_time_budgets.txt', 'w')

        out_greedy_quality=open('GCOMB_BUDGET_RESULTS/{}'.format(dataset) + 'greedy_quality_budgets.txt', 'w')

        out_greedy_time=open('GCOMB_BUDGET_RESULTS/{}'.format(dataset) + 'greedy_time_budgets.txt', 'w')

        out_supgs_quality=open('GCOMB_BUDGET_RESULTS/'.format(dataset) + 'supgs_quality_budgets.txt', 'w')

      #  out_supgs_time=open('GCOMB_BUDGET_RESULTS/{}_nbs{}'.format(dataset,sampling_neighborhood) + 'supgs_time_budgets.txt', 'w')

        for budget in [15, 25, 50,100, 150, 200]:
            print(" SAMPLING ", sampling_neighborhood)
            print("BUDGET ", budget)
            graph_path="./GraphSAGE-master/real_data/{}/test/_bp".format(dataset)
            rl_data = open(graph_path+'-reward_RL{}_nbs{}'.format(budget, sampling_neighborhood),'r').readlines()
            reward_gcomb= rl_data[0].replace('\n','')
            rl_time_taken = float(rl_data[1].replace('\n','').split(':')[1])
            greedy_data = open(graph_path+'-G.json.greedy_reward{}'.format(budget),'r').readlines()
         #   print(greedy_data)
            greedy_reward, greedy_time = greedy_data[0].replace('\n',''), greedy_data[1]
            print("GREEDY ",greedy_reward, greedy_time)


            sup_gs_reward= open(graph_path+'.sup_GS_reward{}'.format(budget),'r').readlines()[0].replace('\n','')
            print(" sup gs ", sup_gs_reward)
            rl_prep_time= float(open(graph_path+'_num_k_{}_time_nbs{}.txt'.format(budget, sampling_neighborhood),'r').readlines()[0].split('RL_PREP_TIME_')[1].replace('\n',''))
        #    print("rl time" ,rl_prep_time)
            total_gcomb_time = rl_prep_time+rl_time_taken
            print("GCOMB",reward_gcomb, total_gcomb_time)
            # out_gcomb_quality.write(str(budget) +"\t" + str(reward_gcomb)+"\n")
            # out_gcomb_total_time.write(str(budget) + "\t" + str(total_gcomb_time)+"\n")
            #
            # out_greedy_quality.write(str(budget) + "\t" + str(greedy_reward)+"\n")
            # out_greedy_time.write(str(budget) + "\t" + str(greedy_time)+"\n")
            #
            # out_supgs  out_gcomb_quality.write(str(budget) +"\t" + str(reward_gcomb)+"\n")
            out_gcomb_quality.write(str(reward_gcomb)+"\n")

            out_gcomb_total_time.write(str(total_gcomb_time)+"\n")

            out_greedy_quality.write(str(greedy_reward)+"\n")
            out_greedy_time.write(str(greedy_time)+"\n")

            out_supgs_quality.write(str(sup_gs_reward)+"\n")
            #out_supgs_time.write(str(budget) + "\t" + str(total_gcomb_time))

#  print(rl_data, greedy_data, sup_gs_data)
#
#+"\t"+str(total_gcomb_time)+"\t" +str(greedy_reward)+"\t"+ str(greedy_time) +"\t"+ str(sup_gs_reward)