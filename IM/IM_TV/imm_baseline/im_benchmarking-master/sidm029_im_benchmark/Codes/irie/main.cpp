#include "limit.h"
#include "graph.h"
#include "general_cascade.h"
#include "memoryusage.h"
#include "ir.h"
#include "irie.h"
#include "heap.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//#include <io.h>
#include <string>
//#include <windows.h>
#include <sstream>
#include <fstream>
#include <iostream>

using namespace std;


void toSimulateOnce(int set_size, char *file, int (*GetNode)(int i), double (*Run)(int num_iter, int size, int set[]))
{
  ofstream out;
  out.open(file);
 
  int set[MAX_NODE];
  for (int t = 0; t< set_size; t++)
  {
    set[t] = GetNode(t);
  }
  out << set_size << '\t' <<  Run(10000, set_size, set) << endl;
  out.close();
}


int main(int argc, char * argv[])
{
  //srand(time_t(NULL));
  srand(1991);
 
  ofstream timeFile, seedFile;
  ofstream simulationFile;
  ofstream controlFile;
 
  char timeFileName[100];
  char simulationFileName[100];
  char controlFileName[100];
  
  if (argc <= 3)
  { 
   printf("-ir bound1 bound2 : IR with 1/theta from bound1 to bound 2\n");
   printf("-irie bound1 bound2 : IRIE with 1/theta from bound1 to bound 2\n");
   printf("Usage: ./exe algo set_size bound1 bound2 network_file\n");
   printf("example: ./mymain -ir 50 0.1 0.9 outputfolder hep\n");
   printf("example: ./mymain -irie 50 0.1 0.9 outputfolder hep\n");
   return 0;
  }
 
  int set_size = atoi(argv[2]);
  string s;

  s = "-ir";
  if (!s.compare(argv[1]))
  {
    //Influence Rank
    Graph::BuildFromFile2GIC(argv[argc - 1]);
    GeneralCascade::Build();
    printf("filename:%s\n", argv[argc - 1]);
    double bound1 = 0.1, bound2 = 0.9;
 
    if (argc == 7)
    {
      sscanf(argv[3],"%lf",&bound1);
      sscanf(argv[4],"%lf",&bound2);
    }
 
    sprintf(timeFileName, "time_IR_%s", argv[argc - 1]);
    timeFile.open(timeFileName);
 
    for (double bound = bound1; bound <= bound2; bound += 0.1)
    {
      int start_time = clock();
      IR::Build(set_size, bound, argv[argc - 1]);
      int end_time = clock();
      int time = end_time - start_time;
      printf("time:%d\n", time);
      timeFile << (double)(time)/CLOCKS_PER_SEC << " sec"<< endl;
      sprintf(simulationFileName, "sim_IR_%.1f_%s", bound, argv[argc - 1]);
      toSimulateOnce(set_size, simulationFileName, IR::GetNode, GeneralCascade::Run);
    }
    timeFile.close();
  }
 
  s = "-irie";
  if(!s.compare(argv[1]))
  {
    //Influence Rank Influence Estimation
    Graph::BuildFromFile2GIC(argv[argc - 1]);
    GeneralCascade::Build();
    double bound1 = 0.1, bound2 = 0.9;
    if (argc == 7)
    {
      sscanf(argv[3],"%lf",&bound1);
      sscanf(argv[4],"%lf",&bound2);
    }

    sprintf(timeFileName, "%s/statFileseeds-%s.txt", argv[5],argv[2]);
    timeFile.open(timeFileName);
    int start_time = clock();
    double alpha = IRIE::Build(set_size, bound1, bound2, argv[argc - 1]);
    int end_time = clock();
    int time = end_time - start_time;
    int set[MAX_NODE];
    
    char seedFileName[100];
    sprintf(seedFileName, "%s/seedFileseeds-%s.txt", argv[5],argv[2]);
    seedFile.open(seedFileName);
    for (int t = 0; t< set_size; t++)
    {
       set[t] = IRIE::GetNode(t);
       seedFile<<set[t]<<endl; 
    }
    seedFile.close(); 
     timeFile <<set_size<<" "<<GeneralCascade::Run(10000, set_size, set)<<" "<< (double)(time)/CLOCKS_PER_SEC<<" "<<disp_mem_usage("")<< endl;
    //sprintf(simulationFileName, "sim_IRIE_%.1f_i3", alpha);
    //toSimulateOnce(set_size, simulationFileName, IRIE::GetNode, GeneralCascade::Run);
    cout << "optimal alpha = " << alpha << endl;

    timeFile.close();
  }
  return 1;
}
