#ifndef RANDOM_H
#define RANDOM_H

#include "limit.h"
#include<vector>
using namespace std;

class Random
{
private:
	static int n;
	static vector<int> list;
	static char file[STR_LEN];

public:
	static void Build();
	static void BuildFromFile();
	static int  GetNode(int i);	
};

#endif

