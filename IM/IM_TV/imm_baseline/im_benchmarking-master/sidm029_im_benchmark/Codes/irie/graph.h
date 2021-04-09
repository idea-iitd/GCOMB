#ifndef GRAPH_H
#define GRAPH_H

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
	static vector<int> correspond;
	static vector<int> index;
	static vector<Edge> edges;

	static void qsort_edges(int h, int t);

public:
	static void	Build();
	static int	GetN();
	static int	GetM();
	static int	GetDegree(int node);
	static int	GetNeighbor(int node);
	static Edge	GetEdge(int node, int idx);

        static void BuildFromFile2GIC(char *file);
	static void BuildIC();
	static void BuildWC();
	static void Build2GC();
	static void Build2WC(char file[]);
	static void Build2DWC();
	static void Build2IC();
	static void Build2DIC(double, double);
	static void Build2RIC(double, double);
	static void Build2TIC(double, double, double);
	static void Build2DT(double, double, double);
	static void Build2T(double, double, double);
	static void Stats();
};

#endif

