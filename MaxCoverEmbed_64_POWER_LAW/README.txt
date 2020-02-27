To train SUP gsage

Run
python GraphSAGE-master/train_multiple.py

Run
python GraphSAGE-master/predict_for_train.py


To train RL:
Run
python train_RL.py



Testing

predict gsage embeddings

python predict_multiple.py



Test RL:
python easy_testing.py





MVC can me mapped to Max Cover Problem.

Given Graph 'G'
New bipartitite Graph G_bp:

For given graph G, for each node ni in graph G, create 2 nodes ni_1, ni_2. 
ni_1 belongs to left side of bi-partite graph G_bp.

ni_2 belongs to left side of bi-partite graph G_bp.


Consider two nodes 'ni' and 'nj' in G.

There is an edge between node ni_1 and nj_2 in G_bp if there is an edge between node 'ni' and 'nj' in G.





