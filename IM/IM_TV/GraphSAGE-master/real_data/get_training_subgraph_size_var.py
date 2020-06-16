#
import sys
import random
dataset="youtube/TV"

full_graph_edge_file_path = dataset+"/train//edges.txt"

for size_var in [1,3,5,10,15,20,40,50,60,70,90,99]:

    train_sub_graph_edge_file_path = dataset+"/train{}/edges.txt".format(size_var)


    f_full_graph_edge = open(full_graph_edge_file_path,'r')
    f_train_sub_graph_edge = open(train_sub_graph_edge_file_path,'w')


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
        n1, n2, wt= line.replace('\n','').split(' ')
     #   if((n1,n2) in dict_edge_processed or (n2,n1) in dict_edge_processed):
          #  print(" already ")
      #      continue

        dict_edge_processed[(n1,n2)] =1

     #   n1, n2 = line.replace('\n','').split('\t')
        #print(n1,n2)
        random_int = random.randint(0, 100)
      #  break
        if random_int <size_var :
            f_train_sub_graph_edge.write(str(n1) +' ' + str(n2)+' '+ str(wt)+'\n')


        counter+=1

        if(counter%10000==0):
            print(counter)

    f_train_sub_graph_edge.close()


