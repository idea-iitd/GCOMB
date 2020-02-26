from __future__ import print_function
import tensorflow as tf
import random
import numpy as np
import sys
import os
import tensorflow as tf

from sklearn import preprocessing
from random import randint
import graphEnv

flags = tf.app.flags
FLAGS = flags.FLAGS

# neural network parameters
dimension = 0
learningRate = 0
numOfEpochs = 0
batchSize = 0
print_after_every = 0

os.environ["CUDA_DEVICE_ORDER"] = "PCI_BUS_ID"
os.environ["CUDA_VISIBLE_DEVICES"] = str(1
                                         )


your_feature_spec = {
	"number": tf.placeholder(dtype=tf.float32, shape=[None, 1], name='number'),

	# "b": tf.placeholder(dtype=tf.float32, shape=[None, 1],name='b'),
	# "c": tf.placeholder(dtype=tf.float32, shape=[None, 1],name='c'),
}

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



def init(learningRate1, numOfEpochs1, batchSize1, dimension1):
	global learningRate, batchSize, dimension, numOfEpochs
	learningRate = learningRate1
	numOfEpochs = numOfEpochs1
	batchSize = batchSize1
	dimension = dimension1+1

def neuralNetModel(features, labels, mode):
	# layer 1 for the selected nodes
	if isinstance(features, dict):
		print("here")
		features=features['mu_selected'], features['mu_left'],features['mu_v']

	layer_1 = tf.layers.dense(features[0],dimension,activation=None)

	# layer 2 for the left nodes
	layer_2 = tf.layers.dense(features[1],dimension,activation=None)

	# layer 3 for the selected nodes
	layer_3 = tf.layers.dense(features[2],dimension,activation=None)

	layer_4 = tf.concat([layer_1, layer_2, layer_3],1)

#	layer_5 = tf.nn.leaky_relu(layer_4)

	# layer_6 = tf.layers.dense(layer_5,dimension,activation=None)

	# layer_7 = tf.nn.relu(layer_6)

	# layer_8 = tf.concat([layer_7, layer_3],1)

	# layer_9 = tf.nn.relu(layer_8)

	output = tf.layers.dense(layer_4,1,activation=None)

	if (mode == tf.estimator.ModeKeys.PREDICT):
		return tf.estimator.EstimatorSpec(mode, predictions=output,  export_outputs={
			'predict': tf.estimator.export.PredictOutput(output)
		})


	#tf.losses.huber_loss(y_true, y_pred, **huber_loss_kwargs)


	#loss_op=tf.losses.huber_loss(output, labels)#tf.reduce_mean(tf.losses.huber_loss(output, labels))

	loss_op = tf.reduce_mean(tf.squared_difference(output, labels))
	#tf.print(loss_op, [loss_op])


	err=labels - output
	# huber_loss=tf.where(tf.abs(err) < 1.0,
	# 					 0.5 * tf.square(err),
	# 					 tf.abs(err) - 0.5)
	# # err=tf.subtract(output, labels)
	# # huber_loss=tf.where(tf.less(output, labels),
	# # 					tf.sqrt(tf.square(err)),
	# # 					tf.abs(err))
	# loss_op =tf.reduce_mean(huber_loss)

	optimizer = tf.train.AdamOptimizer(learning_rate=learningRate)
	train_op = optimizer.minimize(loss_op, global_step=tf.train.get_global_step())

	tf.summary.scalar('loss_op', loss_op)
	estim_specs = tf.estimator.EstimatorSpec(mode=mode, predictions=output, loss=loss_op, train_op=train_op)
	return estim_specs

model_dir_name = "./trained_model_MC"
#model = tf.estimator.Estimator(neuralNetModel,model_dir = model_dir_name)

run_config = tf.estimator.RunConfig().replace(
	session_config=tf.ConfigProto(#log_device_placement=True,
		device_count={'GPU': 4},
	))

model = tf.estimator.Estimator(neuralNetModel,model_dir = model_dir_name, config=run_config)
# last idx is the index of the state array upto which the nodes have been assumed to be added
def createMuMean(lastIdx, graph):
	mu_selected = np.zeros(dimension)
	mu_left = np.zeros(dimension)
	count_selected = 0
	count_left = 0
	for node in graph.top_tenpct_nodes:
		i = node
		if (graph.isSelected[i]!=-1 and graph.isSelected[i]< lastIdx):
			mu_selected = mu_selected + graph.embedding[i].reshape(dimension)
			count_selected = count_selected + 1
		else:
			mu_left = mu_left + graph.embedding[i].reshape(dimension)
			count_left = count_left + 1

	if count_selected!=0:
		mu_selected = mu_selected/count_selected
	if count_left!=0:
		mu_left = mu_left/count_left
	return (mu_selected, mu_left)

def createMu(lastIdx, graph):
	mu_selected = -1000000*np.ones(dimension)
	mu_left = -1000000*np.ones(dimension)
	for node in graph.top_tenpct_nodes:
		i = node
		if (graph.isSelected[i]!= (-1) and graph.isSelected[i]< lastIdx):
			mu_selected = np.maximum(mu_selected,graph.embedding_time[lastIdx][i].reshape(dimension))
		else:
			mu_left = np.maximum(mu_left,graph.embedding_time[lastIdx][i].reshape(dimension))

	return (mu_selected, mu_left)


def trainNeuralNet(historyOfTuples):

	historySize = len(historyOfTuples)
	gamma= 0.8
	print("trainNeuralNet: Training the neural net with number of history action tuples as: ", historySize)
	for i in range(0,numOfEpochs):
		mu_s = []
		mu_l = []
		mu_v = []
		y_train = []

		for j in range(0,batchSize):
			uniformSample = random.randint(0, historySize-1)
			#print(" uniform samples" , uniformSample)
			#(startIdx,nodeSelectedEarlier,cumulatedReward,endIdx,mu_s_single, mu_l_single, gid) = historyOfTuples[uniformSample]
			(mu_v_single,nodeSelectedEarlier,cumulatedReward,endIdx,mu_s_single, mu_l_single, gid) = historyOfTuples[uniformSample]
			(nodeSelectedLater, pred_later) = getBestNode(gid, endIdx)
			#if(cumulatedReward<5):
			#    cumulatedReward=0
			y_train_single = cumulatedReward + gamma*pred_later
			print(" cumulative reward ", cumulatedReward)

			y_train.append(y_train_single)
		#	mu_v_single = graphEnv.graphEnvironment[gid].embedding[nodeSelectedEarlier].reshape(dimension)

			mu_s.append(mu_s_single)
			mu_l.append(mu_l_single)
			mu_v.append(mu_v_single)

		mu_s = np.array(mu_s)
		mu_l = np.array(mu_l)
		mu_v = np.array(mu_v)

		y_train = np.array(y_train, dtype=float)
	#	print(" y train ",y_train)
		#input_fn = tf.estimator.inputs.numpy_input_fn(x={'mu_selected': mu_s, 'mu_left': mu_l, 'mu_v': mu_v}, y=y_train, shuffle=False)
		input_fn = tf.estimator.inputs.numpy_input_fn(x={'mu_selected': mu_s, 'mu_left': mu_l, 'mu_v': mu_v}, y=y_train, batch_size=3, num_epochs=500, shuffle=False)
		print('trainNeuralNet: Training Start: Epoch :',i)
	#	predctns_before = list(model.predict(input_fn))


		model.train(input_fn, steps=None)
		#predctns_after_train = list(model.predict(input_fn))
		print('trainNeuralNet: Training End: Epoch :',i)

	print('Model Trained')
	#export_path = model.export_saved_model(model_dir_name+'/export/', serving_input_receiver_fn)


#	model2=tf.saved_model.load(export_path)

	print("saved")
	#
	# x=tf.feature_column.numeric_column("mu_selected")
	# y=tf.feature_column.numeric_column("mu_left")
	# z=tf.feature_column.numeric_column("mu_v")
	#
	# #
	# #"mu_left", "mu_v"
	# feature_columns=[x,y,z]
	# feature_spec=tf.feature_column.make_parse_example_spec(feature_columns)
	# export_input_fn=tf.estimator.export.build_parsing_serving_input_receiver_fn(feature_spec)
	#
	#
	# model.export_savedmodel("saved_model", export_input_fn)

	#model.export_saved_model('saved_model', serving_input_receiver_fn)

	# tf.saved_model.save(model, "/tmp/module_no_signatures")


# returns the (index of the unselected node with the maximum Q value, Its Q Value)
def getBestNode(graphid, stateIdx):
	print("getting")
	graph = graphEnv.graphEnvironment[graphid]
	if (os.path.exists(model_dir_name) == False):
		return (-1,0)

	mu_s =  []
	mu_l = []
	mu_v = []
	mu_s_single, mu_l_single = createMu(stateIdx, graph)
	vertices = []

	for i in range(0,len(graph.top_tenpct_nodes)):
		#print(i)
		nd = graph.top_tenpct_nodes[i]
		if(graph.isSelected[nd]==-1 or graph.isSelected[nd]>=stateIdx):
			mu_s.append(mu_s_single)
			mu_l.append(mu_l_single)
			mu_v_single = graph.embedding_time[stateIdx][nd].reshape(dimension)
			mu_v.append(mu_v_single)
			vertices.append(nd)

	mu_s = np.array(mu_s)
	mu_l = np.array(mu_l)
	mu_v = np.array(mu_v)

	input_fn = tf.estimator.inputs.numpy_input_fn(x={'mu_selected': mu_s, 'mu_left': mu_l, 'mu_v': mu_v}, shuffle=False)
	print("predict")
	predictions = model.predict(input_fn)
	print("predicted")
	bestQValue = -1.0*sys.float_info.max
	bestNode = -1
	printVal = []
	for node in vertices:
		#print("node ", node)
		qValue = float(predictions.next())
		printVal.append(qValue)
		if (bestQValue < qValue and graph.isSelected[node]==(-1)):
			bestQValue = qValue
			bestNode = node
	if (bestNode == -1):
		print("QQ:", printVal)
#	print(printVal)

	print("Check: ", bestNode, bestQValue)
	return (bestNode, bestQValue)