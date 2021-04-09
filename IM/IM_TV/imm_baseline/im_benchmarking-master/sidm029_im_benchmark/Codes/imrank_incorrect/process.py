import sys
import random
import numpy as np
list=[]
f = open(sys.argv[1],"r")
for line in f:
        line = line.strip()
	list.append(int(line))

i=0
numBins=100
binsize = int(sys.argv[2])
currlst=[]
while i<numBins:
	#print i
	j=0
	val=0
	while j<binsize:
		r = random.randint(0,9999)
		#print r, len(list)
		val+=list[r]	
		j+=1
	val =val*1.0/binsize
	currlst.append(val)
	i+=1

print np.std(currlst)
