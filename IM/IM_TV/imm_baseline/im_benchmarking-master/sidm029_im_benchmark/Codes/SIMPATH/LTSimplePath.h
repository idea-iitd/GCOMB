#ifndef LTSIMPLEPATH_H
#define LTSIMPLEPATH_H

#include "common.h"
#include "HashTreeMap.cc"

#include <map>
#include <deque>
#include <set>
#include <list>
#include <vector>
#include <queue>
#include <cstdio>
#include <algorithm>
#include <cmath>
#include "MC.h"
#include "LDAG.h"

using namespace _MC;

namespace _LTSimplePath {

struct xNode {
    UID id;         // user id
    bool flag;      // on current path?
    double cov;      // cov^{V-x}(w), for non-VC neighbors of w only 
    double tol;      // tolerance on current path
    double prob;     // incoming edge probability on current path
    double pp;  // propagation prob. of the current path from starting point to exact this node

    multimap<double, xNode *> *N_in;
    multimap<double, xNode *> *N_out;

    int out_deg; 
    int in_deg;
    int leafs; // number of in-neighbors whose out-deg is 1

    xNode *next;    // to use in the linked-list in the buffer

    xNode() : id(0), flag(false), cov(0), tol(0), prob(0), pp(0), N_in(NULL), N_out(NULL), next(NULL) {}

    xNode(UID id1, bool flag1, double cov1, double tol1, double prob1, double pp1) : id(id1), flag(flag1), cov(cov1), tol(tol1), prob(prob1), pp(pp1), N_in(NULL), N_out(NULL), next(NULL) {}

};  

typedef HashTreeMap< UID, map<UID, xNode *> * > HashTreeCovMap;

// for CELF++
struct MGStruct {
	UID nodeID; // user ID
	double gain; // MG(u|S)
	UID v_best; // user that gives best MG till now
	
	// marginal gain of user u w.r.t the curSeedSet + u_best. i.e., MG(u|S+v_best)
	double gain_next;
	int flag; // size of the seed set in current run
};
typedef multimap<double, MGStruct*> Gains;

class LTSimplePath {
    
    AnyOption *opt;
    MC *mc;

    UserList users; // set of all users
    unsigned int edges;
    UserList vertexCover; // set of all vertex cover nodes
    UserList outsideNodes; // set of non vertex cover nodes (may be not needed)
   
    UserList curSeedSet; // set of current seeds
    vector<UID> seedVec;
    
    map<UID, xNode *> *userNodes; // global map maintaining all xNode structs
    //map<UID, set<UID> *> *adjMap; // adjacency lists, used for vertex cover

    map<UID, double> outsideCovMap; // store coverage for each node that is outside VC
    map<UID, double> covSMap; // store cov(S) on V-x for each x
    map<UID, double> covXMap; // store cov(x) on V-S for each x
    vector<double> covPlus; // cov(S+x) on G and G-x
	//map<UID, double> hopsMap; // store avgNumHop(x) for each x  

	string outdir;
	const char* probGraphFile;
	string m;
	PropModels model; 
	GraphType graphType;
	string problem;
ofstream outFile;
        ofstream seedFile;
        ofstream statFile;
	ofstream hopsOutFile; // store avgNumHop(x) for each x
	int startIt;
    int countIterations;
	int topl;
    int celfPlus;
    int optimal;

    double totalCov;
    //double path_tol;
    double cutoff_tol;

    // parameters to monitor the progress of experiments
	time_t startTime;
	time_t stime_mintime;

    xNode* freeTuples;
    int freeTuplesSize;


public:
    LTSimplePath(AnyOption *opt);
    ~LTSimplePath();

    void readInputData();
    void doAll();
    
    double mineSeedSet(); // mine seeds use CELF and factoring optimization
    double mineSeedSetPlus(); // mine seeds CELF++
    double simPath(UserList &, UserList &, int iteration); // an iterface function that call backtrack
    bool simPathPlus(MGStruct *pCurr, MGStruct *pBest, UserList &S, UserList &T, int iteration);

    double backtrackBasic(UID v, UserList &S); // used for computer cov(x) on V-S
    double backtrackFirst(xNode *, UserList &T, map<UID, double> &); // used in first iteration 
    double backtrackSubsequent(xNode *, UserList &S, UserList &T); // used for subsequent iterations
    double backtrackPlus(xNode *, UserList &S, UserList &T); // used when CELF++ is used
    
    void vertexCoverMaxDeg();

    inline xNode *getNewTuple(UID id1, bool flag1, double cov1, double tol1, double prob1, double pp1);
	inline void deleteTuple(xNode *); // function to optimize memory new/free calls
    inline void init_linked_list();


private:

    void setModel(AnyOption *opt);
    float getTime() const;
    void openOutputFiles();
    void writeInFile(UID v, float cov, float marginal_gain, int curTimeStep, float actualCov, float actualMG, int countUsers);
};


inline xNode *LTSimplePath::getNewTuple(UID id1, bool flag1, double cov1, double tol1, double prob1, double pp1) {
    xNode *cur = NULL;
    if (freeTuples != NULL) {
        cur = freeTuples;
        freeTuples = freeTuples->next;
        
        cur->id = id1;
        cur->flag = flag1;
        cur->cov = cov1;
        cur->tol = tol1;
        cur->prob = prob1;
        cur->next = NULL;
        cur->pp = pp1;

        freeTuplesSize--;

    } else {
        cur = new xNode(id1, flag1, cov1, tol1, prob1, pp1);
    }
    
    return cur;
}


inline void LTSimplePath::deleteTuple(xNode *tuple) {
    if (freeTuplesSize < 1000) {
        tuple->next = freeTuples;
        freeTuples = tuple;
        ++freeTuplesSize;
    } else {
        delete tuple;
    }
}

inline void LTSimplePath::init_linked_list() {
    xNode *p, *p_prev;
    
    p = new xNode();
    p_prev = p;
    freeTuples = p;

    for (int i = 1; i < 300000; ++i) {
        p = new xNode();
        p_prev->next = p;
        p_prev = p;
    }

}


}
#endif

