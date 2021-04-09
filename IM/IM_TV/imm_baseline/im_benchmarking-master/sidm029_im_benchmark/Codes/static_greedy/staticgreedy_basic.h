#ifndef STATIC_GREEDY_BASIC_H
#define STATIC_GREEDY_BASIC_H

#include "limit.h"
#include "graph.h"

struct BasicEdge 
{
	int u,v;
};


struct BasicSnapshot{
   BasicEdge* edges;//remain edges
   int* index;//indexs
};


class BasicStaticGreedy
{
	static int n,m,top;
	static int size;
	//static int set[SET_SIZE];
	static int set[MAX_K];
	//static Edge edges[MAX_EDGE];
	static Edge myEdges[MAX_EDGE];
	static int index[MAX_NODE];
	static double average_norelate[MAX_NODE];

	//static int Check();
	//static void Generate();
	static void qsort_edges(int h, int t);

public:
	static double d[MAX_NODE];
	static int list[MAX_NODE];
	static void Build();
	//static int CheckSet(int s, int *arr);
	//static int* GetSeedsFromUGraphForUIC(int R, int k);
	static void GenerateSnapshot(BasicSnapshot* snapshots, int R);
	static int* GetSeeds(int R, int k);
	static double GetInfluenceSpreadFromSnapshot(int R, int k, int* set, BasicSnapshot* snapshots);
	//static double AveSize(int i);
};

#endif
