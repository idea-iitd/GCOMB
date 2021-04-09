#include "staticgreedy_basic.h"
#include "limit.h"
#include "graph.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>

int BasicStaticGreedy::n = 0;
int BasicStaticGreedy::m = 0;
int BasicStaticGreedy::top = 0;
int BasicStaticGreedy::size = 0;
//int BasicStaticGreedy::set[SET_SIZE] = {0};
int BasicStaticGreedy::set[MAX_K] = {0};
double BasicStaticGreedy::d[MAX_NODE] = {0};
int BasicStaticGreedy::list[MAX_NODE] = {0};
Edge BasicStaticGreedy::myEdges[MAX_EDGE];
int BasicStaticGreedy::index[MAX_NODE];
double BasicStaticGreedy::average_norelate[MAX_NODE];

void BasicStaticGreedy::Build()
{
  Graph::Build();
  n = Graph::GetN();
  m = Graph::GetM();
}

void BasicStaticGreedy::qsort_edges(int h, int t)
{
  if (h<t) 
  {
    int  i = h, j = t;
    Edge mid = myEdges[(i+j)/2];
    myEdges[(i+j)/2] = myEdges[i];

    while (i<j) 
    {
      while ((i<j) && ((myEdges[j].u>mid.u)||((myEdges[j].u==mid.u)&&(myEdges[j].v>mid.v))))
        j--;
      if (i<j) {
        myEdges[i] = myEdges[j];
        i++;
      }
      while ((i<j) && ((myEdges[i].u<mid.u)||((myEdges[i].u==mid.u)&&(myEdges[i].v<mid.v))))
        i++;
      if (i<j) {
        myEdges[j] = myEdges[i];
        j--;
      }
    }

    myEdges[i] = mid;
    qsort_edges(h, i-1);
    qsort_edges(i+1, t);
  }
}

//for both undirected graph or directed graph
void BasicStaticGreedy::GenerateSnapshot(BasicSnapshot* snapshots, int R)
{
  //Generate R Snapshots
  bool forward[MAX_EDGE] = {false};
  int* corresponding_temp = (int*)malloc(sizeof(int)*m);//record all the new location

  for (int yy=0; yy<R; yy++)
  {
      //printf("generate snapshot %d\n", yy);
      int i = 0;
      int count_mm = 0;
      for (i=0; i<m; i++)
      {
          if(Graph::edges[i].w1>0.0){
            forward[i] = false;
            double temp_p = 0;
            if ((double)rand()/(double)RAND_MAX < Graph::edges[i].w1)
                {
                  forward[i] = true;
                  count_mm++;
                }
               }
          }

      BasicEdge* myedges_temp = (BasicEdge*)malloc(sizeof(BasicEdge)*count_mm);

      int mm=0;
      for (int vv=0; vv<m; vv++){

        if(forward[vv])
        {
          myedges_temp[mm].u = Graph::edges[vv].u;
          myedges_temp[mm].v = Graph::edges[vv].v;
          mm++;
        }
      }
      
      
      int* index_temp = (int*)malloc(sizeof(int)*n);
      for (i=0; i<n; i++)
          index_temp[i] = -1;
      for (i=0; i<mm; i++)
          index_temp[myedges_temp[i].u] = i;
      for (i=0; i<n; i++)
        if (i!=0)
        {
          if (index_temp[i] < 0)
              index_temp[i] = index_temp[i-1];
        }


      snapshots[yy].edges = myedges_temp;
      snapshots[yy].index = index_temp;
    
    }
    free(corresponding_temp);
}

//calculate for both undirected graph and directed graph 
int* BasicStaticGreedy::GetSeeds(int R, int k)
{
  Build();
  //top = SET_SIZE;
  top = k;

  bool *used= new bool[n];
  memset(used, 0, sizeof(bool)*n);
  //int set[SET_SIZE];
  int set[MAX_K];

  double old = 0.0;

  double *improve=new double[n];
  int *lastupdate=new int[n];
  int *heap=new int[n];

  for (int i=0; i<n; i++)
  {
    heap[i] = i;
    lastupdate[i] = -1;
    improve[i] = (double)(n+1);
  }

  BasicSnapshot* snapshots = (BasicSnapshot*)malloc(sizeof(BasicSnapshot)*R);
  
  //generate G'i by removing each edge e=(u, v) from G with probability 1-p(u, v)
  GenerateSnapshot(snapshots, R);

  for (int i=0; i<top; i++)
  {
    int ccc = 0;

    while (lastupdate[heap[0]] != i)
    {
      ccc++;
      lastupdate[heap[0]] = i;
      set[i] = heap[0];
      improve[heap[0]] = GetInfluenceSpreadFromSnapshot(R, i+1, set, snapshots) - old;
    
      int x = 0;
      while (x*2+2<=n-i)
      {
        int newx=x*2+1;
        if ((newx+1<n-i) && (improve[heap[newx]]<improve[heap[newx+1]]))
          newx++;
        if (improve[heap[x]]<improve[heap[newx]])
        {
          int t=heap[x];
          heap[x] = heap[newx];
          heap[newx] = t;
          x = newx;
        }
        else
          break;
      }
    }

    used[heap[0]] = true;
    set[i] = heap[0];
    list[i] = heap[0];
    d[i] = improve[heap[0]];
    old+=d[i];

    heap[0] = heap[n-i-1];
    int x = 0;
    while (x*2+2<=n-i)
    {
      int newx=x*2+1;
      if ((newx+1<n-i) && (improve[heap[newx]]<improve[heap[newx+1]]))
        newx++;
      if (improve[heap[x]]<improve[heap[newx]])
      {
        int t=heap[x];
        heap[x] = heap[newx];
        heap[newx] = t;
        x = newx;
      }
      else
        break;
    }
  }

  delete[] heap;
  delete[] lastupdate;
  delete[] improve;
  delete[] used;

  for(int bb=0; bb<R; bb++){
    free(snapshots[bb].edges);
    free(snapshots[bb].index);  
  }
  free(snapshots);

  return list;
}

//get influence spread on R snapshots
double BasicStaticGreedy::GetInfluenceSpreadFromSnapshot(int R, int set_num, int* set, BasicSnapshot* snapshots){

  int  targetSize = 0;
  int  resultSize = 0;
  int  target[MAX_K]={0};  
  targetSize = set_num;
  for (int i=0; i<targetSize; i++)
    target[i] = set[i];
  resultSize = 0;

  int    h, t;
  int    *list  = new int[n];
  bool  *active= new bool[n];

  for (int it=0; it<R; it++)
  {
    int temp = 0;
    memset(active, 0, sizeof(bool)*n);
    for (int i=0; i<targetSize; i++) 
    {
      list[i] = target[i];
      //printf("target[%d]=%d", i, target[i]);
      //printf("active[target[%d]]=%s\n", i, active[target[i]]==false?"FALSE":"TRUE");
      active[target[i]] = true;
    }
    resultSize += targetSize;

    h = 0;
    t = targetSize;

    while (h<t) 
    {
      int k;
      if (list[h] == 0)
           k = snapshots[it].index[list[h]]+1;
         else 
           k = snapshots[it].index[list[h]]-snapshots[it].index[list[h]-1];

      for (int i=0; i<k; i++)
      {
        BasicEdge e;
        if (list[h] == 0)
                e =  snapshots[it].edges[i];
              else
                e = snapshots[it].edges[snapshots[it].index[list[h]-1]+1+i];

        if (active[e.v]) continue;

            list[t] = e.v;
            active[e.v] = true;
            t++;
            resultSize++;
            temp ++;
      }
      h++;
    }
  }
  delete[] active;
  delete[] list;
  
  double mean = (double)resultSize / (double)R;
  return mean;
}
