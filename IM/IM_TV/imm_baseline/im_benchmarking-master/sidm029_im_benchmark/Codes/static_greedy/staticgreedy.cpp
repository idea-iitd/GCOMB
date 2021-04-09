#include "staticgreedy.h"

#include "limit.h"
#include "graph.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <time.h>

int StaticGreedy::n = 0;
int StaticGreedy::m = 0;
int StaticGreedy::top = 0;
int StaticGreedy::size = 0;
//int StaticGreedy::set[SET_SIZE] = {0};
int StaticGreedy::set[MAX_K] = {0};
double StaticGreedy::d[MAX_NODE] = {0};
int StaticGreedy::list[MAX_NODE] = {0};
//Edge StaticGreedy::edges[MAX_EDGE];
Edge StaticGreedy::myEdges[MAX_EDGE];
int StaticGreedy::index[MAX_NODE];
double StaticGreedy::average_norelate[MAX_NODE];

void StaticGreedy::Build()
{
	Graph::Build();
	n = Graph::GetN();
	m = Graph::GetM();
	/*
	int count = 0;
	for (int i=0; i<n; i++)
		for (int j=0; j<Graph::GetNeighbor(i); j++)
		{
			edges[count] = Graph::GetEdge(i,j);
			count++;
		}
	assert(m==count);
	*/
}

void StaticGreedy::qsort_edges(int h, int t)
{
	if (h<t) 
	{
		int  i = h, j = t;
		Edge mid = myEdges[(i+j)/2];
		myEdges[(i+j)/2] = myEdges[i];

		while (i<j) 
		{
			while ((i<j) && ((myEdges[j].u>mid.u)||((myEdges[j].u==mid.u)&&(myEdges[j].v>mid.v))))
				j--;
			if (i<j) {
				myEdges[i] = myEdges[j];
				i++;
			}
			while ((i<j) && ((myEdges[i].u<mid.u)||((myEdges[i].u==mid.u)&&(myEdges[i].v<mid.v))))
				i++;
			if (i<j) {
				myEdges[j] = myEdges[i];
				j--;
			}
		}

		myEdges[i] = mid;
		qsort_edges(h, i-1);
		qsort_edges(i+1, t);
	}
}

//only for undirected graph and uic model
void StaticGreedy::GenerateForUICUGraph()
{
	int mm = 0;
	for (int i=0; i<m; i++)
		if (Graph::edges[i].u<Graph::edges[i].v)
		{
			if ((double)rand()/(double)RAND_MAX < exp(-Graph::edges[i].w1))
				{
					myEdges[mm] = Graph::edges[i];
					myEdges[mm+1].u = Graph::edges[i].v;
					myEdges[mm+1].v = Graph::edges[i].u;
					//myEdges[mm+1].c = Graph::edges[i].c;
					mm+=2;
					//break;
				}
			/*
			for (int j=0; j<edges[i].c; j++)
				if ((double)rand()/(double)RAND_MAX < p)
				{
					myEdges[mm] = edges[i];
					myEdges[mm+1].u = edges[i].v;
					myEdges[mm+1].v = edges[i].u;
					myEdges[mm+1].c = edges[i].c;
					mm+=2;
					break;
				}
			*/
		}
	qsort_edges(0, mm-1);

	for (int i=0; i<n; i++)
		index[i] = -1;
	for (int i=0; i<mm; i++)
		index[myEdges[i].u] = i;
	for (int i=0; i<n; i++)
		if (i!=0)
		{
			if ((index[i] < 0) && (index[i-1]>=0))
				index[i] = index[i-1];
		}
}


int* StaticGreedy::GetSeedsFromUGraphForUIC(int R, int k)
{
	clock_t start_time, end_time;
	Build();
	SimpleSnapshot* snapshots = (SimpleSnapshot*)malloc(sizeof(SimpleSnapshot)*R);
	
	//int component[MAX_NODE];
	//int compsize[MAX_NODE];
	const int TIME = R;
	int scores[MAX_NODE] = {0};//nodes 
	
	
	for(int zz = 0; zz < TIME; zz++)
	{
		GenerateForUICUGraph();

		//memset(component, 0, sizeof(int)*MAX_NODE);

		//int component[MAX_NODE] = {0};
		//int compsize[MAX_NODE] = {0};
		int *component = (int*)malloc(sizeof(int)*n);
		int *compsize = (int*)malloc(sizeof(int)*n);
		//int *component;
		//int *compsize;
		memset(component, 0, sizeof(int)*n);
		memset(compsize, 0, sizeof(int)*n);

		int count = 0;
		for (int i=0; i<n; i++)
			if (component[i] == 0)
			{
				count++;
				component[i] = count;

				static int list[MAX_NODE];
				int h, t;
				list[0] = i;
				h = 0; t = 1;
				while(h<t)
				{
					int a, b = index[list[h]]+1;
					if (list[h] == 0) a = 0;
					else a = index[list[h]-1]+1;
					for (int j=a; j<b; j++)
					{
						if (component[myEdges[j].v] == 0)
						{
							component[myEdges[j].v] = count;
							list[t] = myEdges[j].v;
							t++;
						}
						else 
						{
							assert(component[myEdges[j].v] == count);
						}
					}
					h++;
				}
				compsize[count] = t;
			}
		
			snapshots[zz].componentid_node = component;
			snapshots[zz].component_size = compsize;
			snapshots[zz].component_number = count;
	  }


	//caculate all the nodes's score

	for(int v=0; v<R; v++){
		for(int b=0; b<n; b++){
			scores[b] += snapshots[v].component_size[snapshots[v].componentid_node[b]];
		}
	}
	

	int* seeds = (int*)malloc(sizeof(int)*k);
	int totalScore = 0;
	for(int g=0; g<k; g++){
    	//select the node with largest scores
    	int max_score = 0;
    	int max_score_node = 0;

     	for(int h=0; h<n; h++){
     		if(max_score < scores[h]){
    			max_score = scores[h];
	     		max_score_node = h;
            }
      	}
		//totalScore = totalScore + max_score;
		seeds[g] = max_score_node;
		//printf("%d\t%f,%d",g+1, (double)(start_time-end_time)/CLOCKS_PER_SEC, R);
		//printf("max_score_node: %d  \n", max_score_node);
		if(g==(k-1))
			break;
		//update the scores
		for(int l = 0 ; l < R; l ++){
			int comp = snapshots[l].componentid_node[max_score_node];
			//check whether updated
			int flag = 0;
			for(int ee=0; ee<g; ee++){
				if(snapshots[l].componentid_node[seeds[ee]] == comp)
				{flag = 1; break;}
			}
			if(flag==1)
				continue;
			for(int i=0; i<n; i++){
				if(snapshots[l].componentid_node[i] == comp)
				{
					scores[i] -= snapshots[l].component_size[comp];
				}
			}
		}
		
	}
	//printf("average Max_score: %f", (double)totalScore/(double)R);
	for(int bb=0; bb<R; bb++){
		free(snapshots[bb].componentid_node);
		free(snapshots[bb].component_size);	
	}
	free(snapshots);
	
	return seeds;
}

int* StaticGreedy::GetSeedsFromUGraphForUICBasic(int R, int k)
{
	Build();
	//n = Graph::GetN();
	//top = SET_SIZE;
	top = k;


	bool *used= new bool[n];
	memset(used, 0, sizeof(bool)*n);
	//int set[SET_SIZE];
	int set[k];

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

	SimpleSnapshot* snapshots = (SimpleSnapshot*)malloc(sizeof(SimpleSnapshot)*R);
	
	//generate G'i by removing each edge e=(u, v) from G with probability 1-p(u, v)
	for(int zz = 0; zz < R; zz++)
	{
		GenerateForUICUGraph();

		int *component = (int*)malloc(sizeof(int)*n);
		int *compsize = (int*)malloc(sizeof(int)*n);

		memset(component, 0, sizeof(int)*n);
		memset(compsize, 0, sizeof(int)*n);

		int count = 0;
		for (int i=0; i<n; i++)
			if (component[i] == 0)
			{
				count++;
				component[i] = count;

				static int list[MAX_NODE];
				int h, t;
				list[0] = i;
				h = 0; t = 1;
				while(h<t)
				{
					int a, b = index[list[h]]+1;
					if (list[h] == 0) a = 0;
					else a = index[list[h]-1]+1;
					for (int j=a; j<b; j++)
					{
						if (component[myEdges[j].v] == 0)
						{
							component[myEdges[j].v] = count;
							list[t] = myEdges[j].v;
							t++;
						}
						else 
						{
							assert(component[myEdges[j].v] == count);
						}
					}
					h++;
				}
				compsize[count] = t;
			}
		
			snapshots[zz].componentid_node = component;
			snapshots[zz].component_size = compsize;
			snapshots[zz].component_number = count;
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
			//improve[heap[0]] = Run(R, i+1, set) - old;
			improve[heap[0]] = GetInfluenceSpreadFromSnapshot(R, i+1, set, snapshots) - old;

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

	for(int bb=0; bb<R; bb++){
		free(snapshots[bb].componentid_node);
		free(snapshots[bb].component_size);	
	}
	free(snapshots);

	return list;
}


double StaticGreedy::GetInfluenceSpreadFromSnapshot(int R, int k, int* set, SimpleSnapshot* snapshots){
	
	//get the average influence spread on R snapshots
	double influencespread = 0.0;
	//int *componentid;
	int id_num=0;

	for(int i=0; i<R; i++)
	{
		SimpleSnapshot snapshot = snapshots[i];
		int *componentid = (int*)malloc(sizeof(int)*snapshot.component_number);
		memset(componentid, -1, sizeof(int)*snapshot.component_number);
		id_num=0;
		for(int j=0; j<k; j++){
			int newid = snapshot.componentid_node[set[j]];
			bool flag = true;
			for( int c=0; c<id_num; c++)
			{
				if(componentid[c]==newid)
				{
					flag=false;
					break;
				}
			}
			if(flag){
					influencespread += snapshot.component_size[newid];
					componentid[id_num] = newid;
					id_num++;
			}
		}
		free(componentid);
	}
	
	return influencespread/R;
}
