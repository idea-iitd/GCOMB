#
import sys
import random
dataset="large_kite"
full_graph_edge_file_path = dataset+"/large_graph-G.json.edge"

train_sub_graph_edge_file_path = dataset+"/train/large_graph-G.json.edge"
val_sub_graph_edge_file_path = dataset+"/val/large_graph-G.json.edge"
test_sub_graph_edge_file_path = dataset+"/test/large_graph-G.json.edge"

train_point= 30
val_point =70

f_full_graph_edge = open(full_graph_edge_file_path,'r')
f_train_sub_graph_edge = open(train_sub_graph_edge_file_path,'w')
f_val_sub_graph_edge = open(val_sub_graph_edge_file_path,'w')

f_test_sub_graph_edge = open(test_sub_graph_edge_file_path,'w')

counter = 0

dict_edge_processed = {}

while True:
    #for line in file_handle.readline():
    line = f_full_graph_edge.readline()
   # lines-=1
 #   if lines <0:
  #      break
    if not line:
        break
    #print(line)
    n1, n2= line.replace('\n','').split(' ')
    if((n1,n2) in dict_edge_processed or (n2,n1) in dict_edge_processed):
        print(" already ")
        continue

    dict_edge_processed[(n1,n2)] =1

 #   n1, n2 = line.replace('\n','').split('\t')
    #print(n1,n2)
    random_int = random.randint(0, 100)
  #  break
    if random_int <train_point :
        f_train_sub_graph_edge.write(str(n1) +' ' + str(n2)+'\n')

    elif( random_int > train_point and random_int< val_point):
        f_val_sub_graph_edge.write(str(n1) +' ' + str(n2)+'\n')

    else:
        f_test_sub_graph_edge.write(str(n1) +' ' + str(n2) +'\n')
    counter+=1

    if(counter%10000==0):
        print(counter)

f_test_sub_graph_edge.close()
f_train_sub_graph_edge.close()


