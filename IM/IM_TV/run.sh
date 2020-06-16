#!/bin/bash

# max coverage parameter k
k=15

# number of graphs on which you want to train
numEps=20

# dimension of embedding that you want to learn
dimEmbedding=2

# window size for n-step q learning
windowSize=2

# Learning rate of neural network
learning_rate=0.0005

# number of epochs for which you want to train your neural network
numEpochs=1

# batch size to be mentioned in the case of training of neural network
batchSize=10

echo main.py $k $numEps $dimEmbedding $windowSize $learning_rate $numEpochs $batchSize
python main.py $k $numEps $dimEmbedding $windowSize $learning_rate $numEpochs $batchSize
#15 20 2 2 0.0005 1 10


#15 5 1 3 0.0005 1 10