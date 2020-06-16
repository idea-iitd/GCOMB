import os
import sys
test_true = 0# int(sys.argv[1])
print("test_true", test_true)
if(test_true==0):
    val_graphs = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
else:
    val_graphs = [1000, 2000, 3000, 4000, 5000, 10000, 20000]

print(val_graphs)

for val in val_graphs[0:]:

    command = "sh ./supervisedPredict.sh ./graph_data_syn/graph" + str(val) + "/graph" + str(val)
    os.system(command)
pass