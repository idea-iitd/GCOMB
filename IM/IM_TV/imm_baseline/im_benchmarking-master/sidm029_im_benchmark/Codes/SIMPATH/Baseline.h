#ifndef BASELINE_H
#define BASELINE_H

#include "common.h"
#include "HashTreeMap.cc"

#include <map>
#include <deque>
#include <set>
#include <vector>
#include <queue>
#include <cstdio>
#include <algorithm>
#include <cmath>
#include "MC.h"
#include "LDAG.h"

using namespace _MC;

namespace _Baseline {

class Baseline {

    AnyOption *opt;
    MC *mc;

    _MC::HashTreeCube *AM;
    UserList *pUsers;
    vector<UID> usersVec;
    UserList curSeedSet;
    vector<UID> seedsVec;
    unsigned int numEdges;

    string outdir;
	const char* probGraphFile;
	string m;
	PropModels model; 
	GraphType graphType;
	string problem;
	ofstream outFile;
    int countIterations;

    float totalCov;

    // parameters to monitor the progress of experiments
	time_t startTime;
	time_t stime_mintime;

public:
    Baseline(AnyOption *);
    ~Baseline();
    
    bool checkDS();
    void doAll();

    void setAM(_MC::HashTreeCube *AM1) {AM = AM1;}
    void setUsers(UserList *pUsers1) {pUsers = pUsers1;}
    void setNumEdges(unsigned int numEdges1) {numEdges = numEdges1;}

    void mineSeedRand();
    void mineSeedHighDeg();
    void mineSeedPR(); 
    void writeCovInFile();

    float LTCov(UserList &); // the MC coverage function

private:
    void setModel(AnyOption *opt);
    float getTime() const;
    void openOutputFiles();
    void writeInFile(UID v, float cov, float marginal_gain, int curTimeStep, float actualCov, float actualMG, int countUsers);
    

};

}


#endif

