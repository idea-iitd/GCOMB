import sys
#1000 2500 5000 7500 10000 15000 20000

iterations = [1000, 2500, 5000, 7500, 10000, 15000, 20000]
#flowstat_0.1_10000_100
val=[]
std=[]
path = sys.argv[1]
for it in iterations:
        f = open(path+str(it)+"_300","r")
	for line in f:
		line = line.strip()
		line = line.split()
		if(int(line[0])==200):
			val.append(float(line[1]))
			std.append(float(line[2]))

fout = open(sys.argv[2],"w")
num=0
for it in iterations:
	fout.write(str(it)+" "+str(val[num])+" "+str(std[num])+"\n")
	num+=1
fout.close()

