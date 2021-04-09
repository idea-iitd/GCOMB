#include "graph.h"
#include "general_cascade.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
//#include <time.h>

int GeneralCascade::n = 0;
int GeneralCascade::m = 0;
int  GeneralCascade::targetSize = 0;
int  GeneralCascade::resultSize = 0;
int  GeneralCascade::target[MAX_K]={0};
bool GeneralCascade::built = false;
double GeneralCascade::mean = 0;
double GeneralCascade::var = 0;
double GeneralCascade::std = 0;
double GeneralCascade::cv =0;

void GeneralCascade::Build()
{
  n = Graph::GetN();
  m = Graph::GetM();
  built = true;
}

void GeneralCascade::SetTarget(int size, int set[])
{
  if (!built) Build();
  targetSize = size;
  for (int i=0; i<size; i++)
    target[i] = set[i];
  resultSize = 0;
}

double GeneralCascade::Run(int num_iter, int size, int set[])
{
  SetTarget(size, set);

  int    h, t;
  int    *list  = new int[n];
  bool  *active= new bool[n];
  //double  *len = new double[20000];
  double *temp_spread = (double*)malloc(sizeof(double)*num_iter);

  mean = 0; 
  var = 0;
  std = 0;
  cv = 0;

  for (int it=0; it<num_iter; it++)
  {
    int temp = 0;
    memset(active, 0, sizeof(bool)*n);
    for (int i=0; i<targetSize; i++) 
    {
      list[i] = target[i];
      active[target[i]] = true;
      
    }
    resultSize += targetSize;

    h = 0;
    t = targetSize;

    while (h<t) 
    {
      int k = Graph::GetNeighbor(list[h]);
      //printf("%d \n",k);
      for (int i=0; i<k; i++)
      {
        Edge e = Graph::GetEdge(list[h], i);
        if (active[e.v]) continue;

        //if (((double)rand()/(double)RAND_MAX) < exp(-e.w1))
        if (((double)rand()/(double)RAND_MAX) < e.w1)
          {
            list[t] = e.v;
            active[e.v] = true;
            t++;
            resultSize++;
            temp ++;
            
          }
      }
      h++;
    }
    temp_spread[it] = (temp+targetSize);
    
  }
  delete[] active;
  delete[] list;
  
  mean = (double)resultSize / (double)num_iter;

  for(int c=0; c<num_iter; c++)
  {
    var += (mean-temp_spread[c])*(mean-temp_spread[c]);
  }

  var = var/num_iter;
  std = sqrt(var);
  cv = std/mean;
  free(temp_spread);
  //printf("\tvar\t%f\t%f\t%f\t%f\t", aver, var , sqrt(var/num_iter), std, cv);
  //return (double)resultSize / (double)num_iter;
  return mean;
}
