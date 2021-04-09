#include "graph.h"
#include "independ_cascade.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//#include <time.h>

int IndependCascade::n = 0;
int IndependCascade::m = 0;
int	IndependCascade::targetSize = 0;
int	IndependCascade::resultSize = 0;
int	IndependCascade::target[MAX_NODE] = {0};
bool IndependCascade::built = false;
double IndependCascade::ratio = 0;

void IndependCascade::Build(double r)
{
	n = Graph::GetN();
	m = Graph::GetM();
	built = true;
	ratio = r;
}

void IndependCascade::SetTarget(int size, int set[])
{
//	if (!built) Build();
	targetSize = size;
	for (int i=0; i<size; i++)
		target[i] = set[i];
	resultSize = 0;
}

double IndependCascade::Run(int num_iter, int size, int set[])
{
	SetTarget(size, set);
	int		h, t;
	int		list[MAX_NODE];
	bool	active[MAX_NODE];

	for (int it=0; it<num_iter; it++)
	{
		memset(active, 0, sizeof(bool)*MAX_NODE);
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
			for (int i=0; i<k; i++)
			{
				Edge e = Graph::GetEdge(list[h], i);
				if (active[e.v]) continue;
				for (int j=0; j<e.c; j++)
					if (((double)rand()/(double)RAND_MAX) < ratio)
					{
						list[t] = e.v;
						active[e.v] = true;
						t++;
						resultSize++;
						break;
					}
			}
			h++;
		}
	}
	
	return (double)resultSize / (double)num_iter;
}
