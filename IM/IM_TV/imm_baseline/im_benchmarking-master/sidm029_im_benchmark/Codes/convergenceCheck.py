import sys
from os import walk
from numpy import arange

#python conver..py <IMM 0.05 spread>
spread = {}
file = open(sys.argv[2],"r")
for line in file:
    line = line.strip()
    line = line.split()
    spread[int(line[0])] = float(line[1]) - float(line[2])


tech=sys.argv[1]

path = sys.argv[3]
#iterations = [10,25,50,75,100,250,500,750,1000,2500,5000,7500,10000]
#iterations = arange(1.00,0.0,-0.05)
#iterations = [10,25,50,75,100,125,150,175,200,250,300]
if "imrank" in tech:
	iterations = [1,2,3,4,5,6,7,8,9,10]
elif "imm" in tech or "tim" in tech:
	iterations = arange(1.00,0.0,-0.05)
elif "celf" in tech:
	iterations = [1000, 2500, 5000, 7500, 10000]
elif "easyim" in tech:
	iterations=[1, 2, 5, 10, 25, 50, 75, 100, 250, 500, 750, 1000]
elif "pmc" in tech or "static" in tech:
	iterations=[10, 25, 50, 75, 100, 125, 150, 175, 200, 250, 300]
fout = open(sys.argv[4],"w")
seeds=[1,2,5,10,25,50,75,100,125,150,175,200]

for k in seeds:
    val=[]
    std_dev = -1
    max=-1
    for it in iterations:
    #for it in arange(0.05,1.05,0.05):
        if it == 1.0:
			it = int(it)
	if "imrank" in tech or "celf" in tech or "easyim" in tech or "pmc" in tech  :
        	f = open(path+str(it)+".txt_300","r")
	elif "static" in tech or "imm" in tech or "tim" in tech:
        	f = open(path+str(it)+"_300","r")
        for line in f:
            line = line.strip()
            if(len(line)==0):
                continue

            line = line.split()
            
            if(int(line[0])==k):
                val.append(float(line[1]))
                
                if(float(line[1])  > max):
                    max = float(line[1])
                    std_dev = float(line[2])
    max_imm = max - std_dev#spread[k]
    #max_imm = max_imm - 0.01*max_imm
    #print k, val, max
    max_mine = val[-1]
    #max_mine = max_mine - 0.012*max_mine
    found_imm = -100
    found_mine = -100
#print val, max_imm

    for v in val:
        if(v >= max_imm):
            #print "found for k",k,v, iterations[val.index(v)]
            #print k, iterations[val.index(v)]
            found_imm = iterations[val.index(v)]
            break

    '''
    for v in val:
        if(v >= max_mine):
            #print k, iterations[val.index(v)]
            found_mine = iterations[val.index(v)]
            break
    '''
#print found_imm, found_mine, max_mine,v
    #if(found_imm > found_mine or found_imm==-100 ):
    #if(found_imm==-100 ):
    #    print k,found_mine
    #else:
    fout.write(str(k)+" "+str(found_imm)+"\n")
    print k,found_imm
fout.close()

#print spread

