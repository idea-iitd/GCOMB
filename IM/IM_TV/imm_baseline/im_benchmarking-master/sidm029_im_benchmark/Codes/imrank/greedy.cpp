#include "greedy.h"
#include "graph.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int Greedy::n = 0;
int Greedy::top = 0;
double Greedy::d[MAX_NODE] = {0};
int Greedy::list[MAX_NODE] = {0};
char Greedy::file[] = "greedy.txt";

void Greedy::Build(int num, int R, double (*Run)(int num_iter, int size, int set[]))
{
	n = Graph::GetN();
	top = num;

	bool *used= new bool[n];
	memset(used, 0, sizeof(bool)*n);
	int set[SET_SIZE];

	double old = 0.0;

	double *improve=new double[n];
	int *lastupdate=new int[n];
	int *heap=new int[n];
	for (int i=0; i<n; i++)
	{
		heap[i] = i;
		lastupdate[i] = -1;
		improve[i] = (double)(n+1);
	}

	for (int i=0; i<top; i++)
	{
		int ccc = 0;
		printf("%d\n",i);
		while (lastupdate[heap[0]] != i)
		{
			ccc++;
			lastupdate[heap[0]] = i;
			set[i] = heap[0];

			improve[heap[0]] = Run(R, i+1, set) - old;
			

			int x = 0;
			while (x*2+2<=n-i)
			{
				int newx=x*2+1;
				if ((newx+1<n-i) && (improve[heap[newx]]<improve[heap[newx+1]]))
					newx++;
				if (improve[heap[x]]<improve[heap[newx]])
				{
					int t=heap[x];
					heap[x] = heap[newx];
					heap[newx] = t;
					x = newx;
				}
				else
					break;
			}
		}

		used[heap[0]] = true;
		set[i] = heap[0];
		list[i] = heap[0];
		d[i] = improve[heap[0]];
		old+=d[i];


		heap[0] = heap[n-i-1];
		int x = 0;
		while (x*2+2<=n-i)
		{
			int newx=x*2+1;
			if ((newx+1<n-i) && (improve[heap[newx]]<improve[heap[newx+1]]))
				newx++;
			if (improve[heap[x]]<improve[heap[newx]])
			{
				int t=heap[x];
				heap[x] = heap[newx];
				heap[newx] = t;
				x = newx;
			}
			else
				break;
		}
	}

	delete[] heap;
	delete[] lastupdate;
	delete[] improve;
	delete[] used;
}

void Greedy::BuildFromFile(const char* name)
{
	n = Graph::GetN();
	FILE* in =	fopen(name, "r");
	fscanf(in, "%ld", &top);
	for (int i=0; i<top; i++)
		fscanf(in, "%ld %f", &list[i], &d[i]);
	fclose(in);
}

int  Greedy::GetNode(int i)
{
	if (i<0)
		return -1;
	if (i>=top) 
		return -1;
	return list[i];
}


