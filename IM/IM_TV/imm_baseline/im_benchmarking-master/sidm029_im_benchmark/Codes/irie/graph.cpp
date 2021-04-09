#include "graph.h"
#include <fstream>
#include<iostream>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <vector>
#include <map>
#include <queue>
#include <fstream>
using namespace std;

bool Graph::built = false;
int  Graph::n = 0;
int  Graph::m = 0;
vector<int>  Graph::index;
vector<int> Graph::degree(MAX_NODE,0);
vector<Edge> Graph::edges;
vector<int> Graph::correspond(MAX_EDGE,0);

void Graph::qsort_edges(int h, int t)
{
  if (h<t) 
  {
    int i = h, j = t;
    Edge mid = edges[(i+j)/2];
    edges[(i+j)/2] = edges[i];

    while (i<j) 
    {
      while ((i<j) && ((edges[j].u>mid.u)||((edges[j].u==mid.u)&&(edges[j].v>mid.v))))
        j--;
      if (i<j) {
        edges[i] = edges[j];
        i++;
      }
      while ((i<j) && ((edges[i].u<mid.u)||((edges[i].u==mid.u)&&(edges[i].v<mid.v))))
        i++;
      if (i<j) {
        edges[j] = edges[i];
        j--;
      }
    }

    edges[i] = mid;
    qsort_edges(h, i-1);
    qsort_edges(i+1, t);
  }
}

void Graph::Build()
{
  if (built) 
    return;
  built = true;

  scanf("%ld %ld", &n, &m);  
  degree.resize(n);
  edges.resize(2*m);

  for (int i=0; i<m; i++)
  {
    scanf("%ld %ld", &edges[i].u, &edges[i].v);
    edges[i+m].u = edges[i].v;
    edges[i+m].v = edges[i].u;
    edges[i].c   = 1;
    edges[i+m].c = 1;
    degree[edges[i].u]++;
    degree[edges[i].v]++;
  }

  qsort_edges(0, 2*m-1);

  int m1 = 0;
  for (int i=1; i<2*m; i++)
  {
    if ((edges[i].u != edges[m1].u) || (edges[i].v != edges[m1].v))
    {
      m1++;
      edges[m1] = edges[i];
    }
    else 
    {
      edges[m1].c++;
    }
  }
  if (m!=0)
    m = m1+1;
  
  index.resize(n);
  for (int i=0; i<n; i++)
    index[i] = 0;
  for (int i=0; i<m; i++)
    index[edges[i].u] = i;
  for (int i=1; i<n; i++)
    if (index[i] < index[i-1])
      index[i] = index[i-1];
}

int Graph::GetN()
{
  if (!built)  Build();
  return n;
}

int Graph::GetM()
{
  if (!built)  Build();
  return m;
}

int Graph::GetDegree(int node)
{
  if (!built)  Build();
  return degree[node];
}

int Graph::GetNeighbor(int node)
{
  if (!built)  Build();
  if (node == 0)
    return index[node]+1;
  else 
    return index[node]-index[node-1];
}

Edge Graph::GetEdge(int node, int idx)
{
  if (!built)  Build();
  if (node == 0)
    return edges[idx];
  else
    return edges[index[node-1]+1+idx];
}

void Graph::BuildWC()
{
  for (int i=0; i<m; i++) {
    edges[i].w1=-log(1-pow(1-1.0/degree[edges[i].v], edges[i].c));
    edges[i].w2=-log(1-pow(1-1.0/degree[edges[i].u], edges[i].c));
  }
}

//////////////////////////////////////////////////////////////////////////////
//load graph with general UIC model
//network file format(note:each undirected edge should be transformed to two directed edges):
//first line:m n
//other lines:v1 v2 prob
//////////////////////////////////////////////////////////////////////////////
void Graph::BuildFromFile2GIC(char *file){
  
  if (built) 
    return;
  built = true;
  string fileloc = file;
  std::size_t found = fileloc.find_last_of("/");
  fileloc = fileloc.substr(0,found+1)+"attribute.txt";
  
 
        ifstream cin((fileloc).c_str());
            string s;
            while(cin >> s){
                if(s.substr(0,2)=="n="){
                    n=atoi(s.substr(2).c_str());
                    continue;
                }
                if(s.substr(0,2)=="m="){
                    m=atoi(s.substr(2).c_str());
                    continue;
                }
            }

 cout<<n<<" "<<m<<endl;


  FILE* in = fopen(file, "r");

  degree.resize(n);
  edges.resize(2*m);
  correspond.resize(2*m);

  for (int i=0; i<m; i++)
  {
    float p;
    fscanf(in,"%d %d %f\n", &edges[i].u, &edges[i].v, &p);
	 float w = -log(1-pow(1-p, 1));
    edges[i].w1 = w;
    edges[i].w2 = 20000;
    edges[i+m].u = edges[i].v;
    edges[i+m].v = edges[i].u;
    edges[i+m].w1 = 20000;
    edges[i+m].w2 = w;
    edges[i].c   = 1;
    edges[i+m].c = 1;
    degree[edges[i].u]++;
    degree[edges[i].v]++;
    correspond[i] = i+m;
    correspond[i+m] = i;
  }
  qsort_edges(0, 2*m-1);
  int newlocation[MAX_EDGE] = {0};
  int m1 = 0;
  for (int i=1; i<2*m; i++)
  {
	//cout<<i<<" "<<2*m<<endl;
     if ((edges[i].u != edges[m1].u) || (edges[i].v != edges[m1].v))
    {
      m1++;
      edges[m1] = edges[i];
    }
    else 
    {
      edges[m1].c++;
      if (edges[i].w1 < edges[m1].w1) edges[m1].w1 = edges[i].w1;
      edges[m1].w2 = min(edges[m1].w2,edges[i].w2);
    }
   newlocation[i] = m1;
  }
  for(int i=0; i<2*m; i++){
    correspond[newlocation[i]] = newlocation[correspond[i]];
  }

  if (m!=0)
    m = m1+1;
  
  index.resize(n);
  for (int i=0; i<n; i++)
    index[i] = 0;
  for (int i=0; i<m; i++)
    index[edges[i].u] = i;
  for (int i=1; i<n; i++)
    if (index[i] < index[i-1])
      index[i] = index[i-1];
  
  for (int i=0;i<m;i++)
  {
    edges[i].c = 1;
  }
}


void Graph::BuildIC()
{
  for (int i=0; i<m; i++) {
    edges[i].w1=-log(1-pow(1-0.01, edges[i].c));
    edges[i].w2=-log(1-pow(1-0.01, edges[i].c));
  }
}


void Graph::Build2WC(char file[])
{
  if (built) 
    return;
  built = true;

  ifstream in;

  in.open(file);
  
  in >> n;
  in >> m;
  degree.resize(n);
  edges.resize(2*m);

  for (int i=0; i<2*m; i++)
  {
    in >> edges[i].u;
    in >> edges[i].v;
    in >> edges[i].w1;
    in >> edges[i].w2;
    edges[i].u--;
    edges[i].v--;
    edges[i].w1=-log(edges[i].w1);
    edges[i].w2=-log(edges[i].w2);
    //edges[i+m].u = edges[i].v;
    //edges[i+m].v = edges[i].u;
    edges[i].c   = 1;
    //edges[i+m].c = 1;
    degree[edges[i].u]++;
    //degree[edges[i].v]++;
  }
  in.close();

  qsort_edges(0, 2*m-1);
  /*
  for (int i=0; i<2*m; i++)
  {
    printf("%d %d %lf %lf\n", edges[i].u, edges[i].v, exp(-edges[i].w1), exp(-edges[i].w2));
  }
  */

  int m1 = 0;
  for (int i=1; i<2*m; i++)
  {
    if ((edges[i].u != edges[m1].u) || (edges[i].v != edges[m1].v))
    {
      m1++;
      edges[m1] = edges[i];
    }
    else 
    {
      edges[m1].c++;
    }
  }
  if (m!=0)
    m = m1+1;
  
  index.resize(n);
  for (int i=0; i<n; i++)
    index[i] = 0;
  for (int i=0; i<m; i++)
    index[edges[i].u] = i;
  for (int i=1; i<n; i++)
    if (index[i] < index[i-1])
      index[i] = index[i-1];
    //else
    //  printf("%d %d\n",edges[i].u, index[i]);
  //BuildWC();
}

void Graph::Build2DWC()
{
  if (built) 
    return;
  built = true;

  scanf("%ld %ld", &n, &m);  
  degree.resize(n);
  edges.resize(2*m);

  for (int i=0; i<m; i++)
  {
    scanf("%ld %ld", &edges[i].u, &edges[i].v);
    //edges[i].u;
    //edges[i].v;
    edges[i].w1=0;
    edges[i].w2=20000;
    edges[i+m].u = edges[i].v;
    edges[i+m].v = edges[i].u;
    edges[i+m].w1=20000;
    edges[i+m].w2=0;
    edges[i].c   = 1;
    edges[i+m].c = 1;
    degree[edges[i].u]++;
    degree[edges[i].v]++;
  }

  qsort_edges(0, 2*m-1);

  int m1 = 0;
  for (int i=1; i<2*m; i++)
  {
    if ((edges[i].u != edges[m1].u) || (edges[i].v != edges[m1].v))
    {
      m1++;
      edges[m1] = edges[i];
    }
    else 
    {
      edges[m1].c++;
      if (edges[i].w1<edges[m1].w1) edges[m1].w1=edges[i].w1;
      edges[m1].w2=min(edges[m1].w2,edges[i].w2);
      //if (edges[i].u==141 && edges[i].v==0) printf("%d %lg %lg %lg\n",i,edges[m1].w2,edges[i].w2,min(edges[m1].w2,edges[i].w2));
    }
  }
  if (m!=0)
    m = m1+1;
  
  index.resize(n);
  for (int i=0; i<n; i++)
    index[i] = 0;
  for (int i=0; i<m; i++)
    index[edges[i].u] = i;
  for (int i=1; i<n; i++)
    if (index[i] < index[i-1])
      index[i] = index[i-1];
    //else
    //  printf("%d %d\n",edges[i].u, index[i]);
  //BuildWC();
  //for (int i=0;i<m;i++)
  //{
  //  int u=edges[i].u;
  //  int v=edges[i].v;
  //  int d=GetNeighbor(v);
  //  for (int j=0;j<d;j++)
  //  {
  //    Edge e=GetEdge(v,j);
  //    if (e.v==u){
  //      if (u==0 && v==141) printf("%d %lg\n",j,e.w2);
  //      if (e.w2!=edges[i].w1) {
  //        printf("%d %d %lg %lg\n",u,v,edges[i].w1,e.w2);
  //        exit(-1);
  //      }
  //    }
  //  }
  //}
  for (int i=0;i<m;i++)
  {
    if (edges[i].w1==0) edges[i].w1=-log(1-pow(1-1.0/degree[edges[i].v], 1));
    if (edges[i].w2==0) edges[i].w2=-log(1-pow(1-1.0/degree[edges[i].u], 1));
  }
}

void Graph::Build2GC()
{
  if (built) 
    return;
  built = true;

  
  scanf("%ld %ld", &n, &m);  
  degree.resize(n);
  edges.resize(m);
  for (int i=0; i<m; i++)
  {
    scanf("%ld %ld %lg %lg", &edges[i].u, &edges[i].v, &edges[i].w1, &edges[i].w2);
    //printf("%d %d %lg %lg\n",edges[i].u,edges[i].v,edges[i].w1,edges[i].w2);
    //edges[i].u--;
    //edges[i].v--;
    edges[i].w1=-log(edges[i].w1);
    edges[i].w2=-log(edges[i].w2);
    //edges[i+m].u = edges[i].v;
    //edges[i+m].v = edges[i].u;
    edges[i].c   = 1;
    //edges[i+m].c = 1;
    degree[edges[i].u]++;
    //degree[edges[i].v]++;
  }

  qsort_edges(0, m-1);

  int m1 = 0;
  //printf("%d %d\n",n,m);
  for (int i=1; i<m; i++)
  {
    if ((edges[i].u != edges[m1].u) || (edges[i].v != edges[m1].v))
    {
      m1++;
      edges[m1] = edges[i];
    }
    else 
    {
      edges[m1].c++;
    }
  }
  if (m!=0)
    m = m1+1;
  
  index.resize(n);
  for (int i=0; i<n; i++)
    index[i] = 0;
  for (int i=0; i<m; i++)
    index[edges[i].u] = i;
  for (int i=1; i<n; i++)
    if (index[i] < index[i-1])
      index[i] = index[i-1];
    //else
    //  printf("%d %d\n",edges[i].u, index[i]);
}
void Graph::Build2DIC(double prob1=0.1, double prob2=0.01)
{
  if (built) 
    return;
  built = true;

  
  scanf("%ld %ld", &n, &m);  
  degree.resize(n);
  edges.resize(m+m);
  for (int i=0; i<2*m; i++)
  {
    scanf("%ld %ld %lg %lg", &edges[i].u, &edges[i].v, &edges[i].w1, &edges[i].w2);
    //printf("%d %d %lg %lg\n",edges[i].u,edges[i].v,edges[i].w1,edges[i].w2);
    edges[i].u--;
    edges[i].v--;
    if (edges[i].w1>edges[i].w2)
    {
      edges[i].w1=-log(prob1);
      edges[i].w2=-log(prob2);
    } else {
      edges[i].w1=-log(prob2);
      edges[i].w2=-log(prob1);
    }
    //edges[i+m].u = edges[i].v;
    //edges[i+m].v = edges[i].u;
    edges[i].c   = 1;
    //edges[i+m].c = 1;
    degree[edges[i].u]++;
    //degree[edges[i].v]++;
  }

  qsort_edges(0, 2*m-1);

  int m1 = 0;
  //printf("%d %d\n",n,m);
  for (int i=1; i<2*m; i++)
  {
    if ((edges[i].u != edges[m1].u) || (edges[i].v != edges[m1].v))
    {
      m1++;
      edges[m1] = edges[i];
    }
    else 
    {
      edges[m1].c++;
    }
  }
  if (m!=0)
    m = m1+1;
  
  index.resize(n);
  for (int i=0; i<n; i++)
    index[i] = 0;
  for (int i=0; i<m; i++)
    index[edges[i].u] = i;
  for (int i=1; i<n; i++)
    if (index[i] < index[i-1])
      index[i] = index[i-1];
    //else
    //  printf("%d %d\n",edges[i].u, index[i]);
}

void Graph::Build2RIC(double prob1=0.1, double prob2=0.01)
{
  if (built) 
    return;
  built = true;

  
  scanf("%ld %ld", &n, &m);  
  degree.resize(n);
  edges.resize(m+m);
  map<int,map<int,bool>> pa;
  for (int i=0; i<m; i++)
  {
    scanf("%ld %ld", &edges[i].u, &edges[i].v);
    //printf("%d %d %lg %lg\n",edges[i].u,edges[i].v,edges[i].w1,edges[i].w2);
    //edges[i].u--;
    //edges[i].v--;
    //if (edges[i].w1>edges[i].w2)
    if (pa.find(edges[i].u)!=pa.end() && pa[edges[i].u].find(edges[i].v)!=pa[edges[i].u].end()) 
    {
      edges[i].w1=-log(prob1);
      edges[i].w2=-log(prob2); 
  //else {
      edges[i+m].u = edges[i].v;
      edges[i+m].v = edges[i].u;
      edges[i+m].w1=-log(prob2);
      edges[i+m].w2=-log(prob1);
      edges[i].c   = 1;
      edges[i+m].c = 1;
      degree[edges[i].u]++;
      degree[edges[i].v]++;
      continue;
    }
    if (pa.find(edges[i].v)!=pa.end() && pa[edges[i].v].find(edges[i].u)!=pa[edges[i].v].end()) 
    {
      //printf("%d %d\n", edges[i].u, edges[i].v);
      edges[i].w1=-log(prob2);
      edges[i].w2=-log(prob1);
      edges[i+m].u = edges[i].v;
      edges[i+m].v = edges[i].u;
      edges[i+m].w1=-log(prob1);
      edges[i+m].w2=-log(prob2);
      edges[i].c   = 1;
      edges[i+m].c = 1;
      degree[edges[i].u]++;
      degree[edges[i].v]++;
      continue;
    }  
    if (rand()<0.5*RAND_MAX)
    {
      pa[edges[i].u][edges[i].v]=true;
      edges[i].w1=-log(prob1);
      edges[i].w2=-log(prob2); 
  //else {
      edges[i+m].u = edges[i].v;
      edges[i+m].v = edges[i].u;
      edges[i+m].w1=-log(prob2);
      edges[i+m].w2=-log(prob1);
  //  }
    }
    else
    {
      pa[edges[i].v][edges[i].u]=true;
      edges[i].w1=-log(prob2);
      edges[i].w2=-log(prob1); 
  //else {
      edges[i+m].u = edges[i].v;
      edges[i+m].v = edges[i].u;
      edges[i+m].w1=-log(prob1);
      edges[i+m].w2=-log(prob2);
    }
    edges[i].c   = 1;
    edges[i+m].c = 1;
    degree[edges[i].u]++;
    degree[edges[i].v]++;
  }

  qsort_edges(0, 2*m-1);

  int m1 = 0;
  //printf("%d %d\n",n,m);
  for (int i=1; i<2*m; i++)
  {
    if ((edges[i].u != edges[m1].u) || (edges[i].v != edges[m1].v))
    {
      edges[m1].w1=-log(1-pow(1-exp(-edges[m1].w1),edges[m1].c));
      edges[m1].w2=-log(1-pow(1-exp(-edges[m1].w2),edges[m1].c));
      m1++;
      edges[m1] = edges[i];
    }
    else 
    {
      edges[m1].c++;
    }
  }
  if (m!=0)
    m = m1+1;
  
  index.resize(n);
  for (int i=0; i<n; i++)
    index[i] = 0;
  for (int i=0; i<m; i++)
    index[edges[i].u] = i;
  for (int i=1; i<n; i++)
    if (index[i] < index[i-1])
      index[i] = index[i-1];
    //else
    //  printf("%d %d\n",edges[i].u, index[i]);
  //printf("%d\n",GetNeighbor(19205));
}
void Graph::Build2TIC(double prob1=0.2, double prob2=0.04, double prob3=0.008)
{
  if (built) 
    return;
  built = true;

  
  scanf("%ld %ld", &n, &m);  
  degree.resize(n);
  edges.resize(m+m);
  map<int,map<int,bool>> pa;
  for (int i=0; i<2*m; i++)
  {
    double x1,x2;
    scanf("%ld %ld %lg %lg\n", &edges[i].u, &edges[i].v, &x1,&x2);
    //printf("%d %d %lg %lg\n",edges[i].u,edges[i].v,edges[i].w1,edges[i].w2);
    edges[i].u--;
    edges[i].v--;
    //if (edges[i].w1>edges[i].w2)
  //  if (pa.find(edges[i].u)!=pa.end() && pa[edges[i].u].find(edges[i].v)!=pa[edges[i].u].end()) 
  //  {
  ////    edges[i].w1=-log(prob1);
  ////    edges[i].w2=-log(prob2); 
  //////else {
  ////    edges[i+m].u = edges[i].v;
  ////    edges[i+m].v = edges[i].u;
  ////    edges[i+m].w1=-log(prob2);
  ////    edges[i+m].w2=-log(prob1);
  ////    edges[i].c   = 1;
  ////    edges[i+m].c = 1;
  ////    degree[edges[i].u]++;
  ////    degree[edges[i].v]++;
  //    continue;
  //  }
    if (i%2) 
    {
      ////printf("%d %d\n", edges[i].u, edges[i].v);
      //edges[i].w1=-log(prob2);
      //edges[i].w2=-log(prob1);
      //edges[i+m].u = edges[i].v;
      //edges[i+m].v = edges[i].u;
      //edges[i+m].w1=-log(prob1);
      //edges[i+m].w2=-log(prob2);
      //edges[i].c   = 1;
      //edges[i+m].c = 1;
      //degree[edges[i].u]++;
      //degree[edges[i].v]++;
      edges[i].w1=edges[i-1].w2;
      edges[i].w2=edges[i-1].w1;
      edges[i].c=1;
      continue;
    }  
    //pa[edges[i].u][edges[i].v]=true;
    int x=rand();
    if (x*3<RAND_MAX)
    {
      edges[i].w1=-log(prob1);
  //else {
  //  }
    }
    else if (x*3<2*RAND_MAX)
    {
      edges[i].w1=-log(prob2);
  //else {
    }
    else {
      edges[i].w1=-log(prob3);
    }
    x=rand();
    if (x*3<RAND_MAX)
    {
      edges[i].w2=-log(prob1); 
  //else {
  //  }
    }
    else if (x*3<2*RAND_MAX)
    {
      edges[i].w2=-log(prob2); 
    }
    else {
      edges[i].w2=-log(prob3); 
    }    
    edges[i].c   = 1;
    
    degree[edges[i].u]++;
    degree[edges[i].v]++;
  }

  qsort_edges(0, 2*m-1);

  int m1 = 0;
  //printf("%d %d\n",n,m);
  for (int i=1; i<2*m; i++)
  {
    if ((edges[i].u != edges[m1].u) || (edges[i].v != edges[m1].v))
    {
      //edges[m1].w1=-log(1-pow(1-exp(-edges[m1].w1),edges[m1].c));
      //edges[m1].w2=-log(1-pow(1-exp(-edges[m1].w2),edges[m1].c));
      m1++;
      edges[m1] = edges[i];
    }
    else 
    {
      edges[m1].c++;
      if (edges[i].w1<edges[m1].w1) edges[m1].w1=edges[i].w1;
      edges[m1].w2=min(edges[m1].w2,edges[i].w2);
    }
  }
  if (m!=0)
    m = m1+1;
  
  index.resize(n);
  for (int i=0; i<n; i++)
    index[i] = 0;
  for (int i=0; i<m; i++)
    index[edges[i].u] = i;
  for (int i=1; i<n; i++)
    if (index[i] < index[i-1])
      index[i] = index[i-1];
    //else
    //  printf("%d %d\n",edges[i].u, index[i]);
  //printf("%d\n",GetNeighbor(19205));
}

void Graph::Build2DT(double prob1=0.2, double prob2=0.04, double prob3=0.008)
{
  if (built) 
    return;
  built = true;

  
  scanf("%ld %ld", &n, &m);  
  degree.resize(n);
  edges.resize(m+m);
  map<int,map<int,bool>> pa;
  for (int i=0; i<m; i++)
  {
    //double x1,x2;
    scanf("%ld %ld\n", &edges[i].u, &edges[i].v);
    //printf("%d %d %lg %lg\n",edges[i].u,edges[i].v,edges[i].w1,edges[i].w2);
    int x=rand();
    if (x*3<RAND_MAX)
    {
      edges[i].w1=-log(prob1);
  //else {
  //  }
    }
    else if (x*3<2*RAND_MAX)
    {
      edges[i].w1=-log(prob2);
  //else {
    }
    else {
      edges[i].w1=-log(prob3);
    }
    edges[i].w2=20000; 
    edges[i].c   = 1;
    
    edges[i+m].u = edges[i].v;
    edges[i+m].v = edges[i].u;
    edges[i+m].w1=20000;
    edges[i+m].w2=edges[i].w1;
    edges[i].c   = 1;
    edges[i+m].c = 1;
    degree[edges[i].u]++;
    degree[edges[i].v]++;
  }

  qsort_edges(0, 2*m-1);

  int m1 = 0;
  //printf("%d %d\n",n,m);
  for (int i=1; i<2*m; i++)
  {
    if ((edges[i].u != edges[m1].u) || (edges[i].v != edges[m1].v))
    {
      //edges[m1].w1=-log(1-pow(1-exp(-edges[m1].w1),edges[m1].c));
      //edges[m1].w2=-log(1-pow(1-exp(-edges[m1].w2),edges[m1].c));
      m1++;
      edges[m1] = edges[i];
    }
    else 
    {
      edges[m1].c++;
      if (edges[i].w1<edges[m1].w1) edges[m1].w1=edges[i].w1;
      edges[m1].w2=min(edges[m1].w2,edges[i].w2);
    }
  }
  if (m!=0)
    m = m1+1;
  
  index.resize(n);
  for (int i=0; i<n; i++)
    index[i] = 0;
  for (int i=0; i<m; i++)
    index[edges[i].u] = i;
  for (int i=1; i<n; i++)
    if (index[i] < index[i-1])
      index[i] = index[i-1];
    //else
    //  printf("%d %d\n",edges[i].u, index[i]);
  //printf("%d\n",GetNeighbor(19205));
}

void Graph::Build2IC()
{
  if (built) 
    return;
  built = true;

  scanf("%ld %ld", &n, &m);  
  degree.resize(n);
  edges.resize(2*m);

  for (int i=0; i<2*m; i++)
  {
    scanf("%ld %ld %lg %lg", &edges[i].u, &edges[i].v, &edges[i].w1, &edges[i].w2);
    edges[i].u--;
    edges[i].v--;
    edges[i].w1=-log(0.01);
    edges[i].w2=-log(0.01);
    //edges[i+m].u = edges[i].v;
    //edges[i+m].v = edges[i].u;
    edges[i].c   = 1;
    //edges[i+m].c = 1;
    degree[edges[i].u]++;
    degree[edges[i].v]++;
  }

  qsort_edges(0, 2*m-1);

  int m1 = 0;
  for (int i=1; i<2*m; i++)
  {
    if ((edges[i].u != edges[m1].u) || (edges[i].v != edges[m1].v))
    {
      m1++;
      edges[m1] = edges[i];
    }
    else 
    {
      edges[m1].c++;
    }
  }
  if (m!=0)
    m = m1+1;
  
  index.resize(n);
  for (int i=0; i<n; i++)
    index[i] = 0;
  for (int i=0; i<m; i++)
    index[edges[i].u] = i;
  for (int i=1; i<n; i++)
    if (index[i] < index[i-1])
      index[i] = index[i-1];
    //else
    //  printf("%d %d\n",edges[i].u, index[i]);
}

void Graph::Build2T(double prob1=0.2, double prob2=0.04, double prob3=0.008)
{
  if (built) 
    return;
  built = true;

  
  scanf("%ld %ld", &n, &m);  
  degree.resize(n);
  edges.resize(m+m);
  map<int,map<int,bool>> pa;
  for (int i=0; i<m; i++)
  {
    //double x1,x2;
    scanf("%ld %ld\n", &edges[i].u, &edges[i].v);
    //printf("%d %d %lg %lg\n",edges[i].u,edges[i].v,edges[i].w1,edges[i].w2);
    int x=rand();
    if (x*3<RAND_MAX)
    {
      edges[i].w1=-log(prob1);
  //else {
  //  }
    }
    else if (x*3<2*RAND_MAX)
    {
      edges[i].w1=-log(prob2);
  //else {
    }
    else {
      edges[i].w1=-log(prob3);
    }
    x=rand();
    if (x*3<RAND_MAX)
    {
      edges[i].w2=-log(prob1);
  //else {
  //  }
    }
    else if (x*3<2*RAND_MAX)
    {
      edges[i].w2=-log(prob2);
  //else {
    }
    else {
      edges[i].w2=-log(prob3);
    }
    //edges[i].w2=20000; 
    edges[i].c   = 1;
    
    edges[i+m].u = edges[i].v;
    edges[i+m].v = edges[i].u;
    edges[i+m].w1=edges[i].w2;
    edges[i+m].w2=edges[i].w1;
    edges[i].c   = 1;
    edges[i+m].c = 1;
    degree[edges[i].u]++;
    degree[edges[i].v]++;
  }

  qsort_edges(0, 2*m-1);

  int m1 = 0;
  //printf("%d %d\n",n,m);
  for (int i=1; i<2*m; i++)
  {
    if ((edges[i].u != edges[m1].u) || (edges[i].v != edges[m1].v))
    {
      //edges[m1].w1=-log(1-pow(1-exp(-edges[m1].w1),edges[m1].c));
      //edges[m1].w2=-log(1-pow(1-exp(-edges[m1].w2),edges[m1].c));
      m1++;
      edges[m1] = edges[i];
    }
    else 
    {
      edges[m1].c++;
      edges[m1].w1=max(edges[i].w1,edges[i].w1);
      edges[m1].w2=max(edges[m1].w2,edges[i].w2);
    }
  }
  if (m!=0)
    m = m1+1;
  
  index.resize(n);
  for (int i=0; i<n; i++)
    index[i] = 0;
  for (int i=0; i<m; i++)
    index[edges[i].u] = i;
  for (int i=1; i<n; i++)
    if (index[i] < index[i-1])
      index[i] = index[i-1];
    //else
    //  printf("%d %d\n",edges[i].u, index[i]);
  //printf("%d\n",GetNeighbor(19205));
}


void Graph::Stats()
{
  printf("number of vertices:\t%d\n",n);
  printf("number of edges:\t%d\n",m/2);
  printf("density:\t%lg\n",double(m)/n/(n-1));
  int maxdegree=0;
  double tdegree=0.0;
  int i,j,k;
  for (i=0;i<n;i++)
  {
    if (degree[i]>maxdegree) maxdegree=degree[i];
    tdegree+=degree[i];
    //if (degree[i]%2) printf("%d\n", i);
  }
  printf("average degree:\t%lg\n",tdegree/n);
  printf("maximal degree:\t%d\n",maxdegree);
  int maxcmp=0,ncmp=0;
  bool *used=new bool[n];
  memset(used,0,n);
  while (1)
  {
    queue<int> q;
    for (i=0;i<n;i++)
      if (!used[i]) break;
    if (i==n) break;
    ncmp++;
    int cmpsize=0;
    q.push(i);
    used[i]=true;
    while (!q.empty())
    {
      k=q.front();
      q.pop();
      cmpsize++;
      j=GetNeighbor(k);
      for (i=0;i<j;i++)
      {
        Edge e=GetEdge(k,i);
        if (used[e.v]) continue;
        q.push(e.v);
        used[e.v]=true;
      }
    }
    if (cmpsize>maxcmp) maxcmp=cmpsize;
  }
  printf("# of connected component:\t%d\n",ncmp);
  printf("largest component size:\t%d\n",maxcmp);
  printf("average component size:\t%lg\n",double(n)/ncmp);
}
