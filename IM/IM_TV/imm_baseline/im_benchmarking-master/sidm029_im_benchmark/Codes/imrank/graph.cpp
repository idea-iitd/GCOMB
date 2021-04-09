#include "graph.h"
#include <fstream>
#include<stdlib.h>
#include <stdio.h>
#include<iostream>
#include <string.h>
#include <math.h>
#include <vector>
#include <map>
#include <queue>
using namespace std;

bool Graph::built = false;
int  Graph::n = 0;
int  Graph::m = 0;
vector<int> Graph::index;
vector<int> Graph::degree(0,0);
vector<int> Graph::indegree(0,0);
vector<int> Graph::outdegree(0,0);
vector<Edge> Graph::edges;
vector<int> Graph::correspond(0,0);

void Graph::qsort_edges(int h, int t)
{
	if (h<t) 
	{
		int  i = h, j = t;
		Edge mid = edges[(i+j)/2];
		int midc = correspond[(i+j)/2];
		edges[(i+j)/2] = edges[i];
		if (midc != i)
		{
			correspond[correspond[i]] = (i+j)/2;
			correspond[(i+j)/2] = correspond[i];
		}
		else
			midc = (i+j)/2;

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
		edges[i].c	 = 1;
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
	if (!built)	Build();
	return n;
}

int Graph::GetM()
{
	if (!built)	Build();
	return m;
}

int Graph::GetDegree(int node)
{
	if (!built)	Build();
	return degree[node];
}

int Graph::GetOutDegree(int node)
{
	if (!built)	Build();
	return outdegree[node];
}

int Graph::GetInDegree(int node)
{
	if (!built)	Build();
	return indegree[node];
}

int Graph::GetNeighbor(int node)
{
	if (!built)	Build();
	if (node == 0)
		return index[node]+1;
	else 
		return index[node]-index[node-1];
}

Edge Graph::GetEdge(int node, int idx)
{
	if (!built)	Build();
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

void Graph::BuildIC()
{
	for (int i=0; i<m; i++) {
		edges[i].w1=-log(1-pow(1-0.1, edges[i].c));
		edges[i].w2=-log(1-pow(1-0.1, edges[i].c));
	}
}


void Graph::Build2WC()
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
		edges[i].w1=-log(edges[i].w1);
		edges[i].w2=-log(edges[i].w2);
		edges[i].c	 = 1;
		degree[edges[i].u]++;
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

void Graph::BuildFromFile2DWC(char  *file){
	if (built) 
		return;
	built = true;

	
	std::string fileloc(file);
 	 std::size_t found = fileloc.find_last_of("/");
 	 fileloc = fileloc.substr(0,found+1)+"attribute.txt";
	//   FILE* attributein = fopen(fileloc.c_str(),"r");
//	 fscanf(attributein,"%d %d", &n, &m);
	cout<<fileloc<<endl;

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
	fscanf(in,"%ld %ld", &n, &m);	

	degree.resize(n);
	indegree.resize(n);
	outdegree.resize(n);
	edges.resize(2*m);
	correspond.resize(2*m);

	double prob;
	for (int i=0; i<m; i++)
	{
		fscanf(in,"%ld %ld %f", &edges[i].u, &edges[i].v,&prob);
		edges[i].w1=0;
		edges[i].w2=20000;
		edges[i+m].u = edges[i].v;
		edges[i+m].v = edges[i].u;
		edges[i+m].w1=20000;
		edges[i+m].w2=0;
		edges[i].c	 = 1;
		edges[i+m].c = 1;
		degree[edges[i].u]++;
		degree[edges[i].v]++;
		indegree[edges[i].v]++;
		outdegree[edges[i].u]++;
		correspond[i] = i+m;
		correspond[i+m] = i;
	}
	qsort_edges(0, 2*m-1);
	vector<int> newlocation;
	for(int k=0;k<2*m;k++)
		newlocation.push_back(0);
	//int newlocation[MAX_EDGE] = {0};
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
		edges[i].c = 1;\
		if (edges[i].w1==0) edges[i].w1=-log(1-pow(1-1.0/indegree[edges[i].v], 1));
		if (edges[i].w2==0) edges[i].w2=-log(1-pow(1-1.0/indegree[edges[i].u], 1));
	}
}


void Graph::BuildFromFile2WC(char *file){
	if (built) 
		return;
	built = true;

	
	std::string fileloc(file);
 	 std::size_t found = fileloc.find_last_of("/");
 	 fileloc = fileloc.substr(0,found+1)+"attribute.txt";
	//   FILE* attributein = fopen(fileloc.c_str(),"r");
//	 fscanf(attributein,"%d %d", &n, &m);
	cout<<fileloc<<endl;

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
	indegree.resize(n);
	outdegree.resize(n);
	edges.resize(2*m);
	correspond.resize(2*m);
	double prob;
	for (int i=0; i<m; i++)
	{
		fscanf(in,"%ld %ld %f", &edges[i].u, &edges[i].v, &prob);
		edges[i].w1=0;
		edges[i].w2=0;
		edges[i+m].u = edges[i].v;
		edges[i+m].v = edges[i].u;
		edges[i+m].w1=0;
		edges[i+m].w2=0;
		edges[i].c	 = 1;
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
	//int newlocation[MAX_EDGE] = {0};
	vector<int> newlocation;
       for(int k=0;k<2*m;k++)
               newlocation.push_back(0);
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
			edges[i].w1=-log(1-pow(1-1.0/indegree[edges[i].v], edges[i].c));
			edges[i].w2=-log(1-pow(1-1.0/indegree[edges[i].u], edges[i].c));
	}
}

void Graph::BuildFromFile2UC(char *file, double p){
	
	if (built) 
		return;
	built = true;

	FILE* in = fopen(file, "r");
	fscanf(in,"%ld %ld", &n, &m);	

	degree.resize(n);
	indegree.resize(n);
	outdegree.resize(n);
	edges.resize(2*m);
	correspond.resize(2*m);

	for (int i=0; i<m; i++)
	{
		fscanf(in,"%ld %ld", &edges[i].u, &edges[i].v);   
		edges[i].w1=0;
		edges[i].w2=0;
		edges[i+m].u = edges[i].v;
		edges[i+m].v = edges[i].u;
		edges[i+m].w1=0;
		edges[i+m].w2=0;
		edges[i].c	 = 1;
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
	vector<int> newlocation;
	for(int k=0;k<2*m;k++)
		newlocation.push_back(0);
	//int newlocation[MAX_EDGE] = {0};
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
			edges[i].w1=-log(1-pow(1-p, edges[i].c));
			edges[i].w2=-log(1-pow(1-p, edges[i].c));
	}
}

void Graph::BuildFromFile2DUC(char *file, double p){
	
	if (built) 
		return;
	built = true;

	FILE* in = fopen(file, "r");
	fscanf(in,"%ld %ld", &n, &m);	

	degree.resize(n);
	indegree.resize(n);
	outdegree.resize(n);
	edges.resize(2*m);
	correspond.resize(2*m);

	for (int i=0; i<m; i++)
	{
		fscanf(in,"%ld %ld", &edges[i].u, &edges[i].v);
		int temp = 0;
	    temp = edges[i].u;
        edges[i].u = edges[i].v;
        edges[i].v = temp;

		edges[i].w1=0;
		edges[i].w2=20000;
		edges[i+m].u = edges[i].v;
		edges[i+m].v = edges[i].u;
		edges[i+m].w1=20000;
		edges[i+m].w2=0;
		edges[i].c	 = 1;
		edges[i+m].c = 1;
		degree[edges[i].u]++;
		degree[edges[i].v]++;
		indegree[edges[i].v]++;
		outdegree[edges[i].u]++;
		correspond[i] = i+m;
		correspond[i+m] = i;
	}
	
	qsort_edges(0, 2*m-1);
	
	int m1 = 0;
	 vector<int> newlocation;
        for(int k=0;k<2*m;k++)
                newlocation.push_back(0);
	//int newlocation[MAX_EDGE] = {0};
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
		if (edges[i].w1==0) edges[i].w1=-log(1-pow(1-p, 1));
		if (edges[i].w2==0) edges[i].w2=-log(1-pow(1-p, 1));
	}
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
		edges[i].w1=0;
		edges[i].w2=20000;
		edges[i+m].u = edges[i].v;
		edges[i+m].v = edges[i].u;
		edges[i+m].w1=20000;
		edges[i+m].w2=0;
		edges[i].c	 = 1;
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
		edges[i].w1=-log(edges[i].w1);
		edges[i].w2=-log(edges[i].w2);
		edges[i].c	 = 1;
		degree[edges[i].u]++;
	}

	qsort_edges(0, m-1);

	int m1 = 0;
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
}


void Graph::Build2IC(char *file)
{
	if (built) 
		return;
	built = true;
	FILE* in = fopen(file, "r");
	fscanf(in,"%ld %ld", &n, &m);	
	degree.resize(n);
	edges.resize(2*m);
	//edges.resize(2*m);
	for (int i=0; i<m; i++)
	{
		fscanf(in,"%ld %ld", &edges[i].u, &edges[i].v);
		edges[i].u--;
		edges[i].v--;
		edges[i].w1=-log(0.1);
		edges[i].w2=-log(0.1);
		edges[i].c	 = 1;
		degree[edges[i].u]++;
		degree[edges[i].v]++;
	}
	cout<<"HELLO @"<<endl;
	qsort_edges(0, m-1);
	cout<<"HELLO @"<<endl;

	int m1 = 0;
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
	cout<<"HELLO @"<<endl;
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


void Graph::Build2DT(char *file)
{
	double prob1=0.1;
	double prob2=0.1;
	double prob3=0.1;

	if (built) 
		return;
	built = true;

	FILE* in = fopen(file, "r");
	fscanf(in,"%ld %ld", &n, &m);	

	degree.resize(n);
	edges.resize(m);
	for (int i=0; i<m; i++)
	{
		fscanf(in, "%ld %ld\n", &edges[i].u, &edges[i].v);
		int x=rand();
		if(x%3==0)
		{
			edges[i].w1=-log(prob1);
		}
		else if(x%3==1)
		{
			edges[i].w1=-log(prob2);
		}
		else if(x%3==2){
			edges[i].w1=-log(prob3);
		}else{
			printf("wrong!");
	    }
		edges[i].w2 = 20000;
	
	}
}

void Graph::Build2UT(char *file)
{
	double prob1=0.1;
	double prob2=0.1;
	double prob3=0.001;

	if (built) 
		return;
	built = true;

    FILE* in = fopen(file, "r");
	fscanf(in,"%ld %ld", &n, &m);	

	degree.resize(n);
	edges.resize(m);
	for (int i=0; i<m; i++)
	{
		fscanf(in, "%ld %ld\n", &edges[i].u, &edges[i].v);

		int x=rand();
		if(x%3==0)
		{
			edges[i].w1=-log(prob1);
		}
		else if(x%3==1)
		{
			edges[i].w1=-log(prob2);
		}
		else if(x%3==2){
			edges[i].w1=-log(prob3);
		}else{
			printf("wrong!");
	    }

		x=rand();
		if(x%3==0)
		{
			edges[i].w2=-log(prob1);
		}
		else if(x%3==1)
		{
			edges[i].w2=-log(prob2);
		}
		else if(x%3==2){
			edges[i].w2=-log(prob3);
		}else{
			printf("wrong!");
	    }
	}
}

void Graph::BuildFromFile2DT(char *file){
  
	if (built) 
		return;
	built = true;
	
	std::string fileloc(file);
 	 std::size_t found = fileloc.find_last_of("/");
 	 fileloc = fileloc.substr(0,found+1)+"attribute.txt";
	//   FILE* attributein = fopen(fileloc.c_str(),"r");
//	 fscanf(attributein,"%d %d", &n, &m);
	cout<<fileloc<<endl;

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
	indegree.resize(n);
	outdegree.resize(n);
	edges.resize(2*m);
	correspond.resize(2*m);

	cout<<fileloc<<endl;
	double prob;
	for (int i=0; i<m; i++)
	{
	//	cout<<"this is done"<<i<<endl;
		fscanf(in,"%ld %ld %f", &edges[i].u, &edges[i].v,&prob);
		//fscanf(in,"%ld %ld %lf %lf", &edges[i].u, &edges[i].v, &edges[i].w1, &edges[i].w2);
		edges[i].w1 = -log(edges[i].w1);
		edges[i].w2 = 20000;
		//ADDED BY SAINYAM FOR IC
		edges[i].w1=-log(0.1);
            //    edges[i].w2=-log(0.1);
                edges[i].c       = 1;
		//REMOVE IF YOU WANT ORIGINAL CODE

	//	edges[i+m].u = edges[i].v;
	//	edges[i+m].v = edges[i].u;
	//	edges[i+m].w1 = edges[i].w2;
	//	edges[i+m].w2 = edges[i].w1;

		edges[i].c	 = 1;
	//	edges[i+m].c = 1;
		degree[edges[i].u]++;
		degree[edges[i].v]++;
		indegree[edges[i].v]++;
		outdegree[edges[i].u]++;
		correspond[i] = i+m;
		correspond[i+m] = i;
	}
	qsort_edges(0, 2*m-1);
 	vector<int> newlocation;
        for(int k=0;k<2*m;k++)
                newlocation.push_back(0);
//	int newlocation[MAX_EDGE] = {0};
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
	cout<<"graph reading is done"<<endl;
}

void Graph::BuildFromFile2UT(char *file){
	
	if (built) 
		return;
	built = true;

	FILE* in = fopen(file, "r");
	fscanf(in,"%ld %ld", &n, &m);	

	degree.resize(n);
	indegree.resize(n);
	outdegree.resize(n);
	edges.resize(2*m);
	correspond.resize(2*m);

	for (int i=0; i<m; i++)
	{
		fscanf(in,"%ld %ld %lf %lf", &edges[i].u, &edges[i].v,  &edges[i].w1, &edges[i].w2);
		edges[i].w1 = -log(edges[i].w1);
		edges[i].w2 = -log(edges[i].w2);
		edges[i+m].u = edges[i].v;
		edges[i+m].v = edges[i].u;
		edges[i+m].w1 = edges[i].w2;
		edges[i+m].w2 = edges[i].w1;
		edges[i].c	 = 1;
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
	 vector<int> newlocation;
        for(int k=0;k<2*m;k++)
                newlocation.push_back(0);
	//int newlocation[MAX_EDGE] = {0};
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
