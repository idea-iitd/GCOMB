To train SUP gsage

Run
 cd GraphSAGE-master
 python train_multiple.py

Run
cd GraphSAGE-master
python predicte_multiple_for_train.py


To train RL:
Run
python trainRL.py



Testing

predict gsage embeddings
cd GraphSAGE-master

python predict_multiple.py



Test RL:
python easy_testing.py



To evaluate spread:

Run python im_eval_spread_for_tv.py

The results of chosen nodes are present in

GraphSAGE-master/real_data/ "DATASET"/IC/large_graph-result_RL_"BUDGET"_nbs_"SAMPLINGRATE"

where DATASET is youtube, stack

BUDGET is 10,20,50,...200

SAMPLING RATE is 0.05 etc.
