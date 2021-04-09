#ifndef IRIE_H
#define IRIE_H

#include "limit.h"
#include <vector>
#include <set>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

class IRIE
{
	private:
		static int n;
		static int top;
		static double totalAP;
		static double d[MAX_K];
		static int list[MAX_K];
		static char file[STR_LEN];
		static vector<bool> visited;
		static vector<double> dp;
		static vector<int> dd;
		static vector<double> ap;
		static vector<bool> seed;
		static vector<double> od;
		static bool saturated;
	public:
		static double Build(int num, double bound1, double bound2, char data[]);
		static void getAnswer(double alpha);
		static double initialInfluence(double alpha, int round);
		static double residualInfluence(double alpha, int round);
		static void computeAP(int src, int theta);
		static double EDV();
		static double getOptimalAlpha(double bound1, double bound2);
		static int GetMax();
		static int GetNode(int i);
		static void outputSeedSet(double alpha, char data[]);
		static void outputDummy(char data[]);
};


#endif

