import sys
loc = "../Results/freshRuns/processed/fig"
tech = sys.argv[1]
model = sys.argv[2]

files = []
if "celf" in tech and "ic" in model:
	files.append("4a")
	files.append("15a")
elif "celf" in tech and "wc" in model:
	files.append("4b")
        files.append("15a")
elif "celf" in tech and "lt" in model:
	files.append("4c")
        files.append("15a") 
elif "easyim" in tech and "ic" in model:
	files.append("4a")
        files.append("15a")
	files.append("15d")
        files.append("15g")
elif "easyim" in tech and "wc" in model:
	files.append("4b")
        files.append("15b")
	files.append("15e")
        files.append("15h")
elif "easyim" in tech and "lt" in model:
	files.append("4c")
        files.append("15c")
	files.append("15f")
        files.append("15i")
elif "imrank" in tech and "ic" in model:
	files.append("4d")
        files.append("15j")
	files.append("15l")
        files.append("15n")
elif "imrank" in tech and "wc" in model:
	files.append("4e")
        files.append("15k")
	files.append("15m")
        files.append("15o")
elif "pmc" in tech and "ic" in model:
	files.append("4f")
        files.append("16a")
	files.append("16c")
        files.append("16e")
elif "pmc" in tech and "wc" in model:
	files.append("4g")
        files.append("16b")
	files.append("16d")
        files.append("16e")
elif "sg" in tech and "ic" in model:
	files.append("4f")
        files.append("16a")
	files.append("16c")
        files.append("16e")
elif "sg" in tech and "wc" in model:
	files.append("4g")
        files.append("16b")
	files.append("16d")
        files.append("16e")
elif "imm" in tech and "ic" in model:
	files.append("4h")
        files.append("16g")
elif "imm" in tech and "wc" in model:
	files.append("4i")
        files.append("16h")
        files.append("16j")
        files.append("16l")
elif "imm" in tech and "lt" in model:
	files.append("4j")
        files.append("16i")
	files.append("16k")
        files.append("16m")
elif "tim" in tech and "ic" in model:
	files.append("4h")
        files.append("16g")
elif "tim" in tech and "wc" in model:
	files.append("4i")
        files.append("16g")
	files.append("16j")
        files.append("16l")
elif "tim" in tech and "lt" in model:
	files.append("4j")
        files.append("16i")
	files.append("16k")
	files.append("16m")

max = 0
for fval in files:
	f = open(loc+fval+"/"+tech+".txt")
	for line in f:
		line = line.strip()
		line = line.split()
		if(int(line[0])==200):
			v = int(line[1])
			if v >max:
				max = v
print max
