#ifndef LTTEST_H
#define LTTEST_H

#include "common.h"
#include "HashTreeMap.cc"

#include <map>
#include <deque>
#include <set>
#include <vector>
#include <queue>
#include <cstdio>
#include <algorithm>
#include "MC.h"
#include "LDAG.h"
#include "LTSimplePath.h"
#include "Baseline.h"

using namespace _MC;
using namespace _LTSimplePath;
using namespace _Baseline;

struct VNode {
    UID id;
    int degree;
    int leaf_num;
    vector<VNode *> neighborVec;
};

class LTTest {

    AnyOption *opt;

    _MC::HashTreeCube *AM;
    UserList *users;
    UserList curSeedSet;
    unsigned int numEdges; 

    string outdir;
	const char *probGraphFile;
	string m;
	PropModels model;
	GraphType graphType;
	
	time_t startTime;
	string problem;
	ofstream outFile;

public:
    LTTest(AnyOption *opt);
    ~LTTest();
    void doAll();

private:

    void setModel(AnyOption *opt);
    void clearAll();
	float getTime() const;
	void openOutputFile();
    void writeOutputFile(UID v, float cov, float marginal_gain, int curTimeStep);

};


#endif

