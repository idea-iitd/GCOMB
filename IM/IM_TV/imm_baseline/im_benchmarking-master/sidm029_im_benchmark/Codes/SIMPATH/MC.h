#ifndef MC_H
#define MC_H

#include "common.h"
#include "HashTreeMap.cc"
#include <vector>
#include <cmath>
#include <ctime>
#include <map>
#include <queue>

namespace _MC {

typedef map<UID, float> FriendsMap;
typedef HashTreeMap<UID, FriendsMap*> HashTreeCube;
typedef HashTreeMap<UID, multimap<float, UID>*> HashTreeCube2;
typedef map<int, unsigned int> RadiusCovMap;

struct MGStruct {
	UID nodeID; // user ID
	float gain; // MG(u|S)
	UID v_best; // user that gives best MG till now
	
	// marginal gain of user u w.r.t the curSeedSet + u_best. i.e., MG(u|S+v_best)
	float gain_next;
	int flag; // size of the seed set in current run
};

// to make a list sorted by MGs (or coverage)
typedef multimap<float, MGStruct*> Gains; 

struct NodeParams {
	float threshold;
	float inWeight;
	bool active;
};

class MC {

	AnyOption* opt;
	
	UserList curSeedSet; 
	UserList users;
	UserList covBestNode;	 // newly made for binary prob. case 
	//UserList covSeedSet;
    unsigned int numEdges;

	HashTreeCube *AM; 
    //HashTreeCube* AM_in;
    HashTreeCube2 *revAM;
    FriendsMap seedSetNeighbors;
	Gains mgs;

	string outdir;
	const char* probGraphFile;
	string m;
	PropModels model; 
	GraphType graphType;

	string problem;
	ofstream outFile;
	ofstream seedFile;
        ofstream statFile;
	int startIt;

	int countIterations;
    float path_tol;
	float totalCov; 
	float tol;

	// parameters to monitor the progress of experiments
	time_t startTime;
	time_t stime_mintime;

public:
	MC (AnyOption* opt);
	~MC ();
	void doAll();
	void readInputData(float alpha=0);
	PropModels getModel();
	float LTCov(UserList& list);
	float ICCov(UserList& list);
	
	void writeCovInFile(vector<UID> &seedVec, float tol);

	void setAM(HashTreeCube* AM1) {AM = AM1;}
    HashTreeCube *getAM() {return this->AM;}
    //HashTreeCube *getAMIn() {return this->AM_in;}
    HashTreeCube2 *getRevAM() {return this->revAM;}
    UserList *getUsers() {return &this->users;}
    unsigned int getNumEdges() {return this->numEdges;}

private:
	// functions called from constructor
	void setModel(AnyOption* opt);

	// functions called from doAll
	float mineSeedSet(int t_ub=0);
	float ICCov(UserList& list, int t_ub);
	void clear();
	void computeCov();
	
	/****** For CELF++ on Monte Carlo Simulation!  *****/
	float mineSeedSetPlus();
	bool ICCovPlus(MGStruct *pMG, MGStruct *pBestMG); // IC model + CELF++
    bool LTCovPlus(MGStruct *pMG, MGStruct *pBestMG, UserList &); // LT model + CELF++	

	// other private functions
	float getTime() const;
	float getTime_cur() const;
	void writeInFile(UID v, float cov, float marginal_gain, int curTimeStep, float actualCov, float actualMG, int countUsers);
	void openOutputFiles();
	void printVector(vector<UID>& vec, float pp);

};
}
#endif
