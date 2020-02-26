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



The results of chosen nodes are present in

GraphSAGE-master/real_data/ "DATASET"/IC/large_graph-result_RL_"BUDGET"_nbs_"SAMPLINGRATE"

where DATASET is youtube, stack

BUDGET is 10,20,50,...200

SAMPLING RATE is 0.05 etc.


To evaluate spread:

cd Executables
Run python testingim_eval_spread_for_ic.py




To create test data for stack:

cd GraphSage-master/real_data/
run python conv_train_test_subgraph_type.py





