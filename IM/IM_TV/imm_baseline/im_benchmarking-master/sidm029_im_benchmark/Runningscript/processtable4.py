ldag=[]
simpath=[]
f = open("../Results/freshRuns/algorithmOutput/table4/ldag_nethept/stats_LT.txt","r")
for line in f:
	line = line.strip()
	line = line.split()
	if(int(line[0])==200):
		ldag.append(line[2])

f = open("../Results/freshRuns/algorithmOutput/table4/ldag_netheptP/stats_LT.txt","r")
for line in f:
        line = line.strip()
        line = line.split()
        if(int(line[0])==200):
                ldag.append(line[2])

f = open("../Results/freshRuns/algorithmOutput/table4/ldag_hepph/stats_LT.txt","r")
for line in f:
        line = line.strip()
        line = line.split()
        if(int(line[0])==200):
                ldag.append(line[2])

f = open("../Results/freshRuns/algorithmOutput/table4/ldag_dblp/stats_LT.txt","r")
for line in f:
        line = line.strip()
        line = line.split()
        if(int(line[0])==200):
                ldag.append(line[2])

f = open("../Results/freshRuns/algorithmOutput/table4/ldag_dblpP/stats_LT.txt","r")
for line in f:
        line = line.strip()
        line = line.split()
        if(int(line[0])==200):
                ldag.append(line[2])

f = open("../Results/freshRuns/algorithmOutput/table4/simpath_nethept/stats_LTNew_10000.txt","r")
for line in f:
        line = line.strip()
        line = line.split()
        if(int(line[0])==200):
                simpath.append(line[2])

f = open("../Results/freshRuns/algorithmOutput/table4/simpath_netheptP/stats_LTNew_10000.txt","r")
for line in f:
        line = line.strip()
        line = line.split()
        if(int(line[0])==200):
                simpath.append(line[2])

f = open("../Results/freshRuns/algorithmOutput/table4/simpath_hepph/stats_LTNew_10000.txt","r")
for line in f:
        line = line.strip()
        line = line.split()
        if(int(line[0])==200):
                simpath.append(line[2])

simpath.append("inf")
f = open("../Results/freshRuns/algorithmOutput/table4/simpath_dblpP/stats_LTNew_10000.txt","r")
for line in f:
        line = line.strip()
        line = line.split()
        if(int(line[0])==200):
                simpath.append(line[2])

print "NEthept netheptP hepph dblp dblpP"
print ' '.join(ldag)
print ' '.join(simpath)
