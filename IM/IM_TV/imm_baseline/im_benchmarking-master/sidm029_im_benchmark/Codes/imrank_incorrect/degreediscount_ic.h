#ifndef DEGREE_DISCOUNT_IC_H
#define DEGREE_DISCOUNT_IC_H

#include "limit.h"
#include<vector>
using namespace std;

class DegreeDiscount_IC
{
private:
	static int n;
	static vector<int> d;
	static int list[MAX_K];
	static char file[STR_LEN];

//	static void qsort_degree(int h, int t);

public:
	static void Build(double ratio);
	static void BuildFromFile();
	static int GetNode(int i);
};

#endif

