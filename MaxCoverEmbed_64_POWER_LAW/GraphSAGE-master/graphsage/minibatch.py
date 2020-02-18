from __future__ import division
from __future__ import print_function
from sklearn.preprocessing import StandardScaler
from networkx.algorithms import bipartite
import time
import pickle

import numpy as np

np.random.seed(123)

class EdgeMinibatchIterator(object):
    
    """ This minibatch iterator iterates over batches of sampled edges or
    random pairs of co-occuring edges.

    G -- networkx graph
    id2idx -- dict mapping node ids to index in feature tensor
    placeholders -- tensorflow placeholders object
    context_pairs -- if not none, then a list of co-occuring node pairs (from random walks)
    batch_size -- size of the minibatches
    max_degree -- maximum size of the downsampled adjacency lists
    n2v_retrain -- signals that the iterator is being used to add new embeddings to a n2v model
    fixed_n2v -- signals that the iterator is being used to retrain n2v with only existing nodes as context
    """
    def __init__(self, G, id2idx, 
            placeholders, context_pairs=None, batch_size=100, max_degree=25,
            n2v_retrain=False, fixed_n2v=False,
            **kwargs):

        self.G = G
        self.nodes = G.nodes()
        self.id2idx = id2idx
        self.placeholders = placeholders
        self.batch_size = batch_size
        self.max_degree = max_degree
        self.batch_num = 0

        self.nodes = np.random.permutation(G.nodes())
        self.adj, self.deg = self.construct_adj()
        self.test_adj = self.construct_test_adj()
        if context_pairs is None:
            edges = G.edges()
        else:
            edges = context_pairs
        self.train_edges = self.edges = np.random.permutation(edges)
        if not n2v_retrain:
            self.train_edges = self._remove_isolated(self.train_edges)
            self.val_edges = [e for e in G.edges() if G[e[0]][e[1]]['train_removed']]
        else:
            if fixed_n2v:
                self.train_edges = self.val_edges = self._n2v_prune(self.edges)
            else:
                self.train_edges = self.val_edges = self.edges

        print(len([n for n in G.nodes() if not G.node[n]['test'] and not G.node[n]['val']]), 'train nodes')
        print(len([n for n in G.nodes() if G.node[n]['test'] or G.node[n]['val']]), 'test nodes')
        self.val_set_size = len(self.val_edges)

    def _n2v_prune(self, edges):
        is_val = lambda n : self.G.node[n]["val"] or self.G.node[n]["test"]
        return [e for e in edges if not is_val(e[1])]

    def _remove_isolated(self, edge_list):
        new_edge_list = []
        missing = 0
        for n1, n2 in edge_list:
            if not n1 in self.G.node or not n2 in self.G.node:
                missing += 1
                continue
            if (self.deg[self.id2idx[n1]] == 0 or self.deg[self.id2idx[n2]] == 0) \
                    and (not self.G.node[n1]['test'] or self.G.node[n1]['val']) \
                    and (not self.G.node[n2]['test'] or self.G.node[n2]['val']):
                continue
            else:
                new_edge_list.append((n1,n2))
        print("Unexpected missing:", missing)
        return new_edge_list

    def construct_adj(self):
        adj = len(self.id2idx)*np.ones((len(self.id2idx)+1, self.max_degree))
        deg = np.zeros((len(self.id2idx),))

        for nodeid in self.G.nodes():
            if self.G.node[nodeid]['test'] or self.G.node[nodeid]['val']:
                continue
            neighbors = np.array([self.id2idx[neighbor] 
                for neighbor in self.G.neighbors(nodeid)
                if (not self.G[nodeid][neighbor]['train_removed'])])
            deg[self.id2idx[nodeid]] = len(neighbors)
            if len(neighbors) == 0:
                continue
            if len(neighbors) > self.max_degree:
                neighbors = np.random.choice(neighbors, self.max_degree, replace=False)
            elif len(neighbors) < self.max_degree:
                neighbors = np.random.choice(neighbors, self.max_degree, replace=True)
            adj[self.id2idx[nodeid], :] = neighbors
        return adj, deg

    def construct_test_adj(self):
        adj = len(self.id2idx)*np.ones((len(self.id2idx)+1, self.max_degree))
        for nodeid in self.G.nodes():
            neighbors = np.array([self.id2idx[neighbor] 
                for neighbor in self.G.neighbors(nodeid)])
            if len(neighbors) == 0:
                continue
            if len(neighbors) > self.max_degree:
                neighbors = np.random.choice(neighbors, self.max_degree, replace=False)
            elif len(neighbors) < self.max_degree:
                neighbors = np.random.choice(neighbors, self.max_degree, replace=True)
            adj[self.id2idx[nodeid], :] = neighbors
        return adj

    def end(self):
        return self.batch_num * self.batch_size >= len(self.train_edges)

    def batch_feed_dict(self, batch_edges):
        batch1 = []
        batch2 = []
        for node1, node2 in batch_edges:
            batch1.append(self.id2idx[node1])
            batch2.append(self.id2idx[node2])

        feed_dict = dict()
        feed_dict.update({self.placeholders['batch_size'] : len(batch_edges)})
        feed_dict.update({self.placeholders['batch1']: batch1})
        feed_dict.update({self.placeholders['batch2']: batch2})

        return feed_dict

    def next_minibatch_feed_dict(self):
        start_idx = self.batch_num * self.batch_size
        self.batch_num += 1
        end_idx = min(start_idx + self.batch_size, len(self.train_edges))
        batch_edges = self.train_edges[start_idx : end_idx]
        return self.batch_feed_dict(batch_edges)

    def num_training_batches(self):
        return len(self.train_edges) // self.batch_size + 1

    def val_feed_dict(self, size=None):
        edge_list = self.val_edges
        if size is None:
            return self.batch_feed_dict(edge_list)
        else:
            ind = np.random.permutation(len(edge_list))
            val_edges = [edge_list[i] for i in ind[:min(size, len(ind))]]
            return self.batch_feed_dict(val_edges)

    def incremental_val_feed_dict(self, size, iter_num):
        edge_list = self.val_edges
        val_edges = edge_list[iter_num*size:min((iter_num+1)*size, 
            len(edge_list))]
        return self.batch_feed_dict(val_edges), (iter_num+1)*size >= len(self.val_edges), val_edges

    def incremental_embed_feed_dict(self, size, iter_num):
        node_list = self.nodes
        val_nodes = node_list[iter_num*size:min((iter_num+1)*size, 
            len(node_list))]
        val_edges = [(n,n) for n in val_nodes]
        return self.batch_feed_dict(val_edges), (iter_num+1)*size >= len(node_list), val_edges

    def label_val(self):
        train_edges = []
        val_edges = []
        for n1, n2 in self.G.edges():
            if (self.G.node[n1]['val'] or self.G.node[n1]['test'] 
                    or self.G.node[n2]['val'] or self.G.node[n2]['test']):
                val_edges.append((n1,n2))
            else:
                train_edges.append((n1,n2))
        return train_edges, val_edges

    def shuffle(self):
        """ Re-shuffle the training set.
            Also reset the batch number.
        """
        self.train_edges = np.random.permutation(self.train_edges)
        self.nodes = np.random.permutation(self.nodes)
        self.batch_num = 0

class NodeMinibatchIterator(object):
    
    """ 
    This minibatch iterator iterates over nodes for supervised learning.

    G -- networkx graph
    id2idx -- dict mapping node ids to integer values indexing feature tensor
    placeholders -- standard tensorflow placeholders object for feeding
    label_map -- map from node ids to class values (integer or list)
    num_classes -- number of output classes
    batch_size -- size of the minibatches
    max_degree -- maximum size of the downsampled adjacency lists
    """
    def __init__(self, G, id2idx, 
            placeholders, label_map, num_classes, 
            batch_size=100, max_degree=25, mode="None",prefix =None, budget=None, bud_mul_fac=None,
                 neighborhood_sampling=None,
            **kwargs):

        self.G = G
        self.nodes = G.nodes()
        self.id2idx = id2idx
        self.placeholders = placeholders
        self.batch_size = batch_size
        self.max_degree = max_degree
        self.batch_num = 0
        self.label_map = label_map
        self.num_classes = num_classes


        if (mode =="train"):
            self.adj, self.deg=self.construct_adj()
            self.test_adj=self.construct_test_adj()

            self.test_nodes=[n for n in self.G.nodes() if self.G.node[n]['val']]
            self.all_nodes=[n for n in self.G.nodes()]
            self.train_nodes=[x for x in self.all_nodes ]#if float(self.label_map[x][0]) > 0]

            self.val_nodes=self.train_nodes[0:1000]#n for n in self.G.nodes() if self.deg[id2idx[n]] > 0][
                           #int(len(G) / 2) - int(0.05 * len(G) / 2):int(len(G) / 2)]

           # self.no_train_nodes_set=set(self.val_nodes + self.test_nodes)
            #self.train_nodes=set(G.nodes()).difference(self.no_train_nodes_set)
            # don't train on nodes that only have edges to test set
            self.train_nodes=[n for n in self.train_nodes if self.deg[id2idx[n]] > 0][
                             0:int(len(G) / 2) ]
            print(len(self.val_nodes), len(self.train_nodes))

            self.features=self.scale_features(np.load(prefix + "-feats.npy"))
            print("prefix", prefix)
            print("shape feature before ", self.features.shape)
            self.features=self.features[:, [0]]
            print("shape feature after keeping one feature", self.features.shape)

        elif (mode == "test"):
            self.all_nodes=[n for n in self.G.nodes()]
            #   self.adj, self.deg = self.construct_adj()
            self.adj, self.deg, self.features, self.top_degree_nodes, time_sampling_feature, self.dict_map_couter_to_real_node, sampled_nbrs_for_rl=self.construct_adj_and_feat_for_top_degree_testing(
                budget, bud_mul_fac, neighborhood_sampling)
            time_norm_start=time.time()


            self.features=self.scale_features(self.features)
            time_norm_end=time.time()

            time_norm=time_norm_end - time_norm_start
            print("sampling +norm time ", time_norm + time_sampling_feature)

            self.time_sampling_plus_norm = time_norm+ time_sampling_feature
            print(" num top nodes ", len(self.top_degree_nodes))

            with open(prefix + '-sampled_nbrs_for_rl{}_nbs{}'.format(budget, neighborhood_sampling) + '.pickle', 'wb') as handle:
                pickle.dump(sampled_nbrs_for_rl, handle, protocol=pickle.HIGHEST_PROTOCOL)

           # exit()

    def scale_features(self, feats):
        scaler=StandardScaler()
        scaler.fit(feats)
        feats=scaler.transform(feats)
        return feats


    def _make_label_vec(self, node):
        label = self.label_map[node]
        if isinstance(label, list):
            label_vec = np.array(label)
         #   print(" is instance")
        else:
            label_vec = np.zeros((self.num_classes))
            class_ind = self.label_map[node]
            label_vec[class_ind] = 1
        return label_vec

    def construct_adj(self):
        adj = len(self.id2idx)*np.ones((len(self.id2idx)+1, self.max_degree))
        deg = np.zeros((len(self.id2idx),))

        for nodeid in self.G.nodes():
            if self.G.node[nodeid]['test'] or self.G.node[nodeid]['val']:
                continue
            neighbors = np.array([self.id2idx[neighbor] 
                for neighbor in self.G.neighbors(nodeid)
               ])
            deg[self.id2idx[nodeid]] = len(neighbors)
            if len(neighbors) == 0:
                continue
            if len(neighbors) > self.max_degree:
                neighbors = np.random.choice(neighbors, self.max_degree, replace=False)
            elif len(neighbors) < self.max_degree:
                neighbors = np.random.choice(neighbors, self.max_degree, replace=True)
            adj[self.id2idx[nodeid], :] = neighbors
        return adj, deg

    def construct_test_adj(self):
        adj = len(self.id2idx)*np.ones((len(self.id2idx)+1, self.max_degree))
        for nodeid in self.G.nodes():
            neighbors = np.array([self.id2idx[neighbor] 
                for neighbor in self.G.neighbors(nodeid)])
            if len(neighbors) == 0:
                continue
            if len(neighbors) > self.max_degree:
                neighbors = np.random.choice(neighbors, self.max_degree, replace=False)
            elif len(neighbors) < self.max_degree:
                neighbors = np.random.choice(neighbors, self.max_degree, replace=True)
            adj[self.id2idx[nodeid], :] = neighbors
        return adj

    def construct_adj_and_feat_for_top_degree_testing(self, budget=None, bud_mul_fac=None,neighborhood_sampling=None):
        list_of_all_neighbors=list()
        dict_map_real_to_counter_node={}
        node_counter=0  # for useful nodes
        sampled_nbrs_for_rl={}

        dict_map_couter_to_real_node={}


        bottom_nodes, top_nodes=bipartite.sets(self.G)
       # bottom_nodes=list(bottom_nodes)
        print("len bottom nodes ", len(bottom_nodes))

   #     deg=np.zeros((len(self.id2idx),))

        print(" creating input features")

        print("getting degree")
        degree_of_nodes_all=self.G.degree()


      #  print("len degree_of_nodes_left ", len(degree_of_nodes_left))
        degree_of_nodes_all=[degree for node, degree in degree_of_nodes_all.items() ]
        degree_of_nodes_left=degree_of_nodes_all[0:len(bottom_nodes)]

        print(" beginning sampling ")
        import time
        time_sampling_beg=time.time()
        print("bud mul fac ", bud_mul_fac)
        #total_top_degree_nodes=max(int(-budget* bud_mul_fac), -1*len(bottom_nodes))  #
        #total_top_degree_nodes=max(int(-len(bottom_nodes)* 0.002), -1*len(bottom_nodes))  #
        #total_top_degree_nodes=max(int(-len(bottom_nodes)* bud_mul_fac), -1*len(bottom_nodes))  #
        total_top_degree_nodes=max(int(-1* bud_mul_fac ) +1, -1*len(bottom_nodes))  #
        #   random_nodes = self.G.nodes()
        #  import random
        # random.shuffle(random_nodes)
        # random_nodes = random_nodes[0:200]
        top_degree_nodes=set(np.argpartition(degree_of_nodes_left, total_top_degree_nodes)[total_top_degree_nodes:])
        #  print("top_degree nodes" ,top_degree_nodes)
        #   top_degree_nodes = random_nodes
        print(" top deg ", len(top_degree_nodes))
      #  print(list(top_degree_nodes))
        adj={}#len(self.id2idx) * np.ones((len(self.id2idx) + 1, self.max_degree))
        deg=np.zeros((len(self.id2idx),))

      #  list_top_nodes_and_sampled_neighbors=list()#np.array([])#set()
        nbr_processed={}
       # np_array_neighbors = {}
                    
        for nodeid in top_degree_nodes:
            if (nodeid not in dict_map_real_to_counter_node):
                dict_map_real_to_counter_node[nodeid]=node_counter
                dict_map_couter_to_real_node[node_counter]=nodeid
                node_counter+=1


        #    list_top_nodes_and_sampled_neighbors.append(dict_map_real_to_counter_node[nodeid])
            nbr_processed[nodeid]=1
            # if self.G.node[nodeid]['test'] or self.G.node[nodeid]['val']:
            #     continue
            neighbors=self.G.neighbors(nodeid)
          #  np_array_neighbors[nodeid] = neighbors
          #
          #   time_Amins2=time.time() - time_sampling_beg
          #   print(" TIME Aminus2 ", time_Amins2)


         #   print(list_of_all_neighbors)
            list_of_all_neighbors.extend( neighbors)
            # time_Amins=time.time() - time_sampling_beg
            # print(" TIME Aminus ", time_Amins)
            deg[dict_map_real_to_counter_node[nodeid]]=len(neighbors)
            #print("ln" , len(set_of_all_neighbors))
            if len(neighbors) == 0:
                continue

       #     neighbors_weights=np.array([self.G[nodeid][neighbor]['weight'] for neighbor in neighbors])
        #    neighbors_weights=neighbors_weights / sum(neighbors_weights)

            if len(neighbors) > self.max_degree:
                neighbors=np.random.choice(neighbors, self.max_degree,replace=False)
            elif len(neighbors) < self.max_degree:
                neighbors=np.random.choice(neighbors, self.max_degree, replace=True)

            # time_A=time.time() - time_sampling_beg
            # print(" TIME A ", time_A)
          #  adj[self.id2idx[nodeid], :]=np.asarray(list(neighbors))
           # adj[self.id2idx[nodeid]]
          #   neighbors=set(neighbors)

            current_nbrs_counter_node = []
            for each_nbr in neighbors:
                    if (each_nbr not in dict_map_real_to_counter_node):
                        dict_map_real_to_counter_node[each_nbr]=node_counter
                        dict_map_couter_to_real_node[node_counter]=each_nbr
                        node_counter+=1
                    current_nbrs_counter_node.append(dict_map_real_to_counter_node[each_nbr])
                    pass
            # time_B=time.time() - time_sampling_beg
            # print(" TIME B ", time_B)

            adj[dict_map_real_to_counter_node[nodeid]] = current_nbrs_counter_node#np.asarray(current_nbrs_counter_node)
       #     print(nodeid,"----", current_nbrs_counter_node)
       #      time_Cminus=time.time() - time_sampling_beg
       #      print(" TIME C minus ", time_Cminus)

         #   list_top_nodes_and_sampled_neighbors.extend(current_nbrs_counter_node)
            #        print("top nodes sampled before     ", len(set_top_nodes_and_sampled_neighbors))
            # time_C=time.time() - time_sampling_beg
            # print(" TIME C ", time_C)

            # list_top_nodes_and_sampled_neighbors = list(set_top_nodes_and_sampled_neighbors)


            # if len(neighbors) > self.max_degree:
            #    neighbors = np.random.choice(neighbors, self.max_degree, replace=False)
            # elif len(neighbors) < self.max_degree:
            #   neighbors = np.random.choice(neighbors, self.max_degree, replace=True)
        set_top_nodes_and_sampled_neighbors = dict_map_couter_to_real_node.keys()

        time_sampling_end=time.time()
        time_sampling=time_sampling_end - time_sampling_beg

        print("sampling time ", time_sampling)
   #     set_top_nodes_and_sampled_neighbors =set(list_top_nodes_and_sampled_neighbors)
        print("top nodes sampled total ", len(set_top_nodes_and_sampled_neighbors))

        # CREATE FEATURES FOR TOP DEGREE NODES AND 2 LEVEL NEIGHBORS
        import time
        time_feature_start=time.time()
        all_nodes=self.all_nodes

        total_nodes=len(all_nodes)

        features=np.zeros((total_nodes, 1), dtype=np.float64)

        # weight_list = [0.1,0.01,0.001]

        sum_degree=0
        sum_weight_out=0.0

        node_weights_outgoing=[]

        # for node_i in all_nodes:
        #    node_weights_outgoing.append(0.0)

        # for node in set_top_nodes_and_sampled_neighbors:

        #   for nbr in self.G[node]:
        #     inf_prob = self.G[node][nbr]['weight']
        #      node_weights_outgoing[node] = node_weights_outgoing[node] + inf_prob
        # edge_weight = self.G[node][nbr]['weight']

        for counter_node_id in set_top_nodes_and_sampled_neighbors:
            # sum_degree = sum_degree + degree_of_nodes[node_i]
         #   counter_node_id = dict_map_real_to_counter_node[node_i]
          #  print("hi", dict_map_couter_to_real_node[counter_node_id])
            sum_weight_out=sum_weight_out + degree_of_nodes_all[dict_map_couter_to_real_node[counter_node_id]]

            # node_weights_outgoing[node_i]

        print("sum out", sum_weight_out)
      #  print(" type ", type(list_of_all_neighbors))

      #  print(" type ", set(list_of_all_neighbors))
        list_of_all_neighbors=list(set(list_of_all_neighbors))
        print(" len list of all neighbors", len(list_of_all_neighbors))
        print(" neighborhood sampling ", neighborhood_sampling)
        number_of_neighbors_reqd=int(neighborhood_sampling*len(list_of_all_neighbors))
        neighbors_sampled= set(np.random.choice(list_of_all_neighbors, number_of_neighbors_reqd,
                                           replace=False))
        print(" num of neighbors sampled", len(neighbors_sampled))
        
        for counter_node_id in set_top_nodes_and_sampled_neighbors:
            # norm_value = degree_of_nodes[node_i] * 1.0 / sum_degree


            norm_value_out=degree_of_nodes_all[dict_map_couter_to_real_node[counter_node_id]] * 1.0 / sum_weight_out

            # norm_value_out = node_weights_outgoing[node
            # _i] * 1.0 / sum_weight_out

            #     features[node_i][0] =norm_value
            features[counter_node_id][0]=norm_value_out


            if(dict_map_couter_to_real_node[counter_node_id ] in top_degree_nodes):
                neighbors=set(self.G.neighbors(dict_map_couter_to_real_node[counter_node_id]))
                sampled_nbrs_for_rl[dict_map_couter_to_real_node[counter_node_id]]=neighbors.intersection(neighbors_sampled)
            #    print(dict_map_couter_to_real_node[counter_node_id], "deg", degree_of_nodes_left[dict_map_couter_to_real_node[counter_node_id]], "len ",len(sampled_nbrs_for_rl[dict_map_couter_to_real_node[counter_node_id]]))

        # adj = adj[0:len(set_top_nodes_and_sampled_neighbors) +1]
        adj_counter = len(set_top_nodes_and_sampled_neighbors) * np.ones((len(set_top_nodes_and_sampled_neighbors) + 1, self.max_degree))

        for key, value in adj.items():
          #  print(key,value)
            adj_counter[key] = value

        adj = adj_counter
        features = features[0:len(set_top_nodes_and_sampled_neighbors)]


        top_degree_nodes = [ dict_map_real_to_counter_node[x] for x in list(top_degree_nodes)]


        time_feature_end=time.time()
        time_feature=time_feature_end - time_feature_start
        print("Feature time only ", time_feature)

        time_sampling_and_feature=time_sampling + time_feature

        print("sampling +feature time", time_sampling_and_feature)
      #  print(adj)

        print(type(adj), type(features))
        print(features.shape, adj.shape)


        # adj_using_counter =len(set_top_nodes_and_sampled_neighbors) * np.ones((len(set_top_nodes_and_sampled_neighbors) + 1, self.max_degree))
        # features_using_counter =np.zeros((len(set_top_nodes_and_sampled_neighbors), 1), dtype=np.float64)
        #
        # dict_map_real_to_counter_node = {}
        # node_counter = 0 # for useful nodes
        #
        # dict_map_couter_to_real_node = {}
        # for node_i in set_top_nodes_and_sampled_neighbors:
        #     if(node_i not in dict_map_real_to_counter_node):
        #         dict_map_real_to_counter_node[node_i] = node_counter
        #         dict_map_couter_to_real_node[node_counter] = node_i
        #         adj_using_counter[adj[node_counter, :]] =adj[node_i]
        #         features_using_counter[node_counter]  = features[node_i]
        #
        #         node_counter+=1
        #


        return adj, deg, features, list(top_degree_nodes), time_sampling_and_feature, dict_map_couter_to_real_node, sampled_nbrs_for_rl




    def end(self):
        return self.batch_num * self.batch_size >= len(self.train_nodes)

    def batch_feed_dict(self, batch_nodes, val=False):
        batch1id = batch_nodes
        batch1 = [self.id2idx[n] for n in batch1id]
              
        labels = np.vstack([self._make_label_vec(node) for node in batch1id])
        feed_dict = dict()
        feed_dict.update({self.placeholders['batch_size'] : len(batch1)})
        feed_dict.update({self.placeholders['batch']: batch1})
        feed_dict.update({self.placeholders['labels']: labels})

        return feed_dict, labels

    def node_val_feed_dict(self, size=None, test=False):
        if test:
            val_nodes = self.test_nodes
        else:
            val_nodes = self.val_nodes
        if not size is None:
            val_nodes = np.random.choice(val_nodes, size, replace=True)
        # add a dummy neighbor
        ret_val = self.batch_feed_dict(val_nodes)
        return ret_val[0], ret_val[1]

    def incremental_node_val_feed_dict_all_nodes(self, size, iter_num):
        val_nodes = self.all_nodes
        val_node_subset = val_nodes[iter_num*size:min((iter_num+1)*size, 
            len(val_nodes))]
      #  print(" val nodes  subset ", val_node_subset)
        # add a dummy neighbor
        ret_val = self.batch_feed_dict(val_node_subset)
        return ret_val[0], ret_val[1], (iter_num+1)*size >= len(val_nodes), val_node_subset

    def incremental_node_testing_feed_dict_nodes_top_degree(self, size, iter_num):
        val_nodes=list(self.top_degree_nodes)
        val_node_subset=val_nodes[iter_num * size:min((iter_num + 1) * size,
                                                      len(val_nodes))]

        # add a dummy neighbor
        ret_val=self.batch_feed_dict(val_node_subset)
        return ret_val[0], ret_val[1], (iter_num + 1) * size >= len(val_nodes), val_node_subset


    def incremental_node_val_feed_dict(self, size, iter_num, test=False):
        if test:
            val_nodes = self.test_nodes
        else:
            val_nodes = self.val_nodes
        val_node_subset = val_nodes[iter_num*size:min((iter_num+1)*size, 
            len(val_nodes))]

        # add a dummy neighbor
        ret_val = self.batch_feed_dict(val_node_subset)
        return ret_val[0], ret_val[1], (iter_num+1)*size >= len(val_nodes), val_node_subset

    def num_training_batches(self):
        return len(self.train_nodes) // self.batch_size + 1

    def next_minibatch_feed_dict(self):
        start_idx = self.batch_num * self.batch_size
        self.batch_num += 1
        end_idx = min(start_idx + self.batch_size, len(self.train_nodes))
        batch_nodes = self.train_nodes[start_idx : end_idx]
        return self.batch_feed_dict(batch_nodes)

    def incremental_embed_feed_dict(self, size, iter_num):
        node_list = self.nodes
        val_nodes = node_list[iter_num*size:min((iter_num+1)*size, 
            len(node_list))]
        return self.batch_feed_dict(val_nodes), (iter_num+1)*size >= len(node_list), val_nodes

    def shuffle(self):
        """ Re-shuffle the training set.
            Also reset the batch number.
        """
        self.train_nodes = np.random.permutation(self.train_nodes)
        self.batch_num = 0
