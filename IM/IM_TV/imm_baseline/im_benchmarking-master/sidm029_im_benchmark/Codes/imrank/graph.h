#ifndef GRAPH_H
#define GRAPH_H
#include <string>
#include "limit.h"
#include <vector>

using namespace std;

struct Edge 
{
	int u,v,c;	
	double w1,w2;
};

class Graph
{
private:
	static bool built;
	static int n;
	static int m;
	static vector<int> degree;
	static vector<int> indegree;
	static vector<int> outdegree;
	static void qsort_edges(int h, int t);

public:
	static vector<int> index;
	static vector<int> correspond;
	static void	Build();
	static int	GetN();
	static int	GetM();
	static vector<Edge> edges;
	static int	GetDegree(int node);
	static int	GetOutDegree(int node);
	static int	GetInDegree(int node);
	static int	GetNeighbor(int node);
	static Edge	GetEdge(int node, int idx);
	static void BuildIC();
	static void BuildWC();
	static void Build2GC();
	static void Build2WC();
	static void Build2DWC();
	static void BuildFromFile2DWC(char *file);
	static void BuildFromFile2DUC(char *file, double p);
	static void BuildFromFile2WC(char *file);
	static void BuildFromFile2UC(char *file, double p);
	static void Build2IC(char *file);
	static void Build2DT(char *file);
	static void Build2UT(char *file);
	static void BuildFromFile2DT(char *file);
	static void BuildFromFile2UT(char *file);
	static void Stats();
};

#endif

