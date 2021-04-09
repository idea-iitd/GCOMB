#ifndef SPM_GC_H
#define SPM_GC_H

#include "limit.h"

class SPM_gc 
{
private:
	static int	n, m;
	static int	targetSize;
	static double	resultSize;
	static int	target[MAX_NODE];
	static bool built;
	
public:
	static void Build();
	static void SetTarget(int size, int set[]);
	static double Run(int itr_num, int size, int set[]);
};

#endif
