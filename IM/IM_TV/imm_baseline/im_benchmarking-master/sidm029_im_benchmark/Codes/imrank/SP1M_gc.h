#ifndef SP1M_GC_H
#define SP1M_GC_H

#include "limit.h"

class SP1M_gc 
{
private:
	static int	n, m;
	static int	targetSize;
	static double	resultSize;
	static int	target[MAX_NODE];
	static bool built;
	//static double ratio;
	
public:
	static void Build();
	static void SetTarget(int size, int set[]);
	static double Run(int itr_num, int size, int set[]);
};

#endif
