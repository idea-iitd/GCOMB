#ifndef WEIGHTED_DEGREE_H
#define WEIGHTED_DEGREE_H

#include "limit.h"
#include <vector>
using namespace std;
class WeightedDegree
{
private:
	static int n;
	static vector<double> d;
	static int list[SET_SIZE];
	static char file[STR_LEN];

	static void qsort_degree(int h, int t);

public:
	static void Build();
	static void BuildFromFile();
	static int GetNode(int i);
};

#endif

