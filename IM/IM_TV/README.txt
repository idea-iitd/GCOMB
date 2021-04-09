Follow instructions in  GraphSAGE-master/real_data/Readme_Download.txt

Download pre_trained GCN model : https://drive.google.com/drive/folders/1UcQ2PJqHo4QLR5pojz81KXeqwpTlWHYA?usp=sharing
Put it in GraphSAGE-master after extracting


Train:
sh train_script.sh

Test
sh test_script.sh




Summary:


To train SUP gsage

Run
 cd GraphSAGE-master
 python train_multiple.py

Run
python3 predicte_multiple_for_train.py


To train RL:
cd..
python train_RL.py



Testing

predict gsage embeddings

cd GraphSAGE-master

python3 predict_multiple.py



Test RL:
cd ..
python easy_testing.py



To evaluate spread:

Run python im_eval_spread_for_tv.py


#10 simulation graphs are uploaded (GraphSAGE-master/real_data/youtube/TV/test/large_graph/mc_sim_graphs/)
# You can create more simulation graphs for calculating MC using
python spread_pre_process.py

Change 10000 to 10 to run currently without creating more simulation graphs.




To evaluate for more datasets:
Create folder : GraphSAGE-master/real_data/DATASET_NAME/
Put edge file edges.txt in GraphSAGE-master/real_data/DATASET_NAME/

run python convto_nx.py DATASET_NAME


For example stack
python convto_nx.py stack


Download more datasets using
cd GraphSAGE-master/real_data
sh download_datasets.sh



We have used IMM for generating training labelled data since its relatively faster.

Run to get labels for training dataset
Default is youtube
 
./get_train_labels_single.sh



