#ifndef GREEDY_H
#define GREEDY_H

#include "limit.h"

class Greedy
{
private:
	static int n;
	static int top;
	static double d[MAX_NODE];
	static int list[MAX_NODE];
	static char file[STR_LEN];

public:
	static void Build(int k, int R, double (*Run)(int num_iter, int size, int set[]));
	static void BuildFromFile(const char*);
	static int GetNode(int i);
};

#endif

