#ifndef MIA_H
#define MIA_H

#include "limit.h"
#include <vector>
using namespace std;

class MIA
{
private:
	static int n;
	static int top;
	static int k;
	static double d[MAX_K];
	static int list[MAX_K];
	static char file[STR_LEN];
	static int topk[MAX_K];
	static vector<int> dd;
	static double longest;
	static vector<double> dp;
	static vector<bool> used;
	static vector<double*> self;
	static vector<int> lastupdate;
	static vector<double *>delta;
	static vector<int *>children, path;
	static int *S, *numchild, *queue;
	static double *distance, *b;
	static int *heap;
	static int *childlist, *oldchildlist, *parent;
	static bool *validlist[MAX_K];
	static vector<int> *childnum;
	static vector<double> *allb;

public:
	static double Build(int num, int bound);
	static double Build(int num, int k, int bound, double (*Run)(int num_iter, int size, int set[]), double (*RunFast)(int num_iter, int size, int set[]));
	static void BuildFromFile(int bound);
	static int GetNode(int i);
	static int GetMax(int round);
	static int GetMax0(int round);
	static int generateMIAfrom(int round, int node);
	static int generateMIAto(int node);
	static int generateMIAto0(int node);
	static int count(int node);
	static char* filename(int bound);
};

#endif

