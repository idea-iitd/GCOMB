#ifndef InfluenceModels_H
#define InfluenceModels_H

//class core;
#include "common.h"
#include "anyoption.h"
#include "MC.h"

using namespace _MC;

class InfluenceModels {
	
	int pi, debug;
	unsigned int maxTuples;

		// Data Structures
//	bool AM_graph[users][users];
//	unsigned int IM[users][actions] ;

	time_t start, wastedTime;	
	const char* outdir;
	string training_dir;

	int actionsProcessTillNow;
	
	int dataset_size;

	// phase
	// 1 : training only
	// 2 : both training and testing
	// 3 : testing only
	int phase;
	
public:
	InfluenceModels();
	~InfluenceModels();

	void doAll(int argc, char* argv[]);
	void writeSummary(size_t tcount);

	time_t getStartTime() const {
		return start;
	}

	time_t getWastedTime() const {
		return wastedTime;
	}

	void setWastedTime(time_t t) {
		wastedTime = t;
	}
//	float getCurrentMemoryUsageNew();
	
private:
	AnyOption* readOptions(int argc, char* argv[]);

};
#endif
