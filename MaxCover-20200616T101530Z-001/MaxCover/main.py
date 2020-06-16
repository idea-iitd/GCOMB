import numpy as np
import sys
import random
import util
import math
import os
import graphEnv
from sklearn.preprocessing import StandardScaler
import evaluate
import tensorflow as tf
from nn import neuralNetModel
import nn
from tensorflow.contrib import predictor



def serving_input_receiver_fn():
    """Serving input_fn that builds features from placeholders
    Returns
    -------
    tf.estimator.export.ServingInputReceiver
    """
    mu_selected = tf.placeholder(dtype=tf.double, shape=[None, 2], name='mu_selected')
    mu_left = tf.placeholder(dtype=tf.double, shape=[None, 2], name='mu_left')
    mu_v=tf.placeholder(dtype=tf.double, shape=[None, 2], name='mu_v')

    receiver_tensors = {'mu_selected': mu_selected, 'mu_left': mu_left,'mu_v':mu_v}
    features = {'mu_selected': mu_selected, 'mu_left': mu_left,'mu_v':mu_v}
    return tf.estimator.export.ServingInputReceiver(features, receiver_tensors)




os.environ['TF_CPP_MIN_LOG_LEVEL'] = '3'

# parameters for the code
k = int(sys.argv[1])
numEps = int(sys.argv[2])
dimEmbedding = int(sys.argv[3])
windowSize = int(sys.argv[4])
learningRate = float(sys.argv[5])
numOfEpochs = int(sys.argv[6])
batchSize = int(sys.argv[7])
model_iter=int(sys.argv[8])

import time

begin_time = time.time()

bestValModel = None
bestValReward = 0
fileBestRLModel = open("bestRlModel.txt",'w')
timeElapsedfileBestRLModel = open("timeElapsed_bestRlModel_"+str(model_iter)+".txt",'w')


# numSteps should be less than k
# window size must be much smaller than k to generate enough samples
historyOfTuples = []
util.init(learningRate, numOfEpochs, batchSize, dimEmbedding, model_iter)
for episode in range(0,numEps):
    #historyOfTuples=[]
    #episode=episode+2
    print("episode ", episode)
    # generate a new graph for each episode
    nbs = 0.75
    graph = util.Graph(dimEmbedding, episode, k, nbs)
    print("graph ", graph)
    graphEnv.graphEnvironment.append(graph)
    # print(graph.graphX.degree())
    # if episode==0:
    # 	util.initialze_weights(graph)

    # (numsteps == k) => Terminal Condition
    for step in range(0, k):
        print("step: ", step)
        # print("isSelected \n", graph.isSelected)

        # select node to be added
        probOfRandomSelection = max(pow(0.1, step), 0.05)
        # print("probOfRandomSelection is : ", probOfRandomSelection)


        if(step==0):
            action_t= graphEnv.graphEnvironment[episode].top_tenpct_nodes[0]
        # index of the selected node
        else:
            action_t = util.getNode(probOfRandomSelection, episode, step)
        print("Node selected: ", action_t)

        # add action_t to the partial solution
        graphEnv.graphEnvironment[episode].isSelected[action_t] = step
        graphEnv.graphEnvironment[episode].isCounted[action_t] = True
        graphEnv.graphEnvironment[episode].state.append(action_t)
        neighbors_of_chosen_node = set(graphEnv.graphEnvironment[episode].dict_node_sampled_neighbors[action_t])
        graphEnv.graphEnvironment[episode].neighbors_chosen_till_now= graphEnv.graphEnvironment[episode].neighbors_chosen_till_now.union(neighbors_of_chosen_node )




        for node in graph.top_tenpct_nodes:
                neighbors_of_node = set(graphEnv.graphEnvironment[episode].dict_node_sampled_neighbors[node])
                new_neighbors_not_in_solutions_neighbors = neighbors_of_node - graphEnv.graphEnvironment[episode].neighbors_chosen_till_now

                graphEnv.graphEnvironment[episode].embedding_time[step+1][node][0] = len(new_neighbors_not_in_solutions_neighbors)



        scaler = StandardScaler()
        temp_column_for_cover=np.ones((len(graphEnv.graphEnvironment[episode].embedding_time[step+1]), 1), dtype='float64')

        i=0
        dict_map_i_key = {}
        for key, value in graphEnv.graphEnvironment[episode].embedding_time[step+1].items():
            temp_column_for_cover[i] =value[0]
            dict_map_i_key[i] = key
            i+=1


        scaler.fit(temp_column_for_cover)
        temp_column_for_cover_norm = None
        temp_column_for_cover_norm = scaler.transform(temp_column_for_cover)

        for index, value in enumerate(temp_column_for_cover_norm):
            true_node_id = dict_map_i_key[index]
            graphEnv.graphEnvironment[episode].embedding_time[step+1][true_node_id][0] = value

        # returns the short term reward and updates the isCounted
        shortReward = util.getShortReward(action_t, episode)
        print("Short reward for addition of ", action_t, "is ", shortReward)

        if (step==0):
            graphEnv.graphEnvironment[episode].cumulativeReward.append(shortReward)
        else :
            # print('*******************************************************************',shortReward)
            graphEnv.graphEnvironment[episode].cumulativeReward.append(graph.cumulativeReward[step-1] + shortReward)

        if (step > (windowSize)):
            if (step == windowSize):
                netShortReward = graphEnv.graphEnvironment[episode].cumulativeReward[step-1]
            else:
                netShortReward = graphEnv.graphEnvironment[episode].cumulativeReward[step-1] - graphEnv.graphEnvironment[episode].cumulativeReward[step - (windowSize)-1]

            # The short term reward does not include the reward by adding the step vertex
            # Action Tuples are of the form: (startIdx, nodeAdded, net cumulative reward, last index)
            mu_v_at_that_step_minus_window = graphEnv.graphEnvironment[episode].embedding_time[step-windowSize][action_t].reshape(dimEmbedding+1)

            mu_s,mu_l = util.createMuUtil(step-(windowSize), episode)
            actionTuple = (mu_v_at_that_step_minus_window,graph.state[step-(windowSize)],netShortReward,step, mu_s, mu_l, episode)

            historyOfTuples.append(actionTuple)
        if(len(historyOfTuples)>batchSize) :
            util.updateParameters(historyOfTuples)
            print("saving")
            export_path=nn.model.export_saved_model("./trained_model_MC_"+str(model_iter)+"/", serving_input_receiver_fn)
            if True:
                reward_val_array = []
                for val in [0]:
                    num_k =15
                    nbs=0.999
                    command="python get_output.py ./GraphSAGE-master/real_data/large_kite/val/_bp "+  str(model_iter) + " " + str(num_k) +" " + str(nbs)# + " None" #+" > /dev/null"

                    #
                    reward_file_val='./GraphSAGE-master/real_data/large_kite/val/_bp-reward_RL{}_nbs{}'.format(num_k,nbs)



                    read_reward=open(reward_file_val, 'w')
                    read_reward.write("0")
                    read_reward.close()

                    os.system(command)

                    read_reward=int(open(reward_file_val, 'r').readline())
                    print("read reward val ", read_reward)
                    reward_val_array.append(read_reward)

                reward_val_average = sum(reward_val_array)*1.0/len(reward_val_array)

                print("avg now ",reward_val_average, " best", bestValReward)
                if(reward_val_average > bestValReward):
                    bestValReward = reward_val_average
                    bestValModel = export_path

                    fileBestRLModel = open("bestRlModel.txt",'w')
                    fileBestRLModel.write(bestValModel)
                    fileBestRLModel.close()

                print("exp path ", export_path)
                print("best exp path ", bestValModel)
                timeElapsed = time.time() - begin_time
                print(" time elap best model", timeElapsed, bestValModel, bestValReward)
                timeElapsedfileBestRLModel.write(str(timeElapsed) +"\t" + bestValModel+"\t"+ str(bestValReward)+"\n" )
                timeElapsedfileBestRLModel.flush()

        if(len(historyOfTuples) > batchSize):
            historyOfTuples.pop(0)


#15 5 1 2 0.0005 1 10

        print(" eval ", evaluate.evaluate(graphEnv.graphEnvironment[episode].graphX.copy(), graphEnv.graphEnvironment[episode].state))
        print("selected ",graphEnv.graphEnvironment[episode].state)


end_time = time.time()

time_taken = end_time- begin_time

fopen_sup_gs_time = open('time_Rl_train_bp.txt','w')
fopen_sup_gs_time.write("TRAIN TIME : " + str(time_taken) + " numEps "+ str(numEps))
fopen_sup_gs_time.close()

