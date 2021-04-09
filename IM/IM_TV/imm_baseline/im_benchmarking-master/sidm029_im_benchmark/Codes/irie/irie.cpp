#include "irie.h"
#include "heap.h"
#include "graph.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <float.h>
//#include <windows.h>
#include <vector>
#include <set>

int IRIE::n = 0;
bool IRIE::saturated = false;
int IRIE::top = 0;
double IRIE::d[MAX_K];
double IRIE::totalAP;
int IRIE::list[MAX_K];
char IRIE::file[] = "IRIE.txt";

vector<double> IRIE::od(MAX_NODE,0);
vector<int> IRIE::dd(MAX_NODE,0);
vector<double> IRIE::dp(MAX_NODE,1.0);
vector<double> IRIE::ap(MAX_NODE,0);
vector<bool> IRIE::seed(MAX_NODE, false);
vector<bool> IRIE::visited(MAX_NODE, false);

double IRIE::EDV()
{
	set<int> NB;
	set<int> A;
	set<int>::iterator NBIter;

	double edv = 0, temp = 1;

	int i, j, k;

	for(i = 0; i < top; i++)
	{
		j = Graph::GetNeighbor(list[i]);

		A.insert(list[i]);

		for(k = 0; k < j; k++)
		{
			Edge e = Graph::GetEdge(list[i], k);

			NB.insert(e.v);
		}
	}

	for(i = 0; i < top; i++)
		NB.erase(list[i]);

	for(NBIter = NB.begin(); NBIter != NB.end(); NBIter++)
	{
		temp = 1;
		j = Graph::GetNeighbor(*NBIter);
		for(k = 0; k < j; k++)
		{
			Edge e = Graph::GetEdge(*NBIter, k);

			if(A.find(e.v) != A.end())
			{
				temp *= 1 - exp(-e.w2);
				//cout << *NBIter << '\t' << e.v << '\t' << exp(-e.w2) << endl;
				//cin.get();
			}
		}
		edv += 1 - temp;
	}
	return edv;
}

double IRIE::getOptimalAlpha(double bound1, double bound2)
{
	double alpha, maxInf, maxAlpha, temp;
	
	maxInf = 0;
	maxAlpha = bound1;
	
	for(alpha = bound1; alpha <= bound2 + EPS; alpha += 0.1)
	{
		temp = 0;

		getAnswer(alpha);

		if(!saturated)
		{
			for(int i = 0; i < n; i++)
				temp += (ap[i] * od[i]);

			if(temp > maxInf)
			{
				maxAlpha = alpha;
				maxInf = temp;
			}
		}
	}
	
	return maxAlpha;
}

void IRIE::outputSeedSet(double alpha, char data[])
{
	char seedFileName[200];
	sprintf(seedFileName, "IRIE_%.1f_i3", alpha);
	cout<<seedFileName<<endl;
	FILE *out = fopen(seedFileName, "w");
	fprintf(out, "%d\n", top);
	for (int i=0; i<top; i++) {
		fprintf(out, "%d\t%lf\n", list[i], d[i]);
        }
	fclose(out);
}

void IRIE::outputDummy(char data[])
{
	char seedFileName[100];
	for(double a = 0.1; a <= 0.9; a += 0.1)
	{
		sprintf(seedFileName, "IRIE_%.1f_%s", a, data);
		FILE *out = fopen(seedFileName, "w");
		fprintf(out, "%d\n", top);
		for (int i=0; i<top; i++)
			fprintf(out, "0\t0\n");
		fclose(out);
	}
}

void IRIE::getAnswer(double alpha)
{
	int i, minNode;
	for(i = 0; i < n; i++)
		ap[i] = 0;

	for(i = 0; i < n; i++)
		seed[i] = false;

	saturated = false;
        cout<<top<<endl;
	for(i = 0; i < top; i++)
	{
		if(!i){
			minNode = initialInfluence(alpha, i);
		}
		else minNode = residualInfluence(alpha, i);

		saturated = false;
		//if(saturated) break;
		computeAP(minNode, 320);
	}
}

double IRIE::Build(int num, double bound1, double bound2, char data[])
{
	char seedFileName[100];
	n = Graph::GetN();
	top = num;
	seed.resize(n);
	ap.resize(n);
	dp.resize(n);
	dd.resize(n);
	visited.resize(n);
	od.resize(n);

	int i, j;

	/*
	for(i = 0; i < n; i++)
		od[i] = 0;

	for (i = 0;i < n;i++) 
	{
		dd[i]=Graph::GetNeighbor(i);

		for(j = 0; j < dd[i]; j++)
		{
			Edge e = Graph::GetEdge(i, j);

			od[i] += exp(-e.w1);
		}
	}

	double alpha = getOptimalAlpha(bound1, bound2);
	*/

	double alpha = bound1;

	for (i = 0;i < n;i++) 
		dd[i]=Graph::GetNeighbor(i);

	getAnswer(alpha);

	//outputDummy(data);

	//outputSeedSet(alpha, data);

	return alpha;
}

void IRIE::computeAP(int src, int theta)
{
	Heap h;
	HeapNode hNode;
	HeapNode curNode;
	int minNode;
	double minDist;
	double threshold = log((double)theta);

	int i = 0, j, k;
	
	initHeap(&h, n + 1);

	for(i= 0; i < n; i++)
	{
		if(!seed[i])
		{
			hNode.key = i;
			hNode.value = DBL_MAX;
			insertHeap(&h, hNode);
			visited[i] = false;
		}
		else visited[i] = true;
	}

	hNode.key = src;
	hNode.value = 0;
	insertHeap(&h, hNode);

	while(h.count)
	{
		curNode = h.elements[1];
		minNode = curNode.key;
		minDist = curNode.value;

		if(minDist >= threshold)
			break;

		k = Graph::GetNeighbor(minNode);

		for(j = 0; j < k; j++)
		{
			Edge e = Graph::GetEdge(minNode,j);
			
			if(!visited[e.v])
			{
				if(h.elements[h.index[e.v]].value > minDist + e.w1 + EPS)
					decreaseKeyHeap(&h, h.index[e.v], minDist + e.w1);
			}
		}

		ap[minNode] += exp(-minDist);
		if(ap[minNode] > 1 - EPS)
			ap[minNode] = 1;

		removeMinHeap(&h);
		//visited[minNode] = true;
	}

	freeHeap(&h);
}

int IRIE::GetMax()
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

double IRIE::initialInfluence(double alpha, int round)
{
	char seedFileName[100];
	n = Graph::GetN();
	dp.resize(n);
	dd.resize(n);
	vector<double> newdp(n,0);

	bool changed = true;

	int i=0, j;

	double theta = 0.0001;

	double delta, edgeProb;
	long count=0;

	
	for (i = 0;i < n;i++) newdp[i]=0;

	for (i = 0;i < n;i++) dp[i] = 1;
	
	/*
	for (i = 0;i < n;i++)
	{
		newdp[i]=0;
		dd[i]=Graph::GetNeighbor(i);
		dp[i] = 1;
	}
	*/
	
	//while(count++ < NUM_INIT_LOOP && changed && !saturated)
	while(count++ < NUM_INIT_LOOP && changed)
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
				if(edgeProb > EPS)
					newdp[i] += alpha * edgeProb * dp[e.v];
				

				//newdp[i] += alpha * exp(-e.w1) * dp[e.v];
			}
			newdp[i] += 1;
		}

		//delta=0;

		for (i = 0;i < n; i++)
			if((newdp[i] < dp[i] - theta - EPS) || (newdp[i] > dp[i] + theta + EPS))
				changed = true;

		for (i = 0;i < n; i++)
			if(newdp[i] > n)
				saturated = true;

		for (i = 0;i < n; i++)
			dp[i] = newdp[i];
	}
	int max = GetMax();
	list[round] = max;
	d[round] = dp[max];
	seed[max] = true;

	return max;
}

double IRIE::residualInfluence(double alpha, int round)
{
	char seedFileName[100];
	n = Graph::GetN();
	dp.resize(n);
	dd.resize(n);
	vector<double> newdp(n,0);
	vector<double> newnewdp(n, 0);

	bool changed = true;

	int i=0;

	double theta = 0.0001;

	double delta, edgeProb;
	long count=0;
	
	//new implemented IRIE
	/*
	for (i = 0;i < n;i++) newdp[i] = 0;

	for (i = 0;i < n;i++) dd[i]=Graph::GetNeighbor(i);

	for (i = 0;i < n;i++) dp[i] = 1 - ap[i];
	
	while(count++ < NUM_LOOP && changed && !saturated)
	{
		changed = false;

		for (i = 0;i < n;i++)
			newdp[i]=0;

		//cout << count << endl;

		for (i = 0;i < n;i++)
		{
			for (int j = 0; j < dd[i]; j++)
			{
				Edge e = Graph::GetEdge(i,j);
				
				edgeProb = exp(-e.w1);
				if(edgeProb > EPS || ap[i] < 1 - EPS)
					newdp[i] += alpha * (1 - ap[i]) * edgeProb * dp[e.v];
				

				//newdp[i] += alpha * exp(-e.w1) * dp[e.v];
			}
			newdp[i] += 1 - ap[i];
		}

		//delta=0;

		for (i = 0;i < n; i++)
			if((newdp[i] < dp[i] - theta - EPS) || (newdp[i] > dp[i] + theta + EPS))
				changed = true;

		for (i = 0;i < n; i++)
			if(newdp[i] > n)
				saturated = true;

		for (i = 0;i < n; i++)
			dp[i] = newdp[i];
	}

	int max = GetMax();

	list[round] = max;
	d[round] = dp[max];
	seed[max] = true;

	return max;
	*/

	//IRIE-1 initial
	/*
	for (i = 0;i < n;i++) newdp[i] = (1 - ap[i]) * dp[i];

	for (i = 0;i < n;i++)
	{
		for (int j = 0; j < dd[i]; j++)
		{
			Edge e = Graph::GetEdge(i,j);
				
			edgeProb = exp(-e.w1);
			if(edgeProb > EPS || ap[i] < 1 - EPS)
				newnewdp[i] += alpha * (1 - ap[i]) * edgeProb * newdp[e.v];
				

			//newdp[i] += alpha * exp(-e.w1) * dp[e.v];
		}
		newnewdp[i] += 1 - ap[i];
	}

	for (i = 0;i < n; i++)
		if(newnewdp[i] > n)
			saturated = true;

	double max = -1000000.0;
	int mp = -1;
	for (int j=0; j<n; j++)
	{
		double tmp = newnewdp[j];
		if (tmp >max)
		{
			max = tmp;
			mp = j;
		}
	}

	list[round] = mp;
	d[round] = newnewdp[mp];
	seed[mp] = true;

	return mp;
	*/

	//IRIE-1 previous
	/*
	for (i = 0;i < n;i++) newdp[i] = (1 - ap[i]) * dp[i];

	for (i = 0;i < n;i++)
	{
		for (int j = 0; j < dd[i]; j++)
		{
			Edge e = Graph::GetEdge(i,j);
				
			edgeProb = exp(-e.w1);
			if(edgeProb > EPS || ap[i] < 1 - EPS)
				newnewdp[i] += alpha * (1 - ap[i]) * edgeProb * newdp[e.v];
		}
		newnewdp[i] += 1 - ap[i];
	}

	for (i = 0;i < n; i++)
		dp[i] = newnewdp[i];

	for (i = 0;i < n; i++)
		if(newnewdp[i] > n)
			saturated = true;

	double max = -1000000.0;
	int mp = -1;
	for (int j=0; j<n; j++)
	{
		double tmp = newnewdp[j];
		if (tmp >max)
		{
			max = tmp;
			mp = j;
		}
	}

	list[round] = mp;
	d[round] = newnewdp[mp];
	seed[mp] = true;

	return mp;
	*/
	
	//IRIE-5 initial
	/*
	for (i = 0;i < n;i++) newdp[i] = (1 - ap[i]) * dp[i];

	while(count++ < 5 && changed && !saturated)
	{
		changed = false;

		for (i = 0;i < n;i++)
			newnewdp[i] = 0;

		//cout << count << endl;

		for (i = 0;i < n;i++)
		{
			for (int j = 0; j < dd[i]; j++)
			{
				Edge e = Graph::GetEdge(i,j);
				
				edgeProb = exp(-e.w1);
				if(edgeProb > EPS || ap[i] < 1 - EPS)
					newnewdp[i] += alpha * (1 - ap[i]) * edgeProb * newdp[e.v];
				

				//newdp[i] += alpha * exp(-e.w1) * dp[e.v];
			}
			newnewdp[i] += 1 - ap[i];
		}

		for (i = 0;i < n; i++)
			if((newnewdp[i] < newdp[i] - theta) || (newnewdp[i] > newdp[i] + theta))
				changed = true;

		for (i = 0;i < n; i++)
			if(newnewdp[i] > n)
				saturated = true;
		
		for (i = 0;i < n; i++)
			newdp[i] = newnewdp[i];
		
	}

	double max = -1000000.0;
	int mp = -1;
	for (int j=0; j<n; j++)
	{
		double tmp = newnewdp[j];
		if (tmp >max)
		{
			max = tmp;
			mp = j;
		}
	}

	list[round] = mp;
	d[round] = newnewdp[mp];
	seed[mp] = true;

	return mp;
	*/

	//IRIE-5 previous
	
	for (i = 0;i < n;i++) newdp[i] = (1 - ap[i]) * dp[i];

	while(count++ < NUM_SUBS_LOOP && changed)
	{
		changed = false;

		for (i = 0;i < n;i++)
			newnewdp[i]=0;

		//cout << count << endl;

		for (i = 0;i < n;i++)
		{
			for (int j = 0; j < dd[i]; j++)
			{
				Edge e = Graph::GetEdge(i,j);
				
				edgeProb = exp(-e.w1);
				if(edgeProb > EPS || ap[i] < 1 - EPS)
					newnewdp[i] += alpha * (1 - ap[i]) * edgeProb * newdp[e.v];
			}
			newnewdp[i] += 1 - ap[i];
		}

		//delta=0;

		for (i = 0;i < n; i++)
			if((newnewdp[i] < newdp[i] - theta) || (newnewdp[i] > newdp[i] + theta))
				changed = true;

		for (i = 0;i < n; i++)
			if(newnewdp[i] > n)
				saturated = true;

		for (i = 0;i < n; i++)
			newdp[i] = newnewdp[i];
	}

	for (i = 0;i < n; i++)
		dp[i] = newnewdp[i];

	int max = GetMax();

	list[round] = max;
	d[round] = newnewdp[max];
	seed[max] = true;

	return max;
	
}

int IRIE::GetNode(int i)
{
	if (i<0)
		return -1;
	if (i>=top) 
		return -1;
	return list[i];
}
