#

time_array = [x*300 for x in range(1,20)]
results_model_time_file = open('results_model_time.csv','w')

csv_line_text= ""
#print(time_value)
for i in time_array:
	csv_line_text+=str(i) +"\t"
results_model_time_file.write(csv_line_text)


for model_iter in [91]:#47,48,49,50,51,52,53, 60,61,62,63]:
	timeFile = 'timeElapseReward_{}.txt'.format(model_iter)
	fread= open(timeFile,'r').readlines()
	csv_line_text="\n"
	old_time, old_reward=fread[0].split('\t')
	old_time=float(old_time)+146.0
	for t in time_array:
		for line in fread:
			#print(line)
			#time, reward = line.split('\t')
			#print(time, reward)



			elapsed_time, reward=line.split('\t')
			elapsed_time=float(elapsed_time)+146.0
			# print(elapsed_time,t)
			if (elapsed_time > t):
				reward=old_reward

				print(t, elapsed_time, reward)
				reward=reward.replace('\n', '')
				csv_line_text+=reward + "\t"

				break
			else:
				# print("else")
				old_reward=reward

	results_model_time_file.write(csv_line_text)