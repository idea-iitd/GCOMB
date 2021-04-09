#include "degreediscount_ic.h"
#include "graph.h"
#include <stdio.h>
#include <string.h>

int DegreeDiscount_IC::n = 0;
vector<int> DegreeDiscount_IC::d(MAX_NODE,0);
int DegreeDiscount_IC::list[MAX_K] = {0};
char DegreeDiscount_IC::file[] = "degreediscount_ic.txt";

void DegreeDiscount_IC::Build(double ratio)
{
	n = Graph::GetN();
	d.resize(n);
	for (int i=0; i<n; i++)
		d[i] = Graph::GetOutDegree(i);

	bool *used=new bool[n];
	memset(used, 0, sizeof(bool)*n);
	int *count=new int[n];
	memset(count, 0, sizeof(int)*n);
	int *count_down=new int[n];
	memset(count_down, 0, sizeof(int)*n);

	for (int i=0; i<SET_SIZE; i++)
	{
		double max = -1000000.0;
		int mp = -1;
		for (int j=0; j<n; j++)
			if (!used[j])
			{
				double tmp = d[j]-count[j]-count_down[j]-ratio*count[j]*(d[j]-count_down[j]);
				if (tmp >max)
				{
					max = tmp;
					mp = j;
				}
			}

		list[i] = mp;
		used[mp] = true;

		for (int j=0; j<Graph::GetNeighbor(mp); j++)
		{
			Edge e = Graph::GetEdge(mp, j);
			if(e.w1 < 15000) 
				count[e.v] +=e.c;
			if(e.w2 < 15000)
				count_down[e.v] +=e.c;
		}
	}
}

void DegreeDiscount_IC::BuildFromFile()
{
	n = Graph::GetN();
	FILE* in =	fopen(file, "r");
	for (int i=0; i<n; i++)
		fscanf(in, "%ld %f", &list[i], &d[list[i]]);
	fclose(in);
}

int  DegreeDiscount_IC::GetNode(int i)
{
	if (i<0)
		return -1;
	if (i>=n) 
		return -1;
	return list[i];
}


