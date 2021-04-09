#include "graph.h"
#include <stdio.h>
#include <fstream>
#include <string.h>
#include <math.h>
#include <cstdlib>
#include <vector>
#include <map>
#include <queue>
#include <time.h>
using namespace std;

bool Graph::built = false;
int  Graph::n = 0;
int  Graph::m = 0;
vector<int>  Graph::index;
vector<int> Graph::degree(MAX_NODE,0);
vector<int> Graph::indegree(MAX_NODE,0);
vector<int> Graph::outdegree(MAX_NODE,0);
vector<Edge> Graph::edges;
vector<int> Graph::correspond(MAX_EDGE,0);

void Graph::qsort_edges(int h, int t)
{
  if (h<t) 
  {
    //srand(1991);//(unsigned)time(NULL));
    int  i = h, j = t;

    int e=rand()%(t-h+1)+h;

    Edge mid = edges[e];//edges[(i+j)/2];
    int midc = correspond[e];//correspond[(i+j)/2];
    edges[e] = edges[i];//edges[(i+j)/2] = edges[i];
    if (midc != i)
    {
      correspond[correspond[i]] = e;//(i+j)/2;
      correspond[e] = correspond[i];
    }
    else
      midc = e;

    while (i<j) 
    {
      while ((i<j) && ((edges[j].u>mid.u)||((edges[j].u==mid.u)&&(edges[j].v>mid.v))))
        j--;
      if (i<j) {
        edges[i] = edges[j];
        if (j!=midc)
        {
          correspond[i] = correspond[j];
          correspond[correspond[j]] = i;
        }
        else 
          midc = i;
        i++;
      }
      while ((i<j) && ((edges[i].u<mid.u)||((edges[i].u==mid.u)&&(edges[i].v<mid.v))))
        i++;
      if (i<j) {
        edges[j] = edges[i];
        if (i!=midc) {
          correspond[j] = correspond[i];
          correspond[correspond[i]] = j;
        }
        else
          midc = j;
        j--;
      }
    }

    edges[i] = mid;
    correspond[i] = midc;
    correspond[midc] = i;
    qsort_edges(h, i-1);
    qsort_edges(i+1, t);
  }
}

//mergesort
void Graph::mergesort_edges(int start, int end){
  
  if(start < end){
    int mid = (start+end)/2;
    mergesort_edges(start, mid);
    mergesort_edges(mid+1, end);
        merge(start, mid, end);
  }


}

void Graph::merge_edges(int start, int middle, int end){
  
  vector<Edge> edges_temp;
  edges_temp.resize(2*m);

  vector<int> newindex(MAX_EDGE,0);

  int keymiddle = middle;
  middle = middle+1;
  for(int i=0; i<2*m; i++){
    if(( middle>=(2*m)) || (((edges[start].u<edges[middle].u) || ((edges[start].u == edges[middle].u) && (edges[start].v < edges[middle].v)) && start<=keymiddle))){
      edges_temp[i] = edges[start];
      //change corresponding
      newindex[start] = i;
      start++;

    }else{
      edges_temp[i] = edges[middle];
      newindex[middle] = i;
      if(middle == 2*m-1)
        printf("middle = %d\n", middle);
      middle++;
    }
    
  }

  //copy edges and update correspond
  for(int i=0; i<2*m; i++){
    edges[i] = edges_temp[i];
    correspond[i] = newindex[correspond[i]];
  }

  edges_temp.clear();
  newindex.clear();
}

void Graph::merge(int start, int middle, int end){
  vector<Edge> edges_temp;
  int len = end-start+1;
  edges_temp.resize(len);
  
  vector<int> index_new(MAX_EDGE,0);
  vector<int> index_old(MAX_EDGE,0);

  int keymiddle = middle;
  middle = middle+1;
  for(int i=start; i<=end; i++){
    if(( middle > end) || (((edges[start].u<edges[middle].u) || 
      ((edges[start].u == edges[middle].u) && 
      (edges[start].v < edges[middle].v)) && start<=keymiddle))){
      edges_temp[i] = edges[start];
      //change corresponding
      index_new[start] = i;
      index_old[i] = start;
      start++;
    }else{
      edges_temp[i] = edges[middle];
      index_new[middle] = i;
      index_old[i] = middle;
      //change corresponding
      //correspond[correspond[middle]]= i;
      middle++;
    }
    
  }

  //copy edges and update correspond
  for(int i=start; i<=end; i++){
    edges[i] = edges_temp[i];// new edge
    correspond[i] = index_new[correspond[i]];
    
  }

  edges_temp.clear();
  index_new.clear();
  index_old.clear();
}

void Graph::QuickSort(int p,int r){
    if (p<r){
        int q=partion(p,r);
        QuickSort(p,q-1);
        QuickSort(q+1,r);
    }
}

int Graph::partion(int p,int r){
  time_t t;
  //srand((unsigned)time(&t));

  /*
    int e=rand()%(r-p+1)+p;
    Edge tem;
    tem=a[e];
    a[e]=a[r];
    a[r]=tem;
    int x=a[r], i=p-1;
    for (int j=p;j<r;j++){ 
        if (a[j]<=x){
            tem=a[i+1];
            a[i+1]=a[j];
            a[j]=tem;
            i++;
        }
    }
    tem=a[r];
    a[r]=a[i+1];
    a[i+1]=tem;*/
    return 1;
}
 

void Graph::insertsort_edges(int end)
{
  int i;
  for( i=1; i<end; i++){

    //if(i%5000==0)
      //printf("i: %d \n", i);
    //to insert the i-th element
    Edge temp_edge = edges[i];
    int temp_corres = correspond[i];
    int j = i-1;
    for(; j>=0; j--){
      if(edges[j].u > temp_edge.u)
      {
        edges[j+1] = edges[j];
        //change the correspond index
        correspond[correspond[j]] = j+1;
        correspond[j+1] = correspond[j];
      }else if((edges[j].u == temp_edge.u) && (edges[j].v > temp_edge.v)){
        edges[j+1] = edges[j];
        //change the correspond index
        correspond[correspond[j]] = j+1;
        correspond[j+1] = correspond[j];
      }
      else
        break;
    }

    if(j!=i-1){
         edges[j+1] = temp_edge;
        //change the correspond index
        correspond[j+1] = temp_corres;
        correspond[temp_corres] = j+1;
    }
  }
}

void Graph::Build()
{
  if (built) 
    return;
  built = true;

  scanf("%d %d", &n, &m);  
  degree.resize(n);
  edges.resize(2*m);

  for (int i=0; i<m; i++)
  {
    scanf("%d %d", &edges[i].u, &edges[i].v);
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

int Graph::GetOutDegree(int node)
{
  if (!built)  Build();
  return outdegree[node];
}

int Graph::GetInDegree(int node)
{
  if (!built)  Build();
  return indegree[node];
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

void Graph::BuildFromFile2DWC(char *file){
  
  if (built) 
    return;
  built = true;

  FILE* in = fopen(file, "r");
  fscanf(in,"%d %d", &n, &m);  

  degree.resize(n);
  indegree.resize(n);
  outdegree.resize(n);
  edges.resize(2*m);
  correspond.resize(2*m);


  for (int i=0; i<m; i++)
  {
    fscanf(in,"%d %d", &edges[i].u, &edges[i].v);
    edges[i].w1 = 0;
    edges[i].w2 = -1.0;
    edges[i+m].u = edges[i].v;
    edges[i+m].v = edges[i].u;
    edges[i+m].w1 = -1.0;
    edges[i+m].w2 = 0;
    edges[i].c   = 1;
    edges[i+m].c = 1;
    degree[edges[i].u]++;
    degree[edges[i].v]++;
    indegree[edges[i].v]++;
    outdegree[edges[i].u]++;
    correspond[i] = i+m;
    correspond[i+m] = i;
  }

  qsort_edges(0, 2*m-1);
  int newlocation[MAX_EDGE] = {0};
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
      if (edges[i].w1 > edges[m1].w1) edges[m1].w1 = edges[i].w1;
      edges[m1].w2 = max(edges[m1].w2,edges[i].w2);
      //if (edges[i].w1<edges[m1].w1) edges[m1].w1=edges[i].w1;
      //edges[m1].w2=min(edges[m1].w2,edges[i].w2);
    }
    newlocation[i] = m1;
  }

  
  for(int i=0; i<2*m; i++){
    correspond[newlocation[i]] = newlocation[correspond[i]];
  }

  if (m!=0)
    m = m1+1;
  
  index.resize(n);
  for (int i = 0; i<n; i++)
    index[i] = 0;
  for (int i = 0; i<m; i++)
    index[edges[i].u] = i;
  for (int i = 1; i<n; i++)
    if (index[i] < index[i-1])
      index[i] = index[i-1];
  
  for(int i=0; i<m; i++)
  {
    if(indegree[edges[i].v]==0)
      printf("%f",1.0/indegree[edges[i].v]);
    if(indegree[edges[i].u]==0)
      printf("%f",1.0/indegree[edges[i].u]);
  }

  for (int i=0;i<m;i++)
  {
    edges[i].c = 1;//this sentence is important
    if (edges[i].w1==0) {
      edges[i].w1 = 1.0/indegree[edges[i].v];
      //edges[i].w1=-log(1-pow(1-1.0/indegree[edges[i].v], 1));
    }
    if (edges[i].w2==0) { 
      edges[i].w2 = 1.0/indegree[edges[i].u];
      //edges[i].w2=-log(1-pow(1-1.0/indegree[edges[i].u], 1));
    }
  }
}


void Graph::BuildFromFile2WC(char* file){
  
  if (built) 
    return;
  built = true;

  FILE* in = fopen(file, "r");
  fscanf(in,"%d %d", &n, &m);  

  degree.resize(n);
  indegree.resize(n);
  outdegree.resize(n);
  edges.resize(2*m);
  correspond.resize(2*m);

  for (int i=0; i<m; i++)
  {
    fscanf(in,"%d %d", &edges[i].u, &edges[i].v);
    edges[i].w1 = 0;
    edges[i].w2 = -1.0;
    edges[i+m].u = edges[i].v;
    edges[i+m].v = edges[i].u;
    edges[i+m].w1 = -1.0;
    edges[i+m].w2 = 0;
    edges[i].c   = 1;
    edges[i+m].c = 1;
    degree[edges[i].u]++;
    degree[edges[i].v]++;
    indegree[edges[i].v]++;
    indegree[edges[i].u]++;
    outdegree[edges[i].u]++;
    outdegree[edges[i].v]++;
    correspond[i] = i+m;
    correspond[i+m] = i;
  }

  qsort_edges(0, 2*m-1);
  int newlocation[MAX_EDGE] = {0};
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
      if (edges[i].w1 > edges[m1].w1) edges[m1].w1 = edges[i].w1;
      edges[m1].w2 = max(edges[m1].w2,edges[i].w2);
      //if (edges[i].w1<edges[m1].w1) edges[m1].w1=edges[i].w1;
      //edges[m1].w2=min(edges[m1].w2,edges[i].w2);
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
      edges[i].w1 = 1.0/indegree[edges[i].v];
      edges[i].w2 = 1.0/indegree[edges[i].u];
      //edges[i].w1=-log(1-pow(1-1.0/indegree[edges[i].v], edges[i].c));
      //edges[i].w2=-log(1-pow(1-1.0/indegree[edges[i].u], edges[i].c));
  }
}


//////////////////////////////////////////////////////////////////////////////
//load graph with general UIC model
//network file format(note:each undirected edge should be transformed to two directed edges):
//first line:m n
//other lines:v1 v2 prob
//////////////////////////////////////////////////////////////////////////////

void Graph::BuildFromFile2GIC(char *file){
  if (built){ 
    return;
  }else{
      built = true;
  }


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



  FILE* in = fopen(file, "r");
  /*input is: node number and edge number*/
  //fscanf(in,"%d %d", &n, &m);
  printf("network info:%d  %d\n", n, m);
  /*reset the size of degree and edges*/
  degree.resize(n);
  indegree.resize(n);
  outdegree.resize(n);

  edges.resize(2*m);
  correspond.resize(2*m);

  if(in == NULL){
    return;
  }

  for (int i=0; i<m; i++)
  {
    float w;
    fscanf(in,"%d %d %f\n", &edges[i].u, &edges[i].v, &w);
    edges[i].w1 = w;
    edges[i].w2 = -1.0;
    edges[i+m].u = edges[i].v;
    edges[i+m].v = edges[i].u;
    edges[i+m].w1 = -1.0;
    edges[i+m].w2 = w;
    edges[i].c = 1;
    edges[i+m].c = 1;
    degree[edges[i].u]++;
    degree[edges[i].v]++;
    indegree[edges[i].v]++;
    outdegree[edges[i].u]++;
    correspond[i] = i+m;
    correspond[i+m] = i;
  }
  sortEdges();
}


void Graph::BuildFromFile2UC(char *file, double p){
  
  if (built) 
    return;
  built = true;

  FILE* in = fopen(file, "r");
  fscanf(in,"%d %d", &n, &m);  

  degree.resize(n);
  indegree.resize(n);
  outdegree.resize(n);
  edges.resize(2*m);
  correspond.resize(2*m);

  for (int i=0; i<m; i++)
  {
    fscanf(in,"%d %d", &edges[i].u, &edges[i].v);
    edges[i].w1 = 0;
    edges[i].w2 = -1.0;
    edges[i+m].u = edges[i].v;
    edges[i+m].v = edges[i].u;
    edges[i+m].w1 = -1.0;
    edges[i+m].w2 = 0;
    edges[i].c   = 1;
    edges[i+m].c = 1;
    degree[edges[i].u]++;
    degree[edges[i].v]++;
    indegree[edges[i].v]++;
    indegree[edges[i].u]++;
    outdegree[edges[i].u]++;
    outdegree[edges[i].v]++;
    correspond[i] = i+m;
    correspond[i+m] = i;
  }

  qsort_edges(0, 2*m-1);

  int newlocation[MAX_EDGE] = {0};
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
      if (edges[i].w1 > edges[m1].w1) edges[m1].w1 = edges[i].w1;
      edges[m1].w2 = max(edges[m1].w2,edges[i].w2);
      //if (edges[i].w1<edges[m1].w1) edges[m1].w1=edges[i].w1;
      //edges[m1].w2=min(edges[m1].w2,edges[i].w2);
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
      edges[i].w1 = p;
      edges[i].w2 = p;
      //edges[i].w1=-log(1-pow(1-p, edges[i].c));
      //edges[i].w2=-log(1-pow(1-p, edges[i].c));
  }
}


void Graph::BuildFromFile2DUC(char *file, double p){
  if (built){ 
    return;
  }else{
      built = true;
  }

  FILE* in = fopen(file, "r");
  /*input is: node number and edge number*/
  int n1 = 1;
  fscanf(in,"%d %d", &n1, &m);
  printf("%d  %d\n", n1, m);
  /*reset the size of degree and edges*/
  if(n1>n){
    n = n1;
  }

  degree.resize(n);
  indegree.resize(n);
  outdegree.resize(n);

  edges.resize(2*m);
  correspond.resize(2*m);

  if(in == NULL){
    return;
  }

  for (int i=0; i<m; i++)
  {
    fscanf(in,"%d %d", &edges[i].u, &edges[i].v);
    
    edges[i].w1 = 0;
    edges[i].w2 = -1.0;
    edges[i+m].u = edges[i].v;
    edges[i+m].v = edges[i].u;
    edges[i+m].w1 = -1.0;
    edges[i+m].w2 = 0;
    edges[i].c = 1;
    edges[i+m].c = 1;
    degree[edges[i].u]++;
    degree[edges[i].v]++;
    indegree[edges[i].v]++;
    outdegree[edges[i].u]++;
    correspond[i] = i+m;
    correspond[i+m] = i;

    edges[i].w1 = p;
    edges[i+m].w2 = p;
    //edges[i].w1=-log(1-pow(1-p, 1));
    //edges[i+m].w2=-log(1-pow(1-p, 1));
  }
  
    sortEdges();
}

void Graph::sortEdges(){
  qsort_edges(0, 2*m-1);

  int m1 = 0;
  int newlocation[MAX_EDGE] = {0};

  for (int i=1; i<2*m; i++){
    if ((edges[i].u != edges[m1].u) || (edges[i].v != edges[m1].v)){
      m1++;
      edges[m1] = edges[i];
    }else{
      edges[m1].c++;
      //if (edges[i].w1<edges[m1].w1) edges[m1].w1=edges[i].w1;
      //edges[m1].w2 = min(edges[m1].w2,edges[i].w2);
      if (edges[i].w1 > edges[m1].w1) edges[m1].w1 = edges[i].w1;
      edges[m1].w2 = max(edges[m1].w2, edges[i].w2);
    }
    newlocation[i] = m1;
  }

  for(int i=0; i<2*m; i++){
    correspond[newlocation[i]] = newlocation[correspond[i]];
  }

  if (m!=0){
    m = m1+1;
  }

  index.resize(n);
  for (int i=0; i<n; i++)
    index[i] = 0;
  for (int i=0; i<m; i++)
    index[edges[i].u] = i;
  for (int i=1; i<n; i++){
    if (index[i] < index[i-1]){
      index[i] = index[i-1];
    }
  }
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
  printf("average component size:\t%f\n",double(n)/ncmp);
}
