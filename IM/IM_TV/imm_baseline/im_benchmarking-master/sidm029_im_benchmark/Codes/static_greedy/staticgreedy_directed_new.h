#ifndef STATIC_GREEDY_DIRECTED_NEW_H
#define STATIC_GREEDY_DIRECTED_NEW_H

#include "limit.h"
#include "graph.h"
//#include "DirectedSnapshot.h"

struct ComplexNode{
	int* upstreamNodes;
	int* downstreamNodes;
};

struct NewSnapshot{
	ComplexNode* complexNodes;
	bool* flag_del;
};

struct RandEdge{
	Edge *e;
	bool forward;
	bool reverse;
};

class StaticGreedyDirectedNew
{

	static int n,m;
	static NewSnapshot* newsnapshots;
	static int seed[MAX_NODE];
	static int index[MAX_NODE];
	static bool finish[MAX_NODE];
	
	static bool update[MAX_NODE]; 
    static int  TempRecord_reach[MAX_NODE];
    static bool flag_temp[MAX_NODE];
    static int  RecordID_reach[MAX_NODE];
    static int  TempRecord_upstream[MAX_NODE];
    static int  RecordID_upstream[MAX_NODE];
	static int  otherupstream_id[MAX_NODE];

public:
	static bool flag_forward[MAX_EDGE];
	static void GetReachability(int node, int present, RandEdge* myedges_temp, int* index, int* reach_temp, int* TempRecord, bool* flag);
	static void GetUpStreamNodes(int node, int present, RandEdge* myedges_temp, int* index, int* reach_temp, int* TempRecord, bool* flag);
	static bool flag[MAX_NODE];
	static int* GetSeedsForWICUNDirectedG(int R, int k);
	static void UpdateScore(int seed, NewSnapshot newSnapshot, int* scores);
	static int* GetSeedsForDirectedG(int R, int k);
	static void DeleteNode(int nodeid, NewSnapshot newSnapshot, int* scores);
};

#endif