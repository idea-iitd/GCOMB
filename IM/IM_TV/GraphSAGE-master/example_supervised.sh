echo --train_prefix $1 --model graphsage_meanpool --sigmoid
python -m graphsage.supervised_train --train_prefix $1 --model graphsage_meanpool --sigmoid