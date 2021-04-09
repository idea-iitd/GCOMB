#ifndef LDAG_H
#define LDAG_H

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

using namespace _MC;

namespace _LDAG {


//typedef	map<UID, float> FriendsMap;
//typedef map<UID, FriendsMap *> FriendsMatrix;
//typedef HashTreeMap<UID, FriendsMap *> HashTreeCube;
/*
struct NodeParams {
	float threshold;
	float inWeight;
	bool active;
};
*/
struct MGStruct{
	UID uid;
	float gain;
};

class LDAG {

	AnyOption *opt;
	
	UserList curSeedSet;
    vector<UID> seedVec;

	UserList users;
    _MC::HashTreeCube *AM_out;
    _MC::HashTreeCube *AM_in;
	float totalCov;
	float currentMG;
	map<UID, MGStruct *> MGHeap;
	multimap<float, MGStruct *> revMGHeap;
	
	float tol;
	// LDAG specific data structures
	//FriendsMatrix LDAGInfs;
	map<UID, deque<UID> *> LDAGNodes; // LDAG nodes in topological order
	map<UID, set<UID> *> infSets; // nodes : LDAGs of which they belong to
    _MC::HashTreeCube *alphaMap; // all ap(v,{u})
    _MC::HashTreeCube *apMap; // all ap_{v}(u,S) in LDAG(v)
	
	string outdir;
	const char *probGraphFile;
	string m;
	PropModels model;
	GraphType graphType;
	
	time_t startTime;
	string problem;
	ofstream outFile;
	ofstream seedFile;
        ofstream statFile;
public:
	LDAG(AnyOption *opt);
	~LDAG();
	void doAll();	 
	
private:
	void setModel(AnyOption *opt);
	void readInputData();
	
	void initSeedSet(); // initialize LDAGs, seed set, and marginal gains
	float mineSeedSet(); // select k seeds with LDAG
	
	void greedyLDAG(UID, float);
	void initAlpha(UID);
	
	
	// float weight(UID, UID); // find w(u,v) in AM_out
	
	void clearAll();
	
	// other functions
	float getTime() const;
	void openOutputFile();
	void writeOutputFile(UID v, float cov, float marginal_gain, int curTimeStep);

	
};


}

#endif

