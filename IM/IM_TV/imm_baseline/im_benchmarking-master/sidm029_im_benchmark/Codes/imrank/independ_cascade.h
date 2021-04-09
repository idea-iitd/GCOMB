#ifndef INDEPEND_CASCADE_H
#define INDEPEND_CASCADE_H

#include "limit.h"

class IndependCascade 
{
private:
	static int	n, m;
	static int	targetSize;
	static int	resultSize;
	static int	target[MAX_NODE];
	static bool built;
	static double ratio;
	
public:
	static void Build(double ratio);
	static void SetTarget(int size, int set[]);
	static double Run(int itr_num, int size, int set[]);
};

#endif
