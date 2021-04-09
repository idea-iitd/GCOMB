#include "Baseline.h"
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <iomanip>

namespace _Baseline {

Baseline::Baseline(AnyOption *opt1) {
    opt = opt1;

    cout << "In testing phase" << endl;

    outdir = opt->getValue("outdir");
	probGraphFile = opt->getValue("probGraphFile");
//	tol = strToFloat(opt->getValue("tol_ldag"));
	graphType = DIRECTED;
 
    AM = NULL;
    pUsers = NULL;

    countIterations = strToInt(opt->getValue("mcruns"));
    setModel(opt);

//    srand(time(NULL));

}


Baseline::~Baseline() {
    cout << "Total time taken : " << getTime() << endl;
}


void Baseline::setModel(AnyOption *opt) {
    m = opt->getValue("propModel");
    model = Rand; // just by default
    
    if (m.compare("LT") == 0) {
		model = LT;	
	} else if (m.compare("IC") == 0) {
		model = IC;
	} else if (m.compare("PC") == 0) {
		model = PC;
	} else if (m.compare("LTNew") == 0) {
        model = LTNew;
    } else if (m.compare("Rand") == 0) {
        model = Rand;
    } else if (m.compare("HighDeg") == 0) {
        model = HighDeg;
    } else if (m.compare("PageRank") == 0) {
        model = PageRank;
    } 

}


bool Baseline::checkDS() {
    bool ret = true;
    if (AM == NULL) {
        cout << "AM is NULL!" << endl;
        ret = false;
    } else if (pUsers == NULL) {
        cout << "pUsers is NULL!" << endl;
        ret = false;
    }
    return ret;
}


void Baseline::doAll() {

    usersVec.clear();
    for (UserList::iterator i = pUsers->begin(); i != pUsers->end(); ++i) {
        usersVec.push_back(*i);
    }

    time(&startTime);
    if (model == Rand) {
        cout << "model is : " << model << endl;
        cout << "Random!" << endl;
        mineSeedRand();
    } else if (model == HighDeg) {
        cout << "model is : " << model << endl;
        cout << "High Degree" << endl;
        mineSeedHighDeg();
    } else if (model == PageRank) {
        cout << "model is : " << model << endl;
        cout << "PageRank" << endl;
        mineSeedPR();
    }

    outFile.close();
}


void Baseline::mineSeedRand() {

    cout << "In mineSeedRand" << endl;

    curSeedSet.clear();
    seedsVec.clear();
    int budget = 50;
    //totalCov = 0;

    int numUsers = pUsers->size();

    while (curSeedSet.size() < budget) {
        int rand_num= rand() % numUsers + 1;
        UID u = usersVec.at(rand_num-1);
        
        // when current seed set does not contain this random user
        if (curSeedSet.find(u) == curSeedSet.end()) {
            cout << setprecision(8) << "Picked a seed: " << u << ", time = " << getTime()<< endl;
            curSeedSet.insert(u);
            seedsVec.push_back(u);
        }
    }

    writeCovInFile();
}


void Baseline::mineSeedHighDeg() {

    cout << "In mineSeed HighDeg" << endl;

    curSeedSet.clear();
    seedsVec.clear();
    int budget = 50;
    //totalCov = 0;

    multimap<int, UID> degQueue;
    
    int length = AM->length();

    // rank nodes by degrees
    for (int i = 0; i < length; i++) {
		map<UID, FriendsMap*> curRow = AM->getRow(i);
        
        for (map<UID, FriendsMap*>::iterator j = curRow.begin(); j != curRow.end(); ++j) {
	        UID u = j->first;
            int degree = j->second->size();
            degQueue.insert(std::make_pair(degree, u));
        }
	}

    // select seeds from the highest degree ones
    for (multimap<int, UID>::reverse_iterator i = degQueue.rbegin(); i != degQueue.rend(); ++i) {
        if (curSeedSet.size() >= budget)
            break;
        UID u = i->second;
        cout << setprecision(8) << "Picked a seed: " << u  << ", deg = " << i->first << ", time = " << getTime() << endl;

        curSeedSet.insert(u);
        seedsVec.push_back(u);
    }

    cout << "*** Total time take for HighDegree: " << getTime() << " ***" << endl;

    writeCovInFile();

}

void Baseline::mineSeedPR() {
    cout << "In mineSeed PageRank" << endl;

    map<UID, map<UID, double> *> AM_in;

    curSeedSet.clear();
    seedsVec.clear();
    int budget = 50;

    map<int, UID> IDMap;
    map<UID, int> revIDMap;
    set<UID> zeroDegNodes;

    int numUsers = (int)pUsers->size();
    // assign IDs from 0 to n-1 to nodes
    int k = 0;
    for (UserList::iterator i = pUsers->begin(); i != pUsers->end(); ++i) {
        UID u = *i;
        IDMap.insert(std::make_pair(k, u));  // standard id --> real user id
        revIDMap.insert(std::make_pair(u, k)); // real user id --> standard id
        k++;

        FriendsMap *neighbors = AM->find(u);
        if (neighbors == NULL || neighbors->empty()) {
            // zero outdegree node
            cout << "Found a node with 0 out-degree, UID = " << u << endl;
            zeroDegNodes.insert(u);
        } else {
            // non-zero outdegree node
            int outdegree = (int)neighbors->size();
            for (FriendsMap::iterator j = neighbors->begin(); j != neighbors->end(); ++j) {
                // get an edge: u-->v, w.p. j->second
                UID v = j->first;
                double prob = 0.85 / outdegree + 0.15 / numUsers;

                // in-links (in-neighbors)
                map<UID, double> *in_neighbors = NULL;
                map<UID, map<UID, double> *>::iterator mit = AM_in.find(v);
                if (mit == AM_in.end()) {
                    in_neighbors = new map<UID, double>();
                    in_neighbors->insert(std::make_pair(u, prob));
                    AM_in.insert(std::make_pair(v, in_neighbors));
                } else {
                    in_neighbors = mit->second;
                    in_neighbors->insert(std::make_pair(u, prob));
                } //endif

            } //endfor
        } //endif
    } //endfor

    
    // check if there is need to deal with zero degree nodes
    if (zeroDegNodes.empty() == false) {
        cout << "Number of zero outdegree nodes = " << zeroDegNodes.size() << endl;
        // Do the normalization here:
        for (set<UID>::iterator i = zeroDegNodes.begin(); i != zeroDegNodes.end(); ++i) {
            UID w = *i; // UID of the zero out-deg node
            double prob = 1.0 / numUsers;
            
            for (UserList::iterator j = pUsers->begin(); j != pUsers->end(); ++j) {
                UID v = *j;
                map<UID, double> *in_neighbors = NULL;
                map<UID, map<UID, double> *>::iterator mit = AM_in.find(v);
                if (mit != AM_in.end()) {
                    in_neighbors = mit->second;
                    in_neighbors->insert(std::make_pair(w, prob));
                } else {
                    in_neighbors = new map<UID, double>();
                    in_neighbors->insert(std::make_pair(w, prob));
                    AM_in.insert(std::make_pair(v, in_neighbors));
                }
            }
        }
    } 

    // **** Start of Power Method ****
    //

    cout << "Start Time of Power Method for PageRank: " << getTime() << endl;

    vector<double> rankVec(numUsers, 1.0 / numUsers);
    vector<double> prevVec(numUsers, 0.0);
    vector<double> tempVec(numUsers, 0.0);

    double epsilon = 0.000001;  // stopping criteria: 10^-6
    double residual = 1;

    int num_iter = 0;
    double norm_prob = 0.15 / numUsers;
    while (residual >= epsilon) {
        num_iter++; // one more iteration
        cout << "In iteration No. " << num_iter << endl;
        
        if (rankVec.size() != pUsers->size() || prevVec.size() != pUsers->size() || tempVec.size() != pUsers->size()) {
            cout << "vector dimensions do not match (1)" << endl;
            exit(1);
        } //endif

        prevVec.assign(rankVec.begin(), rankVec.end()); // prevVec <- rankVec

        // compute each user's page rank value for this new iteration
        set<int> idxSet; 
        map<UID, double> *in_neighbors;
        map<UID, map<UID, double> *>::iterator mit;
        
        for (int k = 0; k < pUsers->size(); ++k) {
            UID v = IDMap.find(k)->second;
            double prValue = 0.0;
           
            in_neighbors = NULL;
            mit = AM_in.find(v);

            if (mit != AM_in.end()) {
                in_neighbors = mit->second;
                for (map<UID, double>::iterator it = in_neighbors->begin(); it != in_neighbors->end(); ++it) {
                    UID u = it->first;
                    int idx = revIDMap.find(u)->second;
                    prValue += (double) rankVec.at(idx) * it->second;
                } //endfor
            } 

            tempVec.at(k) = prValue;
        }

        // update pagerank vector for current iteration
        rankVec.assign(tempVec.begin(), tempVec.end());

        // calculate residual, in 1-norm
        if (rankVec.size() == prevVec.size()) {
            residual = 0.0;
            for (int k = 0; k < rankVec.size(); ++k) {
                residual += abs(rankVec.at(k)-prevVec.at(k));
            } //endfor
        } else {
            cout << "vector dimensions do not match (2)" << endl;
            exit(2); 
        } //endif

    } //endwhile

    cout << "Number of iterations till convergence: " << num_iter << endl;

    // sort pagerank values
    multimap<double, int> rankMap;
    for (int k = 0; k < rankVec.size(); ++k) {
        rankMap.insert(std::make_pair(rankVec.at(k), k));
    }

    // select seeds, from highest page rank node
    multimap<double, int>::reverse_iterator rit;
    int counter = 0;
    for (rit = rankMap.rbegin(); rit != rankMap.rend(); ++rit) {
        counter++;
        if (counter > budget)
            break;
    
        int idx = rit->second;
        UID u = IDMap.find(idx)->second;
    
        curSeedSet.insert(u);
        seedsVec.push_back(u);
    }

    // compute coverage using MC and output to file
    //writeCovInFile();
}


void Baseline::writeCovInFile() {
    cout << "In writeCovInFile" << endl;
    
    string filename;

    if (model == Rand) {
        filename = outdir + "/" + "Rand"+".txt"; 
    } else if (model == HighDeg) {
        filename = outdir + "/" + "HighDeg"+".txt";
    } else if (model == PageRank) {
        filename = outdir + "/" + "PageRank"+".txt";
    }

    ofstream outFile (filename.c_str(), ios::out);

    UserList S;
	for(vector<UID>::iterator i = seedsVec.begin(); i != seedsVec.end(); ++i) {
		UID v = *i; 
		S.insert(v);
        //cout << "Seed No. " << S.size() << " " << v << endl;
		//if (S.size() % 10 != 0) continue;
//		if (S.size() == 1 || S.size() % 5 == 0) {
			float cov = 0;
//            float cov = LTCov(S);
		    outFile << S.size() << " " << v << " " << cov << endl;
	    	cout << S.size() << " " << v << " " << cov << endl;
  //      }
				
	}

	outFile.close();

	cout << "Done writeCovInFile" << endl;

}

float Baseline::LTCov(UserList& S) {
//    cout << "In LTCov" << endl;
    float tol = 0.00001;
    unsigned long long cov = 0;

//    map<UID, float> ppIn; // what is the prob with which the node is covered
   
    for (int b = 0; b < countIterations; ++b) {
        // initialize random seed: 

        float cov1 = 0;
        // T is the set of nodes that are to be processed
        queue<UID> T;
        // Q is the set of nodes that have been seen until now
        // Thus, Q is a superset of T
        // Q contains the nodes that are processed and to be processed
        map<UID, NodeParams> Q;

        cov += S.size();
        // cov1 == coverage in one run .. that is, number of nodes reachable
        cov1 += S.size();
       
        for (UserList::iterator i=S.begin(); i!=S.end(); ++i) {
            UID v = *i;
 //           ppIn[v] += 1;

            FriendsMap* neighbors = AM->find(v);
            if (neighbors != NULL) {
                for (FriendsMap::iterator j = neighbors->begin(); j!=neighbors->end(); ++j) {
                    UID u = j->first;

                    if (S.find(u) == S.end()) {

                        if (Q.find(u) == Q.end()) {
                            // if the node u has not been seen before
                            // create a new NodeParams
                            // create a random threhsold
                            NodeParams& np = Q[u];
                            np.active = false;
                            np.inWeight = j->second;

                            // generate secret number: 
                            np.threshold = ((float)(rand() % 1001))/(float)1000;
                            T.push(u);
                        } else {
                            NodeParams& np = Q[u];
                            np.inWeight += j->second;                   
                        } //endif
                    } //endif
                } //endfor
            } //endif
        } //endfor

        while (!T.empty()) {
            UID u = T.front();

//            cout << "T.size " << T.size() << endl;

            NodeParams& np = Q.find(u)->second;
            if (np.active == false && np.inWeight >= np.threshold + tol) {
//                ppIn[u] += 1;
                np.active = true;
                cov++;
                cov1++;

                // add u's neighbors to T
                FriendsMap* neighbors = AM->find(u);
                if (neighbors != NULL) {
                    // for each neighbor w of u
                    for (FriendsMap::iterator k = neighbors->begin(); k!=neighbors->end(); ++k) {
                        UID w = k->first;
                        // is w is in S, no need to do anything
                        if (S.find(w) != S.end()) continue;

                        // if w is not in S, locate it in Q
                        map<UID, NodeParams>::iterator it = Q.find(w);

                        if (it == Q.end()) {
                            // if it is not in Q, then
                            NodeParams& np_w = Q[w];
                            np_w.threshold = ((float)(rand() % 1001))/(float)1000;
//                            np_w.threshold = (float)rand()/RAND_MAX;
                            np_w.active = false;
                            np_w.inWeight = k->second;
                            T.push(w);
                        } else {
                            // if w is in Q, then
                            NodeParams& np_w = it->second;
                            if (np_w.active == false) {
                                T.push(w);
                                np_w.inWeight += k->second;

                                if (np_w.inWeight - 1 > tol) {
                                    cout << "Something wrong, the inweight for a node is > 1. (w, inweight) = " << w << ", " << np_w.inWeight - 1<< endl;
                                }
                            }
                        }
                    }
                }
            }

            // deletes the first element
            T.pop();

        } //endwhile
//        cout << "Coverage in this iteration: " << cov1 << endl;
    }

	
    // coverage from ppIn
    //float cov1 = 0;


    //for (map<UID, float>::iterator i = ppIn.begin(); i!=ppIn.end(); ++i) {
    //    cov1 += i->second;
    //    cout << "ppIN: " << i->first << " " << i->second/countIterations << endl;
    //}
    
    //cov1 = cov1/countIterations;
	
    double retCov = (double) cov/countIterations;

//    cout << "(cov, retCov, cov1) = " << cov << ", " << retCov << ", " << cov1 << endl;

    return retCov;
   

}


float Baseline::getTime() const {
	time_t curTime;
	time(&curTime);

	float min = ((float)(curTime - startTime))/60;
	return min;
}


void Baseline::openOutputFiles() {

	if (outFile.is_open()) {
		outFile.close(); 
	}
	
	string algorithm = "Baseline";
	cout << "problem : " << problem << endl;
	string filename = outdir + "/" + m + "_" + algorithm + ".txt";
	outFile.open (filename.c_str());

	if (outFile.is_open() == false) {
		cout << "Can't open file " << filename << " for writing" << endl;
		exit(1);
	}

}


void Baseline::writeInFile(UID v, float cov, float marginal_gain, int curTimeStep, float actualCov, float actualMG, int countUsers) {
    cout << endl << endl << "Picked a seed node: " << v << ", total: " << curSeedSet.size() << endl;
	outFile << v << " " << cov << " " << marginal_gain << " " << curTimeStep << " " << getCurrentMemoryUsage() << " " << getTime() <<  " " << " " << actualCov << " " << actualMG << " " << countUsers << endl;
	cout << v << " " << cov << " " << marginal_gain << " " << curTimeStep << " " << getCurrentMemoryUsage() << " " << getTime() << " " <<  " " << actualCov << " " << actualMG << " " << countUsers << endl;
	cout << endl << endl;
}



// end of name space here...
}
