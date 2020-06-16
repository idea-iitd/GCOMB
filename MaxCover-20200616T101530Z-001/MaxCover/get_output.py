import tensorflow as tf
import random
import numpy as np
import sys
import os
import util_output
from random import shuffle
import time
import evaluate
from sklearn.preprocessing import StandardScaler
tf.logging.set_verbosity(tf.logging.ERROR)
from pathlib import Path
from tensorflow.contrib import predictor

# neural network parameters
os.environ['TF_CPP_MIN_LOG_LEVEL'] = '3'
dimension = 0
np.set_printoptions(threshold=np.inf)

os.environ["CUDA_DEVICE_ORDER"] = "PCI_BUS_ID"
os.environ["CUDA_VISIBLE_DEVICES"] = str(2)
def init(dimension1):
	global dimension
	dimension = dimension1



def neuralNetModel(features, labels, mode):
	# layer 1 for the selected nodes
	layer_1 = tf.layers.dense(features['mu_selected'],dimension,activation=None)

	# layer 2 for the left nodes
	layer_2 = tf.layers.dense(features['mu_left'],dimension,activation=None)

	# layer 3 for the selected nodes
	layer_3 = tf.layers.dense(features['mu_v'],dimension,activation=None)

	layer_4 = tf.concat([layer_1, layer_2, layer_3],1)
	#layer_5 = tf.nn.relu(layer_4)

	#output = tf.layers.dense(layer_5,1,activation=None)
	output = tf.layers.dense(layer_4,1,activation=None)

	#if (mode == tf.estimator.ModeKeys.PREDICT):
	return tf.estimator.EstimatorSpec(mode, predictions=output)

	loss_op = tf.reduce_mean(tf.squared_difference(output, labels))
	optimizer = tf.train.GradientDescentOptimizer(learning_rate=learningRate)
	train_op = optimizer.minimize(loss_op, global_step=tf.train.get_global_step())

#	tf.summary.scalar('loss_op', loss_op)
#	estim_specs = tf.estimator.EstimatorSpec(mode=mode, predictions=output)
	#estim_specs = tf.estimator.EstimatorSpec(mode=mode, predictions=output, loss=loss_op, train_op=train_op)

	return output

# def neuralNetModel(features, labels, mode):
# 	# print("In Neural Net Model: Dimension is ", dimension)

# 	# layer 1 for the selected nodes
# 	layer_1 = tf.layers.dense(features['mu_selected'],dimension,activation=None)

# 	# layer 2 for the left nodes
# 	layer_2 = tf.layers.dense(features['mu_left'],dimension,activation=None)

# 	# layer 3 for the selected nodes
# 	layer_3 = tf.layers.dense(features['mu_v'],dimension,activation=None)

# 	layer_4 = tf.concat([layer_1, layer_2, layer_3],1)
# 	layer_4 = tf.nn.relu(layer_4)

# 	output = tf.layers.dense(layer_4,1,activation=None)

# 	if (mode == tf.estimator.ModeKeys.PREDICT):
# 		return tf.estimator.EstimatorSpec(mode, predictions=output)

# 	return estim_specs

init(2)

graph_dir = sys.argv[1]


num_k = int(sys.argv[3])
nbs = float(sys.argv[4])

graph = util_output.Graph(graph_dir, num_k, nbs)

file_handle = open(graph_dir + "-result_RL{}_nbs{}".format(num_k, nbs),"w")

file_handle.write(str(num_k))
file_handle.write("\n")

# shuffle(graph.top_tenpct_nodes)


total_time_for_neighbors = 0

start_time = time.time()
solution_set = []
#TESTING

print(sys)

if ((len(sys.argv) >5)):
    bestModelPath = sys.argv[2]
    
    print(" doing time testing")
    print(bestModelPath)
    pass
else:# TRAINING
    
   # print(" Getting overall best model")
    model_dir_name = "./trained_model_MC_"+str(sys.argv[2])
    #model = tf.estimator.Estimator(neuralNetModel,model_dir = model_dir_name)
    #model = tf.saved_model.load("saved_model")
    #
    #export_dir = 'trained_model_MC/'
    # fileBestRLModel = open("bestRlModel.txt",'r')
    # bestModelPath = fileBestRLModel.read()
    # fileBestRLModel.close()

    # if(os.path.exists(bestModelPath)):
    #     pass
    #
    # else:
    subdirs = [x for x in Path(model_dir_name).iterdir()
               if x.is_dir() and 'temp' not in str(x)]
    bestModelPath = str(sorted(subdirs)[-1])
    print("model", bestModelPath)


predict_fn = predictor.from_saved_model(bestModelPath)

# last idx is the index of the state array upto which the nodes have been assumed to be added
def createMu_Mean(lastIdx, graph):
	mu_selected = np.zeros(dimension)
	mu_left = np.zeros(dimension)
	count_selected = 0
	count_left = 0
	for i in range(0,graph.numNodes):
		if (graph.isSelected[i]!=-1 and graph.isSelected[i]< lastIdx):
			mu_selected = mu_selected + graph.embedding[lastIdx][i].reshape(dimension)
			count_selected = count_selected + 1
		else:
			mu_left = mu_left + graph.embedding[lastIdx][i].reshape(dimension)
			count_left = count_left + 1

	if (count_selected != 0):
		mu_selected = mu_selected/count_selected
	if (count_left != 0):
		mu_left = mu_left/count_left
	return (mu_selected, mu_left)

def createMu(lastIdx, graph):
	time_prep_beg=time.time()

	mu_selected = -1000000*np.ones(dimension)
	mu_left = -1000000*np.ones(dimension)
	for node in graph.top_tenpct_nodes:
		i = node
		if (graph.isSelected[i]!= (-1)):
			mu_selected = np.maximum(mu_selected,graph.embedding_time[lastIdx][i].reshape(dimension))
		else:
			mu_left = np.maximum(mu_left,graph.embedding_time[lastIdx][i].reshape(dimension))


	time_prep_end=time.time()
	print("mu time  ", time_prep_end -time_prep_beg)
	return (mu_selected, mu_left)

def argmax(iterable):
	return max(enumerate(iterable), key=lambda x: x[1])[0]

# returns the (index of the unselected node with the maximum Q value, Its Q Value)
def getBestNode(graph, stateIdx):
	# if (os.path.exists(model_dir_name) == False):
	# 	return (0,0)

	mu_s = []
	mu_l = []
	mu_v = []


	time_prep_beg=time.time()


	mu_s_single, mu_l_single = createMu(stateIdx, graph)
	vertices = []

	for nd in graph.top_tenpct_nodes:
		if graph.isSelected[nd]==-1:
			mu_s.append(mu_s_single)
			mu_l.append(mu_l_single)
			mu_v.append(graph.embedding_time[stateIdx][nd].reshape(dimension))
			vertices.append(nd)



	mu_s = np.array(mu_s)
	mu_l = np.array(mu_l)
	mu_v = np.array(mu_v)
	# print('*******',mu_v[0][1],mu_v[1][1],mu_v[2][1])


	#input_fn = tf.estimator.inputs.numpy_input_fn(x={'mu_selected': mu_s, 'mu_left': mu_l, 'mu_v': mu_v}, shuffle=False)
#

	#model_input=tf.train.Example(
	#	features=tf.train.Features(feature={'x': tf.train.Feature(float_list=tf.train.FloatList(value=[mu_s, mu_l,mu_v]))}))
	#model_input=model_input.SerializeToString()
	#predictions=predict_fn({"inputs": [model_input]})


	time_prep_end=time.time()
	prep_time=time_prep_end - time_prep_beg

	print('prepare time', prep_time)


	time_pred_begin  = time.time()


#	predictions= list(model.predict(input_fn))

	#print([mu_s,mu_l,mu_v])
	predictions = 	predict_fn({'mu_selected': [mu_s][0], 'mu_left': [mu_l][0], 'mu_v': [mu_v][0]})
	predictions = predictions['output']
#	print("muv", len([mu_v][0]), [[mu_v][0]])
#predict_fn(input_fn)
	#print("pred ", predictions)
	bestQValue = -1.0*sys.float_info.max
	bestNode = -1

	printVal = []
	max_index = argmax(predictions)
	print("max index ", max_index)
	#print(" max _value ", predictions[max_index])
	bestNode=vertices[argmax(predictions)]
	#print(" argmax",bestNode )
	# for node in vertices:
	# 	qValue = float(predictions.next())
	# 	printVal.append(qValue)
	# #	print(" node qvalue ", node, "  ", qValue)
	# 	if (bestQValue < qValue):
	# 		# print('here')
	# 		bestQValue = qValue
	# 		bestNode = node
	time_pred_end=time.time()
	pred_time = time_pred_end-time_pred_begin
	print('pred time', pred_time)

	# print(printVal)

	if (bestNode == -1):
		print("QQ:", printVal)
	return (bestNode, bestQValue)

#neighors_dict = {}
#for node in graph.top_tenpct_nodes:
#	neighbors_of_node=set(graph.graphx.neighbors(node))
#	neighors_dict[node]= neighbors_of_node

for step in range(0, num_k):
	# print("step: ", step)
	if(step==0):
		nodeSelected= graph.top_tenpct_nodes[0]
		value_q=-1
	else:
		(nodeSelected,value_q) = getBestNode(graph,step)
	# graph.top_tenpct_nodes.remove(nodeSelected)
	solution_set.append(nodeSelected)
	print(nodeSelected,value_q)
	graph.isSelected[nodeSelected] = step
	graph.state.append(nodeSelected)
	graph.isCounted[nodeSelected] = True
	start_time_neighbors=time.time()

	neighbors_of_chosen_node=graph.dict_node_sampled_neighbors[nodeSelected]#set(graph.graphx.neighbors(nodeSelected))
	graph.neighbors_chosen_till_now=graph.neighbors_chosen_till_now.union(neighbors_of_chosen_node)

	for node in graph.top_tenpct_nodes:
		neighbors_of_node=graph.dict_node_sampled_neighbors[node]#set(graph.graphx.neighbors(node))
		new_neighbors_not_in_solutions_neighbors=neighbors_of_node - graph.neighbors_chosen_till_now

		graph.embedding_time[step + 1][node][0]=len(new_neighbors_not_in_solutions_neighbors)

	scaler=StandardScaler()
	temp_column_for_cover=np.ones((len(graph.embedding_time[step + 1]), 1), dtype='float64')

	i=0
	dict_map_i_key={}
	for key, value in graph.embedding_time[step + 1].items():
		temp_column_for_cover[i]=value[0]
		dict_map_i_key[i]=key
		i+=1

	scaler.fit(temp_column_for_cover)
	temp_column_for_cover_norm=None
	temp_column_for_cover_norm=scaler.transform(temp_column_for_cover)

	for index, value in enumerate(temp_column_for_cover_norm):
		true_node_id=dict_map_i_key[index]
		graph.embedding_time[step + 1][true_node_id][0]=value

	end_time_neighbors=time.time()
	total_time_for_neighbors += end_time_neighbors- start_time_neighbors


	print(" total time neighbors ", total_time_for_neighbors)
#	print("dict ", graph.embedding_time[step + 1])
	print(" next step ", step+1)
	print("sol set " ,solution_set)
end_time=time.time()

for node_sel in solution_set:
	file_handle.write(str(node_sel))
	file_handle.write(" ")

elapsed_time = end_time - start_time
print("time taken = ",elapsed_time)

file_handle.close()


reward = evaluate.evaluate(graph.graphx,solution_set)
file_handle2 = open(graph_dir + "-reward_RL{}_nbs{}".format(num_k, nbs),"w")
file_handle2.write(str(reward)+"\n" + "time :"+str(elapsed_time))
file_handle2.close()
print(" reward@{}k@nbs{} = ".format(num_k,nbs), reward)