# !/bin/sh
# Hyperparameters are: k, numEps, dimEmbedding, windowSize, learning_rate, numEpochs, batchSize
# max-coverage parameter k
# number of graphs on which you want to train
# dimension of embedding that you want to learn
# window size for n-step q learning
# Learning rate of neural network
# number of epochs for which you want to train your neural network

# Things to take care:
# 1. Each model should be saved in a different file

for k in 30 40 50
do
	for numEps in 1 2 4
	do
		for dimEmbedding in 128 256
		do
			for windowSize in 4 8
			do
				for learning_rate in 0.0001 0.0005
				do
					for numEpochs in 2 4
					do
						python main.py $k $numEps $dimEmbedding $windowSize $learning_rate $numEpochs $batchSize
					done
				done
			done
		done
	done
done
