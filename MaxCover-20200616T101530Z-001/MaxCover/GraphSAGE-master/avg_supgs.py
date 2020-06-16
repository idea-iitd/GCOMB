#
greedy_array =[]
for i in  range(2,10):
    path ='GraphSAGE-master/graph_data/graph{}/graph{}-G.json.greedy_reward15'.format(i,i)
    file_read = open(path, 'r')
    greedy_value=int(file_read.readlines()[0])
    print(greedy_value)
    greedy_array.append(greedy_value)

print("average ", sum(greedy_array)/len(greedy_array))