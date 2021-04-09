#ifndef IR_H
#define IR_H

#include "limit.h"
#include <vector>
#include <set>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

class IR
{
private:
	static int n;
	static int top;
	static double d[MAX_K];
	static int list[MAX_K];
	static char file[STR_LEN];
	static vector<double> dp;
	static vector<int> dd;

public:
	static double Build(int num, double alpha, char data[]);
	static int GetMax();
	static int GetNode(int i);
};


#endif
