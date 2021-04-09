

import sys
import subprocess
import os
import psutil
import shutil
import pdb

def init_alloc_core():

	global allocated 
	
	total_cores = psutil.cpu_count()
	
	for i in range(total_cores):
	
		allocated[i] = 0

	return

def return_free_core():

	#select the core having minimum utilization

	global allocated
    
	core_usage_list =  psutil.cpu_percent(interval=1, percpu=True)

	core_index = -1

	min_val = 1000

	for i in range(len(core_usage_list)):

		if not allocated[i] and core_usage_list[i]<=min_val and core_usage_list[i]<=min_util:

			min_val = core_usage_list[i]

			core_index = i

	allocated[core_index] = 1
	
	return core_index


def run_experiment():

	init_alloc_core()
	#if(os.path.exists(dirname)):
	#	shutil.rmtree(dirname)
	#os.mkdir(dirname)

	process_stat = [None]*Cores
	
	for i in range(Cores):
	
		st_file = dirname+"/"+'stat'+str(i)+'.tmp'

		dt_file = dirname+"/"+'detailed'+str(i)+'.tmp'

		spread_file = dirname+"/"+'spread'+str(i)+'.tmp'

		core_index = return_free_core()
		
		cmd = "taskset -c {0} ".format(core_index)+" "+exec_file+" "+graph_file+" "+seed_file+" "+str(num_seeds)+" "+str(lamda)+" "+str(it/Cores)+" "+str(d)+" "+st_file+" "+dt_file+" "+spread_file

		#print cmd
		
		process_stat[i] = subprocess.Popen(cmd,shell=True)
		
	#sys.exit(1)
	exit_code = [p.wait() for p in process_stat]

	#print exit_code	
		
	return
	
def main():

	if len(sys.argv)<10:

		print "usage python experiments.py <exec-file> <graph-file> <seed-file> <num-seeds> <lambda> <#iter> <depth-for-seeds> <path-to-results-dir> <#cores>"

	global min_util,allocated,Cores, exec_file, graph_file, seed_file, num_seeds, lamda, it, d, dirname, first
	
	allocated = {}
	
	min_util = 10.0

	exec_file = sys.argv[1]
	graph_file = sys.argv[2]
	seed_file = sys.argv[3]
	num_seeds = int(sys.argv[4])
	lamda = int(sys.argv[5])
	it = int(sys.argv[6])
	d = int(sys.argv[7])
	dirname = sys.argv[8]
	Cores = int(sys.argv[9])
	first = False
	
	run_experiment()
	
	return


if __name__=='__main__':
    main()
