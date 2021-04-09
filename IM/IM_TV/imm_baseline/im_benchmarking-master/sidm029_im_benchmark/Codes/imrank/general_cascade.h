#ifndef GENERAL_CASCADE_H
#define GENERAL_CASCADE_H

#include "limit.h"

class GeneralCascade 
{
private:
	static int	n, m;
	static int	targetSize;
	static int	resultSize;
	static int	target[MAX_K];
	static bool built;

	
public:
	static void Build();
	static void SetTarget(int size, int set[]);
	static double Run(int num_iter, int size, int set[]);
	static double mean;
};

#endif
