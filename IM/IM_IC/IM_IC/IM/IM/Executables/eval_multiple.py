#
import os
#for budget in [5,10,15,20,25,50,100]:
for budget in [1,5,10,15,20,25,100, 150, 200]:
    command="python eval_anything.py ../GraphSAGE-master/real_data/youtube/WC/test/large_graph-result_RL_"+str(budget)+" "+ str(budget)+" RL test"
    print(command)
    os.system(command)

    command="python eval_anything.py ../GraphSAGE-master/real_data/youtube/WC/test/large_graph_sup_GS_sol.txt_"+ str(budget)+" " +str(budget)+" supgs test"
    print(command)
    os.system(command)


    command = "python eval_anything.py ../GraphSAGE-master/real_data/youtube/WC/test/multi_iter/large_graph_ic_imm_sol_eps0.5_num_k_"+str(budget)+"_iter_0.txt "+ str(budget)+" imm test"
    print(command)
    os.system(command)