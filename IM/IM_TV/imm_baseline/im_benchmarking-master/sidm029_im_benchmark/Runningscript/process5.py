import sys
technique=sys.argv[1]

loc=sys.argv[2]
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

val1=[]
val50=[]
val100=[]
val150=[]
val200=[]
for it in iterations:
	if "imrank" in tech or "celf" in tech or "easyim" in tech or "pmc" in tech  :
                f = open(path+str(it)+".txt_300","r")
        elif "static" in tech or "imm" in tech or "tim" in tech:
                f = open(path+str(it)+"_300","r")
	for line in f:
		line = line.strip()
		line = line.split()
		if(int(line[0])==1):
			val1.append(line[1])
		elif int(line[0])==50:
                        val50.append(line[1])
		elif int(line[0])==100:
                        val100.append(line[1])
		elif int(line[0])==150:
                        val150.append(line[1])
		elif int(line[0])==200:
                        val200.append(line[1])

fout = open(sys.argv[3],"w")
num=0
for it in iterations:
	fout.write(str(it)+" "+str(val1[num])+" "+str(val50[num])+" "+str(val100[num])+" "+str(val150[num])+" "+str(val200[num])+"\n")
	num+=1
fout.close()
print val1

