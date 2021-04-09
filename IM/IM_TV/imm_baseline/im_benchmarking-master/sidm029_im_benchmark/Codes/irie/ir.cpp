#include "ir.h"
#include "graph.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <float.h>
//#include <windows.h>
#include <vector>

int IR::n = 0;
int IR::top = 0;
double IR::d[MAX_K];
int IR::list[MAX_K];
char IR::file[] = "IR.txt";

vector<int> IR::dd(MAX_NODE,0);
vector<double> IR::dp(MAX_NODE,1.0);

int IR::GetMax()
{
	double max = -1000000.0;
	int mp = -1;
	for (int j=0; j<n; j++)
	{
		double tmp = dp[j];
		if (tmp >max)
		{
			max = tmp;
			mp = j;
		}
	}
	return mp;
}

double IR::Build(int num, double alpha, char data[])
{
	char seedFileName[100];
	n = Graph::GetN();
	top = num;
	dp.resize(n);
	dd.resize(n);
	vector<double> newdp(n,0);

	bool changed = true;
	bool saturated = false;

	int i=0;

	double theta = 0.0001;

	double delta, edgeProb;
	long count=0;

	for (i=0;i<n;i++)
	{
		newdp[i]=0;
		dp[i] = 1;
		dd[i]=Graph::GetNeighbor(i);
	}
	
	while(count++ < NUM_LOOP && changed && !saturated)
	{
		changed = false;

		for (i=0;i<n;i++)
			newdp[i]=0;

		//cout << count << endl;

		for (i=0;i < n;i++)
		{
			for (int j=0; j < dd[i];j++)
			{
				Edge e = Graph::GetEdge(i,j);
				
				edgeProb = exp(-e.w1);
				if(edgeProb)
					newdp[i] += alpha * edgeProb * dp[e.v];
				

				//newdp[i] += alpha * exp(-e.w1) * dp[e.v];
			}
			newdp[i] += 1;
		}

		//delta=0;

		for (i = 0;i < n; i++)
			if((newdp[i] < dp[i] - theta) || (newdp[i] > dp[i] + theta))
				changed = true;

		for (i = 0;i < n; i++)
			if(newdp[i] > n)
				saturated = true;

		for (i = 0;i < n; i++)
			dp[i] = newdp[i];
	}

	int max;
	for (i = 0; i < top; i++)
	{
		max = GetMax();

		list[i] = max;
		d[i] = dp[max];
		dp[max] = 0;
	}

	sprintf(seedFileName, "IR_%.1f_%s", alpha, data);
	printf("seedFileName: %s", seedFileName);
	FILE *out = fopen(seedFileName, "w");
	fprintf(out, "%d\n", top);
	for (i=0; i<top; i++)
		fprintf(out, "%d\t%lf\n", list[i], d[i]);
	fclose(out);
	return 1;
}

int  IR::GetNode(int i)
{
	if (i<0)
		return -1;
	if (i>=top) 
		return -1;
	return list[i];
}
