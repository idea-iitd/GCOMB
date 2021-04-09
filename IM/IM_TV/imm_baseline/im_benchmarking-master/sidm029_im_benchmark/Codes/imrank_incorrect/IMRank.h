#ifndef IMRANK_H
#define IMRANK_H

#include "limit.h"
#include "graph.h"
#include <vector>

using namespace std;

struct RankNode{
   //int id;
   int rank;
   double score;
};

struct RankLeaders{
	int id;
	double p;
};

struct RankSecondLeaders{
	int id;
	double p;
	int FirstEdgeIndex;
	int SecondEdgeIndex;
};

class imrank
{
private:
	static int n;
	static int top;
	static double d[MAX_K];
	static int list[MAX_K];
	static char file[STR_LEN];
	static vector<double> dp;
	static vector<int> dd;
	static bool debug;
	static vector<Edge> edges;
    static vector<int> correspond;
	static vector<int> index;
	static vector<bool> visited;
	static vector<double> pp;

public:
	static RankNode* rankNodes;
	static long timer;
	static void constructEdges();
	//static double Build(bool secondLeaders);
	static double Build(char *initialRank, int l);
	static int GetNode(int i);
	static int GetMax(int round);
	static int Compare(const void * id1, const void * id2);
	static int Compare2(const void * tempLeader1, const void * tempLeader2);
	static int Compare3(const void * tempSecondLeader1, const void * tempSecondLeader2);
	static void qsort_edges(int h, int t);
    static int GetLeaders(int node);
	static Edge GetEdge(int minNode,int j);
	static int GetNeighbor(int node);
	static void InitialRank(char *initialRank, RankNode *rankNodes);
	static void DeliverCertainLeaders(int n, int lorder, int *rankIDs, RankNode *rankNodes);
	static void DeepSearch(int n, RankNode *rankNodes, int seedid, int nodeid, double current_p, int current_l, RankLeaders* tempLeaders, int* state, int lorder);
 

};


#endif
