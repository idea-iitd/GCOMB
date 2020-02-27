To train SUP gsage

Run
 cd GraphSAGE-master
 python train_multiple.py

Run
cd GraphSAGE-master
python predicte_multiple_for_train.py


To train RL:
Run
python train_RL.py



Testing

predict gsage embeddings

cd GraphSAGE-master

python predict_multiple.py



Test RL:
python easy_testing.py



To evaluate spread:

Run python im_eval_spread_for_tv.py


#10 simulationy graphs are uploaded (GraphSAGE-master/real_data/youtube/TV/test/large_graph/mc_sim_graphs/)
# You can create more simulation graphs for calculating MC using
python spread_pre_process.py

