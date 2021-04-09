#include<stdlib.h>
#include "limit.h"
#include "graph.h"
#include "random.h"
#include "degree.h"
#include "greedy.h"
#include "degreediscount_ic.h"
#include "weighteddegree.h"
#include "SPM_gc.h"
#include "SP1M_gc.h"
#include "pmia.h"
#include "pagerank.h"
#include "IMRank.h"
#include "general_cascade.h"
#include "mia.h"
#include "heap.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <iostream>
#include <fstream>
//#include <windows.h>

using namespace std;
#include <utility>                   // for std::pair
#include <algorithm>                 // for std::for_each
#include <sys/time.h>
#include <cassert>
#include <vector>
#include <list>
#include <unordered_map>
#include <math.h>
#include <ios>
#include <unistd.h>

//////////////////////////////////////////////////////////////////////////////
//
// process_mem_usage(double &, double &) - takes two doubles by reference,
// attempts to read the system-dependent data for a process' virtual memory
// size and resident set size, and return the results in KB.
//
// On failure, returns 0.0, 0.0

void process_mem_usage(double& vm_usage, double& resident_set)
{
   using std::ios_base;
   using std::ifstream;
   using std::string;

   vm_usage     = 0.0;
   resident_set = 0.0;

   // 'file' stat seems to give the most reliable results
   //
   ifstream stat_stream("/proc/self/stat",ios_base::in);

   // dummy vars for leading entries in stat that we don't care about
   //
   string pid, comm, state, ppid, pgrp, session, tty_nr;
   string tpgid, flags, minflt, cminflt, majflt, cmajflt;
   string utime, stime, cutime, cstime, priority, nice;
   string O, itrealvalue, starttime;

   // the two fields we want
   //
   unsigned long vsize;
   long rss;

   stat_stream >> pid >> comm >> state >> ppid >> pgrp >> session >> tty_nr
               >> tpgid >> flags >> minflt >> cminflt >> majflt >> cmajflt
               >> utime >> stime >> cutime >> cstime >> priority >> nice
               >> O >> itrealvalue >> starttime >> vsize >> rss; // don't care about the rest

   stat_stream.close();

   long page_size_kb = sysconf(_SC_PAGE_SIZE) / 1024; // in case x86-64 is configured to use 2MB pages
   vm_usage     = vsize / 1024.0;
   resident_set = rss * page_size_kb;
}

double disp_mem_usage(string msg){
    double vm, rss;

    process_mem_usage(vm, rss);
    vm/=1024;
    rss/=1024;
    //cout<< "Memory Usage:" << msg << " vm:" << vm << " MB  rss:" << rss << " MB"<<endl;
    cout<< "Memory Usage:" << msg << " " << rss << " MB"<<endl;
    return rss;
}

ofstream timetmpfile;
double timer;
clock_t start_time, end_time;
//print out the seeds for algorithms except imrank
void printSeeds(char *file, int (*GetNode)(int i), double time){
	//int set[200];
	int t;

	ofstream out;
	out.open(file,ios::out); 
	for (int t=0; t<SET_SIZE; t++)
	{
		out<<GetNode(t)<<endl;
		//fprintf(out, "%d\n", GetNode(t));
	}
	//fprintf(out,"\n");
	//fclose(out);
}

//print out the influence spread for algorithms except imrank
void printInfluence(char *file, int (*GetNode)(int i), double (*Run)(int num_iter, int size, int set[]), double time){
	
	int set[200];
	double influence[200];
	int t;

	for (int t=0; t<SET_SIZE; t++)
	{
		set[t] = GetNode(t);
		influence[t] = Run(20000, t+1, set);
	}

	FILE *out = fopen(file, "w");

	for (int t=0; t<SET_SIZE; t++)
	{
		fprintf(out, "%02d\t%f\n", t+1, influence[t]);
	}
	fprintf(out,"\n");
	fclose(out);
}

double toSimulateOnce(int setsize, int (*GetNode)(int i), double (*Run)(int num_iter, int size, int set[]))
{
	int set[MAX_NODE];
	int t;
	for (t=0; t<setsize; t++)
	{
		set[t] = GetNode(t);
	}
	return Run(NUM_ITER, t, set);
}

int main(int argc, char * argv[])
{
	time_t t;
	srand(1991);//(unsigned) time(&t));
	//srand((unsigned) time(&t));

	if (argc<=2) { 
		printf("-st : statistics of the weighted cascade graph\n");
		printf("-stg : statistics of the general independent cascade graph\n");
		printf("-b : baseline(random, degree, degreediscount, weighteddegree, pagerank) for general ic\n");
		printf("-g : greedy, SPM and SP1M for general ic\n");
		printf("-p bound1 bound2 : PMIA with 1/theta from bound1 to bound 2\n");
		printf("-m bound1 bound2 : MIA with 1/theta from bound1 to bound 2\n");
		printf("-imrank : IMRank algorithm\n");
		return 0;
	}

	string s;
	s="-stg";
	if (!s.compare(argv[1])) {
		Graph::Build2GC();
		Graph::Stats();
		//disp_mem_usage("");
	}

	char *filename ;
	string model = argv[3];
	filename =argv[4];//"./hep.txt";
	string outdir  = argv[5];	
	//build graph, need to be built according to different networks and models
	//Graph::BuildFromFile2WC(filename); 
	if (model=="IC")
	Graph::BuildFromFile2DT(filename);
	else
	Graph::BuildFromFile2WC(filename); 
	//Graph::Build2IC(filename); 
	GeneralCascade::Build();
	s="-st";
	if (!s.compare(argv[1])) {
		Graph::Build();
		Graph::Stats();
		//disp_mem_usage("");
	}

	s="-d";//Highest outDegree
	if (!s.compare(argv[1])) {
    		start_time = clock();
	     	Degree::Build();
	    	end_time = clock();
		
	    	timer = (double)(end_time-start_time);

            char time_rs[100]; 
     	    sprintf(time_rs, "time_degree.txt");
      	    //timetmpfile = fopen(time_rs, "w");
	   
	        //print time
	      //  timer = timer/1000000;
            //fprintf(timetmpfile,"%f\n",timer);
      	    //fclose(timetmpfile);

	        //print seeds
	        char seed_rs[100]; 
	        sprintf(seed_rs, "seed_degree.txt");
	        printSeeds(seed_rs, Degree::GetNode, timer);
	 	
	        //print influence
	        char influence_rs[100];
    	    sprintf(influence_rs, "influence_degree.txt");
    	    printInfluence(influence_rs, Degree::GetNode, GeneralCascade::Run, timer);
		//disp_mem_usage("");
	}

	s="-r";//Random
	if (!s.compare(argv[1])){
	    	start_time = clock();
	    	Random::Build();
       		end_time = clock();

    		timer = (double)(end_time-start_time);

			//print time
      		char time_rs[100];
       		sprintf(time_rs, "time_random.txt");
      		//timetmpfile = fopen(time_rs, "w");
       	  //  timer = timer/1000000;
           // fprintf(timetmpfile,"%f\n", timer);
    	//	fclose(timetmpfile);
		
	    	//print seeds
      	    char seed_rs[100];
	        sprintf(seed_rs, "seed_random.txt");
	        printSeeds(seed_rs,  Random::GetNode, timer);
	 	
	        //print influence
	       char influence_rs[100];
           sprintf(influence_rs, "influence_random.txt");
           printInfluence(influence_rs, Random::GetNode, GeneralCascade::Run, timer);
		//disp_mem_usage("");
	}

    s="-pr";//pagerank
	if (!s.compare(argv[1])){
    		start_time = clock();
    		pagerank::Build(SET_SIZE, 0.15);
    		end_time = clock();

	    	timer = (double)(end_time-start_time);

		    //print time
      		char time_rs[100]; 
       		sprintf(time_rs, "time_pagerank.txt");
      		//timetmpfile = fopen(time_rs, "w");
       	    timer = timer/1000000;
            //fprintf(timetmpfile,"%f\n", timer);
    	//	fclose(timetmpfile);

			//print seeds
      	    char seed_rs[100];
	        sprintf(seed_rs, "seed_pagerank.txt");
	        printSeeds(seed_rs,  pagerank::GetNode, timer);

		   //print influence
	       char influence_rs[100];
           sprintf(influence_rs, "influence_pagerank.txt");
           printInfluence(influence_rs, pagerank::GetNode, GeneralCascade::Run, timer);
		//disp_mem_usage("");
	}


	s="-g";//GreedyGC(improved by CELF)
	if (!s.compare(argv[1])) {
	    	int R_G = atoi(argv[7]);
	    	start_time = clock();
	    	Greedy::Build(SET_SIZE, R_G, GeneralCascade::Run);
	    	end_time = clock();

	    	timer = (double)(end_time-start_time);
		    
		    //print time
		    char time_rs[100];
       		sprintf(time_rs, "time_CELFGreedy.txt");
      		//timetmpfile = fopen(time_rs, "w");
       	    timer = timer/1000000;
            //fprintf(timetmpfile,"%f\n", timer);
    	//	fclose(timetmpfile);
		
			//print seeds
      	    char seed_rs[100]; 
	        sprintf(seed_rs, "seed_CELFGreedy.txt");
	        printSeeds(seed_rs,  Greedy::GetNode, timer);

		   //print influence
	       char influence_rs[100];
           sprintf(influence_rs, "influence_CELFGreedy.txt");
           printInfluence(influence_rs, Greedy::GetNode, GeneralCascade::Run, timer);
		//disp_mem_usage("");
	}

        
    s="-wd";//weighted degree
	if (!s.compare(argv[1])) {
    		start_time = clock();
       		WeightedDegree::Build();
	    	end_time = clock();

	    	timer = (double)(end_time-start_time);

		    char time_rs[100];
            sprintf(time_rs, "time_WeightedDegree.txt");
            //timetmpfile = fopen(time_rs, "w");
            timer = timer/1000000;
            //fprintf(timetmpfile,"%f\n",timer);
            //fclose(timetmpfile);
		
			//print seeds
      	    char seed_rs[100];
	        sprintf(seed_rs, "seed_WeightedDegree.txt");
	        printSeeds(seed_rs, WeightedDegree::GetNode, timer);
	 	
		   //print influence
	       char influence_rs[100];
           sprintf(influence_rs, "influence_WeightedDegree.txt");
           printInfluence(influence_rs, WeightedDegree::GetNode, GeneralCascade::Run, timer);
		//disp_mem_usage("");
	}

	s="-imrank";
	int l = atoi(argv[2]);
	char initialranking[] = "Degree";
	if(!s.compare(argv[1])){
     		imrank::Build(initialranking, l);


    		timer = imrank::timer;
 		char outfile[100],seedfile[100];
			cout<<outdir<<endl;
		  sprintf(outfile, "%s/statIMRank_k%d_l%d_LOOP%d_ir%s.txt",outdir.c_str(), SET_SIZE,l,IMRank_LOOP, initialranking);
 		 sprintf(seedfile, "%s/seedIMRank_k%d_l%d_LOOP%d_ir%s.txt",outdir.c_str(), SET_SIZE, l, IMRank_LOOP, initialranking);
		cout<<seedfile<<endl; 
		char time_rs[100];
       		sprintf(time_rs, "script_results/time_imrank_num%dseed=%dl=%d.txt",IMRank_LOOP, SET_SIZE,l);
      		timetmpfile.open(outfile,ios::out);// = fopen(outfile, "w");
       	    	//timer = timer/1000000;
       	    	timer = (float) timer / CLOCKS_PER_SEC;
		double mem =disp_mem_usage(""); 
            	cout<<timer<<endl;
            	cout<<mem<<endl;
		timetmpfile<<"-1 "<<timer<<" "<<mem<<endl;
		//fprintf(timetmpfile,"-1 %f %f\n", timer,mem);
    		//fclose(timetmpfile);

		    //print seeds
	        printSeeds(seedfile,  imrank::GetNode, timer);
		
		   //print influence
	    //   char influence_rs[100];
           //sprintf(influence_rs, "influence_imrank_%d.txt", l);
           //printInfluence(influence_rs, imrank::GetNode, GeneralCascade::Run, timer);
     }


	//control bound to test PMIA_GC
	s="-p";
	if (!s.compare(argv[1])) {
		int bound1=320, bound2=340;
		sscanf(argv[7],"%d",&bound1);
		sscanf(argv[8],"%d",&bound2);
		
		char timefilename[]="time_PMIA_0000.txt";
		char SPT_new_WC[]="GC_PMIA_0000.txt";
		for (int bound=bound1;bound<bound2; bound+=bound){
			printf("%d ",bound);
			double spread, treesize=0;
#ifdef COUNT
			{
				spread=SPT_new::Build(SET_SIZE, bound);
				printf("%f\n",spread);
				continue;
			}
#endif
		    	char time_rs[100];
        		sprintf(time_rs, "time_%s_%s_%s_PMIA_%04d.txt", argv[2], argv[3], argv[4], bound);
	    		{
	     			start_time = clock();
		    		treesize=SPT_new::Build(SET_SIZE, bound);
		    		end_time = clock();
			    
		    		timer = (double)(end_time-start_time);

             		//timetmpfile = fopen(time_rs, "a");
             	    timer = timer/1000000;
                    //fprintf(timetmpfile,"%f\n", timer);
    	        //	fclose(timetmpfile);

				   	//print seeds
            	    char seed_rs[100];
	                sprintf(seed_rs,  "seed_%s_%s_%s_PMIA_%04d.txt", argv[2], argv[3], argv[4], bound);
	                printSeeds(seed_rs,  SPT_new::GetNode, timer);

					//print influence
	                char influence_rs[100];
                    sprintf(influence_rs, "influence_%s_%s_%s_PMIA_%04d.txt", argv[2], argv[3], argv[4], bound);
                    printInfluence(influence_rs, SPT_new::GetNode, GeneralCascade::Run, timer);
			}
		}
	
		//disp_mem_usage("");
	}
}

