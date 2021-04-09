#include "weighteddegree.h"
#include "graph.h"
#include <stdio.h>
#include <cmath>
int WeightedDegree::n = 0;
vector<double> WeightedDegree::d(MAX_NODE,0);
int WeightedDegree::list[SET_SIZE];
char WeightedDegree::file[] = "weighteddegree.txt";

void WeightedDegree::qsort_degree(int h, int t)
{
	if (h<t) 
	{
		int i = h, j = t;
		double midd = d[(i+j)/2];
		int midn = list[(i+j)/2];
		d[(i+j)/2] = d[i];
		list[(i+j)/2] = list[i];

		while (i<j) 
		{
			while ((i<j) && (d[j]<midd))
				j--;
			if (i<j) {
				d[i] = d[j];
				list[i] = list[j];
				i++;
			}
			while ((i<j) && (d[i]>midd))
				i++;
			if (i<j) {
				d[j] = d[i];
				list[j] = list[i];
				j--;
			}
		}

		d[i] = midd;
		list[i] = midn;
		qsort_degree(h, i-1);
		qsort_degree(i+1, t);
	}
}

void WeightedDegree::Build()
{
	n = Graph::GetN();
	d.resize(n);
	for (int i=0; i<n; i++)
	{
		int degree=Graph::GetNeighbor(i);
		for (int j=0; j<degree; j++)
			d[i] += exp(-Graph::GetEdge(i,j).w1);
	}

	for (int i=0; i<SET_SIZE; i++)
	{	
		int max = 0;
		for (int j=0; j<n; j++)
			if (d[j]>d[max])
				max = j;
		//printf("%d %lg\n",max,d[max]);
		d[max] = 0.0;
		list[i] = max;

	}

//	qsort_degree(0, n-1);

	FILE *out =	fopen(file, "w");
	for (int i=0; i<SET_SIZE; i++)
		fprintf(out, "%d\n", list[i]);
	fclose(out);
}

void WeightedDegree::BuildFromFile()
{
	n = Graph::GetN();
	d.resize(SET_SIZE);
	FILE* in =	fopen(file, "r");
	for (int i=0; i<SET_SIZE; i++)
		fscanf(in, "%ld %ld", &list[i], &d[i]);
	fclose(in);
}

int  WeightedDegree::GetNode(int i)
{
	if (i<0)
		return -1;
	if (i>=n) 
		return -1;
	//printf("%d %d\n", i, list[i]);
	return list[i];
}


