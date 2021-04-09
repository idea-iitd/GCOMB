#include "random.h"
#include "graph.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int Random::n = 0;
vector<int> Random::list(MAX_NODE,0);
char Random::file[] = "random.txt";

void Random::Build()
{
	n = Graph::GetN();
	list.resize(n);
	for (int i=0; i<n; i++)
		list[i] = i;

	//srand(time(NULL));

	for (int i=n; i>n-SET_SIZE; i--)
	{
		int t = rand()%i;
		int tmp = list[t];
		list[t] = list[i-1];
		list[i-1] = tmp;
		//printf("%d\n%d", t,list[t-1]);
	}

	//FILE *out = fopen(file, "w");
	//for (int i=n-SET_SIZE; i<n; i++)
		//fprintf(out, "%d\n", list[i]);
	//fclose(out);
}

void Random::BuildFromFile()
{
	n = Graph::GetN();
	list.resize(n);
	FILE* in =	fopen(file, "r");
	for (int i=0; i<n; i++)
		fscanf(in, "%ld", &list[i]);
	fclose(in);
}


int  Random::GetNode(int i)
{
	if (i<0)
		return -1;
	if (i>=n) 
		return -1;
	return list[n-i-1];
}


