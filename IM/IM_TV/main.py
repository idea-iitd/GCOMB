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
import evaluate_spread

#30 20 1 2 0.0005 1 6

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

bestValModel = None
bestValReward = 0
fileBestRLModel = open("bestRlModel.txt",'w')
# numSteps should be less than k
# window size must be much smaller than k to generate enough samples
historyOfTuples = []
util.init(learningRate, numOfEpochs, batchSize, dimEmbedding)
for episode in range(0,numEps):
    #episode=episode+2
    print("episode ", episode)
    # generate a new graph for each episode
    graph = util.Graph(dimEmbedding, episode, k)
    print("graph ", graph)
    graphEnv.graphEnvironment.append(graph)
    # print(graph.graphX.degree())
    # if episode==0:
    # 	util.initialze_weights(graph)

    # (numsteps == k) => Terminal Condition
    previous_spread = 0

    for step in range(0, k):
        print("step: ", step)
        # print("isSelected \n", graph.isSelected)

        # select node to be added
        probOfRandomSelection = max(pow(0.1, step), 0.8)
        # print("probOfRandomSelection is : ", probOfRandomSelection)


        if(step==0):
             action_t= util.getRandomNode(episode,step)#graphEnv.graphEnvironment[episode].top_tenpct_nodes[0] action_t=
# graphEnv.graphEnvironment[episode].top_tenpct_nodes[0]#util.getRandomNode(episode,step)#graphEnv.graphEnvironment[episode].top_tenpct_nodes[0]
        # index of the selected node
        else:
            action_t = util.getNode(probOfRandomSelection, episode, step)
        print("Node selected: ", action_t)

        # add action_t to the partial solution
        graphEnv.graphEnvironment[episode].isSelected[action_t] = step
        graphEnv.graphEnvironment[episode].isCounted[action_t] = True
        neighbors_of_chosen_node = graphEnv.graphEnvironment[episode].dict_node_sampled_neighbors[action_t]#neighbors(action_t))
        print("num nbrs of chosen node ", len(neighbors_of_chosen_node))
        new_neighbors_length = len(neighbors_of_chosen_node - graphEnv.graphEnvironment[episode].neighbors_chosen_till_now)
        graphEnv.graphEnvironment[episode].neighbors_chosen_till_now= graphEnv.graphEnvironment[episode].neighbors_chosen_till_now.union(neighbors_of_chosen_node )

        print(" new diff neighbors , ",new_neighbors_length)




        for node in graph.top_tenpct_nodes:
                neighbors_of_node = graphEnv.graphEnvironment[episode].dict_node_sampled_neighbors[node]#neighbors(node))
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
        print("seleected", graph.state)
        # returns the short term reward and updates the isCounted
        shortReward, previous_spread = util.getShortReward(action_t, episode,previous_spread)#= new_neighbors_length
    #	previous_spread= shortReward
        print("Short reward for addition of ", action_t, "is ", shortReward)
        print(" new previous spread, ", previous_spread)
        graphEnv.graphEnvironment[episode].state.append(action_t)

        if (step==0):
            graphEnv.graphEnvironment[episode].cumulativeReward.append(shortReward)
        else :
            # print('*******************************************************************',shortReward)
            graphEnv.graphEnvironment[episode].cumulativeReward.append(graph.cumulativeReward[step-1] + shortReward)
        print(step, windowSize)
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
            if(len(historyOfTuples)>5) :
                util.updateParameters(historyOfTuples)
                print("saving")
                export_path=nn.model.export_saved_model("./trained_model_MC/", serving_input_receiver_fn)
                graph_path = "./GraphSAGE-master/real_data/youtube/TV/train/large_graph"
                command="python get_output.py " + graph_path +" "+ str(50) +" 0.003 None > /dev/null"
                print(command)
                os.system(command)

                rl_result_file = graph_path+'-result_RL_50_nbs_0.003'
                solution_file=open(rl_result_file, "r")

                optimal_nodes=solution_file.readlines()
                int_selected_nodes=[]

                for node_i in optimal_nodes:  # range(0, budget):
                    int_selected_nodes.append(int(node_i))
                # print( appeared_count)

                spread=0
                print(" loading graph")
                #graph_json_path=graph_path + "-G.json"
                # G_data = json.load(open(graph_json_path))

                # G = json_graph.node_link_graph(G_data)

                #print("caculating spread, num of sim", num_mc_simulation)
                # for i in range(0, num_mc_simulation):
                #     G_Copy = G.copy()
                num_mc_simulation =8


                spread=evaluate_spread.evaluate_helper_without_mp(graph_path, None, int_selected_nodes, num_mc_simulation)
                # print(" iter {}  spread {}".format(i, temp_spread))
                # spread = spread + temp_spread
                # spread = spread * 1.0 / num_mc_simulation
                print('Average Spread = ', spread)

                print('Spread = ', spread)
                reward_file_name=graph_path + "reward_RL_budget_" + str(50)
                reward_file=open(reward_file_name, 'w')
                reward_file.write('100mc' + str(spread))
                reward_file.close()
            #	reward_file_val='./GraphSAGE-master/real_data/large_kite/test/_bp-reward_RL'
            #	read_reward=int(open(reward_file_val, 'r').read())
                print("read reward val ", spread)
                print(" best reward val ", bestValReward)
                if(spread > bestValReward):
                    bestValReward = spread
                    bestValModel = export_path
                    print(" got a bettern one", bestValReward, bestValModel)
                    fileBestRLModel = open("bestRlModel.txt",'w')
                    fileBestRLModel.write(bestValModel)
                    fileBestRLModel.close()

                print("exp path ", export_path)
                print("best exp path ", bestValModel)

            if(len(historyOfTuples) > 3*k):
                historyOfTuples.pop(0)



        #
        # print(" eval ", evaluate.evaluate(graphEnv.graphEnvironment[episode].graphX.copy(), graphEnv.graphEnvironment[episode].state))
        # print("selected ",graphEnv.graphEnvironment[episode].state)
