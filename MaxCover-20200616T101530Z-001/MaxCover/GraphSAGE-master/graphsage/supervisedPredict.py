from __future__ import division
from __future__ import print_function

import os
import time
import tensorflow as tf
import numpy as np
import sklearn
from sklearn import metrics
from sklearn import preprocessing
import evaluaterew
import operator

from graphsage.supervised_models import SupervisedGraphsage
from graphsage.models import SAGEInfo
from graphsage.minibatch import NodeMinibatchIterator
from graphsage.neigh_samplers import UniformNeighborSampler
from graphsage.utils import load_data
import networkx as nx

from networkx.algorithms import bipartite

os.environ["CUDA_DEVICE_ORDER"]="PCI_BUS_ID"

# Set random seed
seed = 123
np.random.seed(seed)
tf.set_random_seed(seed)

# Settings
flags = tf.app.flags
FLAGS = flags.FLAGS

tf.app.flags.DEFINE_boolean('log_device_placement', False,
							"""Whether to log device placement.""")
#core params..
flags.DEFINE_string('model', 'graphsage_maxpool', 'model names. See README for possible values.')  
flags.DEFINE_float('learning_rate', 0.0001, 'initial learning rate.')
flags.DEFINE_string("model_size", "small", "Can be big or small; model specific def'ns")
flags.DEFINE_string('train_prefix', '', 'prefix identifying training data. must be specified.')

# left to default values in main experiments 
flags.DEFINE_integer('epochs', 10, 'number of epochs to train.')
flags.DEFINE_float('dropout', 0.0, 'dropout rate (1 - keep probability).')
flags.DEFINE_float('weight_decay', 0.0, 'weight for l2 loss on embedding matrix.')
flags.DEFINE_integer('max_degree', 128, 'maximum node degree.')
flags.DEFINE_integer('samples_1', 15, 'number of samples in layer 1')
flags.DEFINE_integer('samples_2', 25, 'number of samples in layer 2')
flags.DEFINE_integer('samples_3', 0, 'number of users samples in layer 3. (Only for mean model)')
flags.DEFINE_integer('dim_1', 128, 'Size of output dim (final is 2x this, if using concat)')
flags.DEFINE_integer('dim_2', 128, 'Size of output dim (final is 2x this, if using concat)')
flags.DEFINE_boolean('random_context', False, 'Whether to use random context or direct edges')
flags.DEFINE_integer('batch_size', 512, 'minibatch size.')
flags.DEFINE_boolean('sigmoid', False, 'whether to use sigmoid loss')
flags.DEFINE_integer('identity_dim', 0, 'Set to positive value to use identity embedding features of that dimension. Default 0.')

#logging, saving, validation settings etc.
flags.DEFINE_string('base_log_dir', '.', 'base directory for logging and saving embeddings')
flags.DEFINE_integer('validate_iter', 5000, "how often to run a validation minibatch.")
flags.DEFINE_integer('validate_batch_size', 256, "how many nodes per validation sample.")
flags.DEFINE_integer('gpu', 4, "which gpu to use.")
flags.DEFINE_integer('print_every', 5, "How often to print training info.")
flags.DEFINE_integer('max_total_steps', 10**10, "Maximum total number of iterations")


flags.DEFINE_boolean('test_mode', True, 'True for train, False for test')
flags.DEFINE_integer('num_k', 15, 'Budget')
flags.DEFINE_float('bud_mul_fac', 0, 'bud mulp factor, 3 for WC, 10 for IC and TV')
flags.DEFINE_float('neighborhood_sampling', 0.5, 'neighboor hood for RL')


os.environ["CUDA_VISIBLE_DEVICES"]=str(FLAGS.gpu)


GPU_MEM_FRACTION = 0.8

def calc_f1(y_true, y_pred):
	if not FLAGS.sigmoid:
		y_true = np.argmax(y_true, axis=1)
		y_pred = np.argmax(y_pred, axis=1)
	else:
		y_pred[y_pred > 0.5] = 1
		y_pred[y_pred <= 0.5] = 0
	return metrics.f1_score(y_true, y_pred, average="micro"), metrics.f1_score(y_true, y_pred, average="macro")

# Define model evaluation function
def evaluate(sess, model, minibatch_iter, size=None):
	t_test = time.time()
	feed_dict_val, labels = minibatch_iter.node_val_feed_dict(size)
	node_outs_val = sess.run([model.preds, model.loss],
						feed_dict=feed_dict_val)
	mic, mac = calc_f1(labels, node_outs_val[0])
	return node_outs_val[1], mic, mac, (time.time() - t_test)

def log_dir():
	log_dir = FLAGS.base_log_dir + "/sup-" + FLAGS.train_prefix.split("/")[-2]
	log_dir += "/{model:s}_{model_size:s}_{lr:0.4f}/".format(
			model=FLAGS.model,
			model_size=FLAGS.model_size,
			lr=FLAGS.learning_rate)
	if not os.path.exists(log_dir):
		os.makedirs(log_dir)
	return log_dir

def incremental_predict(sess, model, minibatch_iter, size):
	predict_prob = []
	predict_classes = []
	embeddings = []
	finished = False
	iter_num = 0

	while not finished:
		feed_dict_val, batch_labels, finished, _  = minibatch_iter.incremental_node_testing_feed_dict_nodes_top_degree(size, iter_num)
		node_outs_val = sess.run([model.node_temp_embeddings, model.preds],
						 feed_dict=feed_dict_val)
		embeddings.extend(node_outs_val[0])
		predict_prob.extend(node_outs_val[1])
	  #  print(node_outs_val)
		#print(predict_prob)
		# print("embeddings", len(embeddings[0]),len(embeddings[0][0]))
		# print("predict_prob", len(predict_prob[0]), len(predict_prob[0][0]))
		# break
		iter_num += 1

	print("--", FLAGS.sigmoid)
	print("embeddings", len(embeddings),len(embeddings[0]))
	print("predict_prob", len(predict_prob), len(predict_prob[0]))

	val_preds = np.vstack(predict_prob)

	if FLAGS.sigmoid:
		predict_classes = np.argmax(val_preds, axis=1)
	else:
		predict_classes[val_preds > 0.5] = 1
		predict_classes[val_preds <= 0.5] = 0

	return (predict_prob, predict_classes, embeddings)

def incremental_evaluate(sess, model, minibatch_iter, size, test=False):
	t_test = time.time()
	finished = False
	val_losses = []
	val_preds = []
	labels = []
	iter_num = 0
	finished = False

	while not finished:
		feed_dict_val, batch_labels, finished, _  = minibatch_iter.incremental_node_testing_feed_dict_nodes_top_degree(size, iter_num, test=test)
		node_outs_val = sess.run([model.preds, model.loss],
						 feed_dict=feed_dict_val)
		val_preds.append(node_outs_val[0])
		labels.append(batch_labels)
		val_losses.append(node_outs_val[1])
		iter_num += 1
	val_preds = np.vstack(val_preds)
	labels = np.vstack(labels)
	f1_scores = calc_f1(labels, val_preds)
	return np.mean(val_losses), f1_scores[0], f1_scores[1], (time.time() - t_test)

def construct_placeholders(num_classes):
	# Define placeholders
	placeholders = {
		'labels' : tf.placeholder(tf.float32, shape=(None, num_classes), name='labels'),
		'batch' : tf.placeholder(tf.int32, shape=(None), name='batch1'),
		'dropout': tf.placeholder_with_default(0., shape=(), name='dropout'),
		'batch_size' : tf.placeholder(tf.int32, name='batch_size'),
	}
	return placeholders

def predict(train_data, test_data=None):
	num_k = FLAGS.num_k
	G = train_data[0]
	#features = train_data[1]
	#features_store = np.copy(features)
	id_map = train_data[1]
	class_map  = train_data[3]
	if isinstance(list(class_map.values())[0], list):
		num_classes = len(list(class_map.values())[0])
	else:
		num_classes = len(set(class_map.values()))

	# if not features is None:
	# 	# pad with dummy zero vector
	# 	features = np.vstack([features, np.zeros((features.shape[1],))])

	context_pairs = train_data[3] if FLAGS.random_context else None
	placeholders = construct_placeholders(num_classes)
	minibatch = NodeMinibatchIterator(G,
			id_map,
			placeholders,
			class_map,
			num_classes,
			batch_size=FLAGS.batch_size,
			max_degree=FLAGS.max_degree,
			context_pairs = context_pairs,
									  budget=num_k,bud_mul_fac=FLAGS.bud_mul_fac,mode="test", prefix = FLAGS.train_prefix, neighborhood_sampling=FLAGS.neighborhood_sampling
									  )

	features= minibatch.features


	time_minibatch=minibatch.time_sampling_plus_norm  # time_minibatch_end - time_minibatch_start

	print("creatred features and sampling done")
	print("minibatch time", time_minibatch)


	adj_info_tf_time_beg = time.time()


	adj_info_ph = tf.placeholder(tf.int32, shape=minibatch.adj.shape)
	adj_info = tf.Variable(adj_info_ph, trainable=False, name="adj_info")

	adj_info_tf_time_end=time.time()
	adj_info_tf_time=adj_info_tf_time_end -adj_info_tf_time_beg
	print("adj info time", adj_info_tf_time)

	if FLAGS.model == 'graphsage_mean':
		# Create model
		sampler = UniformNeighborSampler(adj_info)
		if FLAGS.samples_3 != 0:
			layer_infos = [SAGEInfo("node", sampler, FLAGS.samples_1, FLAGS.dim_1),
								SAGEInfo("node", sampler, FLAGS.samples_2, FLAGS.dim_2),
								SAGEInfo("node", sampler, FLAGS.samples_3, FLAGS.dim_2)]
		elif FLAGS.samples_2 != 0:
			layer_infos = [SAGEInfo("node", sampler, FLAGS.samples_1, FLAGS.dim_1),
								SAGEInfo("node", sampler, FLAGS.samples_2, FLAGS.dim_2)]
		else:
			layer_infos = [SAGEInfo("node", sampler, FLAGS.samples_1, FLAGS.dim_1)]

		model = SupervisedGraphsage(num_classes, placeholders,
									 features,
									 adj_info,
									 minibatch.deg,
									 layer_infos,
									 model_size=FLAGS.model_size,
									 sigmoid_loss = FLAGS.sigmoid,
									 identity_dim = FLAGS.identity_dim,
									 logging=True)
	elif FLAGS.model == 'gcn':
		# Create model
		sampler = UniformNeighborSampler(adj_info)
		layer_infos = [SAGEInfo("node", sampler, FLAGS.samples_1, 2*FLAGS.dim_1),
							SAGEInfo("node", sampler, FLAGS.samples_2, 2*FLAGS.dim_2)]

		model = SupervisedGraphsage(num_classes, placeholders,
									 features,
									 adj_info,
									 minibatch.deg,
									 layer_infos=layer_infos,
									 aggregator_type="gcn",
									 model_size=FLAGS.model_size,
									 concat=False,
									 sigmoid_loss = FLAGS.sigmoid,
									 identity_dim = FLAGS.identity_dim,
									 logging=True)

	elif FLAGS.model == 'graphsage_seq':
		sampler = UniformNeighborSampler(adj_info)
		layer_infos = [SAGEInfo("node", sampler, FLAGS.samples_1, FLAGS.dim_1),
							SAGEInfo("node", sampler, FLAGS.samples_2, FLAGS.dim_2)]

		model = SupervisedGraphsage(num_classes, placeholders,
									 features,
									 adj_info,
									 minibatch.deg,
									 layer_infos=layer_infos,
									 aggregator_type="seq",
									 model_size=FLAGS.model_size,
									 sigmoid_loss = FLAGS.sigmoid,
									 identity_dim = FLAGS.identity_dim,
									 logging=True)

	elif FLAGS.model == 'graphsage_maxpool':
		sampler = UniformNeighborSampler(adj_info)
		layer_infos = [SAGEInfo("node", sampler, FLAGS.samples_1, FLAGS.dim_1),
							SAGEInfo("node", sampler, FLAGS.samples_2, FLAGS.dim_2)]

		model = SupervisedGraphsage(num_classes, placeholders,
									features,
									adj_info,
									minibatch.deg,
									 layer_infos=layer_infos,
									 aggregator_type="maxpool",
									 model_size=FLAGS.model_size,
									 sigmoid_loss = FLAGS.sigmoid,
									 identity_dim = FLAGS.identity_dim,
									 logging=True)

	elif FLAGS.model == 'graphsage_meanpool':
		sampler = UniformNeighborSampler(adj_info)
		layer_infos = [SAGEInfo("node", sampler, FLAGS.samples_1, FLAGS.dim_1),
							SAGEInfo("node", sampler, FLAGS.samples_2, FLAGS.dim_2)]

		model = SupervisedGraphsage(num_classes, placeholders,
									features,
									adj_info,
									minibatch.deg,
									 layer_infos=layer_infos,
									 aggregator_type="meanpool",
									 model_size=FLAGS.model_size,
									 sigmoid_loss = FLAGS.sigmoid,
									 identity_dim = FLAGS.identity_dim,
									 logging=True)

	else:
		raise Exception('Error: model name unrecognized.')

	config = tf.ConfigProto(log_device_placement=FLAGS.log_device_placement)
	config.gpu_options.allow_growth = True

	#config.gpu_options.per_process_gpu_memory_fraction = GPU_MEM_FRACTION
	config.allow_soft_placement = True

	# Initialize session
	sess = tf.Session(config=config)
	merged = tf.summary.merge_all()
	summary_writer = tf.summary.FileWriter(log_dir(), sess.graph)

	# Init variables
	adj_info_tf_init_time_beg=time.time()

	sess.run(tf.global_variables_initializer(), feed_dict={adj_info_ph: minibatch.adj})


	adj_info_tf_init_time_end=time.time()
	adj_info_tf_init_time=adj_info_tf_init_time_end - adj_info_tf_init_time_beg
	print("adj info time init", adj_info_tf_init_time)


#	train_adj_info = tf.assign(adj_info, minibatch.adj)
#	val_adj_info = tf.assign(adj_info, minibatch.test_adj)

	# Load trained model
	var_to_save = []
	for var in tf.trainable_variables():
		var_to_save.append(var)
	saver = tf.train.Saver(var_to_save)
	print("Trained Model Loading!")
	saver.restore(sess,"KITE_supervisedTrainedModel_MC_marginal/model.ckpt")
	print("Trained Model Loaded!")


	sup_gs_pred_start_time=time.time()

	print("Predicting the classes of all the data set")
	predict_prob, pred_classes, embeddings = incremental_predict(sess, model, minibatch, FLAGS.batch_size)
	print("Predicted the classes of all the data set")
	sup_gs_pred_end_time=time.time()

	print("Saving the Predicted Output")
	sup_gs_prediction_time=sup_gs_pred_end_time - sup_gs_pred_start_time
	print("embed time ", sup_gs_prediction_time)

	# to output
	# print("predict_prob", predict_prob[143])
	# print("pred_classes", pred_classes[143])
	# print("embeddings", embeddings[143])
	print("Saved the Predicted Output")

	active_prob_beg_time=time.time()

	active_one_prob = {}
	active_one_prob_dict = {}

	for index in range(0,len(pred_classes)):
		active_one_prob[index] = predict_prob[index][0]
		# print(index,pred_classes[index],predict_prob[index])

	for index in range(0, len(pred_classes)):
		active_one_prob_dict[minibatch.dict_map_couter_to_real_node[minibatch.top_degree_nodes[index]]] = predict_prob[index][0]

#	bottom_nodes, top_nodes = bipartite.sets(G)
	#bottom_nodes = list(bottom_nodes)
	total_top_ten_percent = len(active_one_prob_dict)#int(num_k*FLAGS.bud_mul_fac)#int(0.25*len(bottom_nodes))

#	if total_top_ten_percent>1000:
#		total_top_ten_percent = 500

	sorted_dict = sorted(active_one_prob_dict.items(), key=operator.itemgetter(1),reverse=True)

	top_ten_percent = []

	count_solution = 0


	y = 0

	while count_solution<total_top_ten_percent:
		#if sorted_dict[y][0] in bottom_nodes:
		top_ten_percent.append(sorted_dict[y][0])
		count_solution = count_solution + 1
		y = y + 1

	active_prob_end_time=time.time()


	active_prob_time=active_prob_end_time - active_prob_beg_time

	print("actve prob time ", active_prob_time)

	result_top_percent = FLAGS.train_prefix + "_top_ten_percent{}_nbs{}.txt".format(num_k, FLAGS.neighborhood_sampling)

	file_handle2 = open(result_top_percent,"w")
	print('*******************',len(top_ten_percent))


	dict_node_scores = {}

	for ind in top_ten_percent:
		file_handle2.write(str(ind))
		file_handle2.write(" ")
		dict_node_scores[ind] = active_one_prob_dict[ind]



	file_handle2.close()

	dict_node_scores_file_name = FLAGS.train_prefix + "_node_scores_supgs{}_nbs{}".format(num_k,FLAGS.neighborhood_sampling)
	import pickle


	pickle_start_time=time.time()

	with open(dict_node_scores_file_name + '.pickle', 'wb') as handle:
		pickle.dump(dict_node_scores, handle, protocol=pickle.HIGHEST_PROTOCOL)
	pickle_end_time = time.time()
	pickle_time = pickle_end_time - pickle_start_time


	print("pickle scores time ", pickle_time)

	#print(dict_node_scores)

	result_top_percent = FLAGS.train_prefix + "_top_ten_percent_analyse{}_nbs{}.txt".format(num_k, FLAGS.neighborhood_sampling)

	file_handle2 = open(result_top_percent, "w")
	print('*******************', len(top_ten_percent))
	print('******************* Writing top to file')


	graph_degree = G.degree()

	for ind in top_ten_percent:
		#print(ind)
		file_handle2.write(str(ind)+ "   " + str(graph_degree[ind]) +  " "+str( active_one_prob_dict[ind]))
		file_handle2.write(" \n")


	file_handle2.close()

	print('******************* Written top to file')

	top_30 = []

	count_solution = 0
	y = 0

	while count_solution<num_k:
#		if sorted_dict[y][0] in bottom_nodes:
		top_30.append(sorted_dict[y][0])
		count_solution = count_solution + 1
		y = y + 1

	result_file_name = FLAGS.train_prefix + "_sup_GS_sol{}.txt".format(num_k)

	file_handle = open(result_file_name,"w")
	file_handle.write(str(num_k))
	file_handle.write("\n")
	for ind in top_30:
		file_handle.write(str(ind))
		file_handle.write(" ")

	file_handle.close()

	from sklearn.preprocessing import StandardScaler

	# scaler = StandardScaler()
	# scaler.fit(embeddings)
	# embeddings = scaler.transform(embeddings)
	#

	embeddings = np.array(embeddings)
	# embeddings = np.hstack([embeddings, features_store])
	print('Final Embeddings shape = ',embeddings.shape)
	embedding_file_name = FLAGS.train_prefix + "_embeddings{}_nbs{}.npy".format(num_k, FLAGS.neighborhood_sampling)
   # np.save(embedding_file_name,embeddings)

	dict_embeddings_top_for_rl_without_rw={}

	for index, node_id in enumerate(top_ten_percent):
		#print("map", node_id, minibatch.top_degree_nodes[index], index)
		embed_sup_gs=embeddings[index]

		dict_embeddings_top_for_rl_without_rw[node_id]=embed_sup_gs
	#	print("index, nodeid ", index, node_id)
	import pickle
	with open(embedding_file_name + '.pickle', 'wb') as handle:
		pickle.dump(dict_embeddings_top_for_rl_without_rw, handle, protocol=pickle.HIGHEST_PROTOCOL)




	total_time_for_rl_prep = adj_info_tf_time + time_minibatch + sup_gs_prediction_time + active_prob_time + adj_info_tf_init_time


	time_rl_prep_file_name=FLAGS.train_prefix + "_num_k_" + str(FLAGS.num_k) + "_time_nbs{}.txt".format(FLAGS.neighborhood_sampling)
	print(time_rl_prep_file_name)
	time_file=open(time_rl_prep_file_name, 'w')
	time_file.write("RL_PREP_TIME_" + str(total_time_for_rl_prep) + '\n')

    

	reward_file_name = FLAGS.train_prefix + ".sup_GS_reward{}".format(num_k)
	reward = evaluaterew.evaluate(G,top_30)
	file_handle3 = open(reward_file_name,"w")
	file_handle3.write(str(reward))
	file_handle3.close()

	print(" time rl prepare", total_time_for_rl_prep)


def main(argv=None):
	print("Loading  data..")
	train_data = load_data(FLAGS.train_prefix)
	print("Done loading  data..")
	predict(train_data)

if __name__ == '__main__':
	tf.app.run()
