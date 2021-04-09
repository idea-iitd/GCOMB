#ifndef PAGERANK_H
#define PAGERANK_H

#include "limit.h"
#include <vector>
using namespace std;

class pagerank
{
private:
	static int n;
	static int top;
	//static int k;
	static double d[MAX_K];
	static int list[MAX_K];
	static char file[STR_LEN];
	//static vector<int> *neighbor;
	static vector<double> dp;
	static vector<int> dd;

public:
	static double Build(int num, double dampen=0.15);
	//static double Build(int num, int k, int bound, double (*Run)(int num_iter, int size, int set[]), double (*RunFast)(int num_iter, int size, int set[]));
	static void BuildFromFile();
	static int GetNode(int i);
	static int GetMax(int round);
	//static int GetMax0(int round);
	//static int generateSPT_newfrom(int round, int node);
	//static int generateSPT_newto(int node);
	//static char* filename(int bound);
};


#endif
