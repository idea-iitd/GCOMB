To train GCN:

Run
python GraphSAGE-master/train_multiple.py

Run
python GraphSAGE-master/predict_for_train.py


To train RL:
Run
python train_multipleRL.py



Testing

predict gsage embeddings

python predict_multiple_budgeted.py



Test RL:
python easy_pred_mul.py



Results are saved in 

GraphSAGE-master/real_data/"DATASET"/test/_bp-reward_RL"BUDGET"_nbs0.999

where DATASET is large_gowallah, large_graph_youtube
BUDGET is 15,25,50 etc.


To get results of total time, cover
run python GCOMB_RESULTS_PARSE_sampling_rate.py

Results are stored GCOMB_BUDGET_RESULTS/



MVC can me mapped to Max Cover Problem.

Given Graph 'G'
New bipartitite Graph G_bp:

For given graph G, for each node ni in graph G, create 2 nodes ni_1, ni_2. 
ni_1 belongs to left side of bi-partite graph G_bp.

ni_2 belongs to right side of bi-partite graph G_bp.


Consider two nodes 'ni' and 'nj' in G.

There is an edge between node ni_1 and nj_2 in G_bp if there is an edge between node 'ni' and 'nj' in G.





