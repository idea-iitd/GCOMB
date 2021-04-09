#include "pagerank.h"
#include "graph.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
//#include <windows.h>
#include <vector>

int pagerank::n = 0;
int pagerank::top = 0;
double pagerank::d[MAX_K];
int pagerank::list[MAX_K];
char pagerank::file[] = "pagerank.txt";

//int pagerank::k=1;
vector<int> pagerank::dd(MAX_NODE,0);
//double SPT_new::longest = log(100.0);
vector<double> pagerank::dp(MAX_NODE,1.0);
//vector<bool> SPT_new::used(MAX_NODE);
//vector<double *>SPT_new::self(MAX_NODE);
//vector<int> pagerank::lastupdate(MAX_NODE,-1);
//vector<double *>SPT_new::delta(MAX_NODE);
//vector<int *>SPT_new::children(MAX_NODE,NULL);
//vector<int *>SPT_new::path(MAX_NODE,NULL);

//int *SPT_new::S;
//double *SPT_new::distance, *SPT_new::b;
//int *SPT_new::heap;
//int *SPT_new::childlist, *SPT_new::oldchildlist, *SPT_new::parent;
//bool *SPT_new::validlist[MAX_K]={NULL};

//double ratio=0.01;

int pagerank::GetMax(int round)
{
		double max = -1000000.0;
		int mp = -1;
		for (int j=0; j<n; j++)
			//if (!used[j] && lastupdate[j]!=round)
			{
				double tmp = dp[j];
				//printf("%d %d %Lg %Lg %Lg\n",round,j, self[j],dp[j], tmp);
				if (tmp > max)
				{
					max = tmp;
					mp = j;
				}
			}
		return mp;
		//fprintf(out, "%ld %ld\n", mp, dd[mp]);
}

double pagerank::Build(int num, double dampen)
{
	n = Graph::GetN();
	top = num;
	dp.resize(n);
	dd.resize(n);
	vector<double> newdp(n,0);
	vector<double> self(n,0.0);
	int set[SET_SIZE];
	int i=0;
	for (i=0; i<n; i++)
	{
		dp[i]=1.0;
		dd[i]=Graph::GetNeighbor(i);
		for (int j=0;j<dd[i];j++)
			self[i]+=exp(-Graph::GetEdge(i,j).w2);
	}
	double theta=1e-3;				
	double delta;
	long count=0;
	do
	{
		for (i=0;i<n;i++)
			newdp[i]=0;
		for (i=0;i<n;i++)
			for (int j=0;j<dd[i];j++)
			{
				Edge e=Graph::GetEdge(i,j);
				newdp[e.v]+=dp[i]*exp(-e.w2)/self[i];
			}
		delta=0;
		for (i=0;i<n;i++)
		{
			delta+=fabs(dampen+(1-dampen)*newdp[i]-dp[i]);
			dp[i]=dampen+(1-dampen)*newdp[i];
		}		
	} while (delta>theta &&++count<10000);

	i=0;
	double max = -1000000.0;
	int mp;

	for (i=0; i<top; i++)
	{
		max = -1000000.0;
		int x=GetMax(i);
		set[i] = x;
		double improve=dp[x];
		if (improve > max) {
				max=improve;
				mp=x;
		}
		dp[mp] = 0;
		set[i] = mp;
		list[i] = mp;
		d[i] = max;
	}
	
	FILE *out = fopen(file, "w");
	fprintf(out, "%d\n", top);
	for (i=0; i<top; i++)
		fprintf(out, "%d\t%Lg\n", list[i], d[i]);
	fclose(out);
	return 1;
}

void pagerank::BuildFromFile()
{
	n = Graph::GetN();
	//sprintf_s(file,"SPT_new_%04d.txt", bound);
	FILE* in =	fopen(file, "r");
	fscanf(in, "%ld", &top);
	for (int i=0; i<top; i++)
		fscanf(in, "%ld %f", &list[i], &d[i]);
	fclose(in);
}

int  pagerank::GetNode(int i)
{
	if (i<0)
		return -1;
	if (i>=top) 
		return -1;
	return list[i];
}


//char* pagerank::filename()
//{
//	sprintf_s(file,"SPT_new_%04d.txt", bound);
//	return file;
