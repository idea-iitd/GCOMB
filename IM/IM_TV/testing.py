import tensorflow as tf
import random
import numpy as np
import sys
import os
import util_output
import time

# neural network parameters
os.environ['TF_CPP_MIN_LOG_LEVEL'] = '3'
dimension = 256
np.set_printoptions(threshold=np.inf)

def neuralNetModel(features, labels, mode):	
	# layer 1 for the selected nodes
	layer_1 = tf.layers.dense(features['mu_selected'],dimension,activation=None)

	# layer 2 for the left nodes
	layer_2 = tf.layers.dense(features['mu_left'],dimension,activation=None)

	# layer 3 for the selected nodes
	layer_3 = tf.layers.dense(features['mu_v'],dimension,activation=None)

	layer_4 = tf.concat([layer_1, layer_2, layer_3],1)
	layer_4 = tf.nn.relu(layer_4)

	output = tf.layers.dense(layer_4,1,activation=None)

	if (mode == tf.estimator.ModeKeys.PREDICT):
		return tf.estimator.EstimatorSpec(mode, predictions=output)

	loss_op = tf.reduce_mean(tf.squared_difference(output, labels))
	optimizer = tf.train.GradientDescentOptimizer(learning_rate=learningRate)
	train_op = optimizer.minimize(loss_op, global_step=tf.train.get_global_step())

	tf.summary.scalar('loss_op', loss_op)
	estim_specs = tf.estimator.EstimatorSpec(mode=mode, predictions=output, loss=loss_op, train_op=train_op)
	return estim_specs
	
model_dir_name = "./trained_model_MC"
model = tf.estimator.Estimator(neuralNetModel,model_dir = model_dir_name)


mu_s_single = np.zeros(dimension)
mu_l_single = np.ones(dimension)

mu_s = []
mu_l = []
mu_v = []
mu_s.append(mu_s_single)
mu_l.append(mu_s_single)
mu_v.append(mu_s_single)
mu_s.append(mu_l_single)
mu_l.append(mu_l_single)
mu_v.append(mu_l_single)

mu_s = np.array(mu_s)
mu_l = np.array(mu_l)
mu_v = np.array(mu_v)
input_fn = tf.estimator.inputs.numpy_input_fn(x={'mu_selected': mu_s, 'mu_left': mu_l, 'mu_v': mu_v}, shuffle=False)
predictions = model.predict(input_fn)


qValue = float(predictions.next())
print("---------------",qValue)
qValue = float(predictions.next())
print("---------------",qValue)