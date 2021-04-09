#include <iomanip>
#include "MC.h"

namespace _MC {

MC::MC(AnyOption* opt1) {
	opt = opt1;
    outdir = opt->getValue("outdir");	
	probGraphFile = opt->getValue("probGraphFile");
	graphType = DIRECTED;

	setModel(opt);

    AM = new HashTreeCube(89041);
    if (model == LTNew) {
        revAM = new HashTreeCube2(89041);
    } else {
        revAM = NULL;
    }
	
	countIterations = strToInt(opt->getValue("mcruns"));
    
	cout << "User specified options: " << endl;
	cout << "model : " << m << " or " << model << endl;
	cout << "outdir : " << outdir << endl;
	cout << "probGraphFile : " << probGraphFile << endl;
	cout << "Number of iterations in MC : " << countIterations << endl;

	time (&startTime);
	//srand ( startTime );
	time (&stime_mintime);

}

MC::~MC() {
	cout << "total time taken : " << getTime() << endl;
}


void MC::setModel(AnyOption* opt) {
	m = opt->getValue("propModel");
	
	model = LT;

	if (m.compare("LT") == 0) {
		model = LT;
		
	} else if (m.compare("IC") == 0) {
		model = IC;
		
	} else if (m.compare("PC") == 0) {
		model = PC;
	} else if (m.compare("LTNew") == 0) {
		model = LTNew;
	}
	

}



void MC::doAll() {

	readInputData();

	cout << "Model " << m << " : " << model << " running" << endl;

	int phase = strToInt(opt->getValue("phase"));

	if (phase == 11) {
        cout << "Phase 11." << endl;
		computeCov();
	} else {
		openOutputFiles();

	    int celfPlus = strToInt(opt->getValue("celfPlus"));	
		if (celfPlus == 1){
			cout << "CELF++.." << endl;
            mineSeedSetPlus();
        } else {
            cout << "CELF..." << endl;
            mineSeedSet();
        }

		outFile.close();

	}

}

void MC::computeCov() {
	// read the seed set from the file
	// for each radius R, generate the table 
	// <S_alg, R, cov^R>
    curSeedSet.clear();
    vector<UID> seedsVec;
    seedsVec.clear();

	//int seeds = -1; 
	int seeds = 0; 

	//	str budget = intToStr(curSeedSet.size());
	string seedFileName = opt->getValue("seedFileName");
	string filename = seedFileName + "_cov.txt" ;
	ofstream outFile (filename.c_str(), ios::out);

	cout << "Seedfilename: " << seedFileName << "; outputfile: " << filename << endl;

	ifstream myfile (seedFileName.c_str(), ios::in);
	string delim = " \t";	
	if (myfile.is_open()) {
		while (! myfile.eof() )	{
			std::string line;
			getline (myfile,line);
			if (line.empty()) continue;
			seeds++;
			//if (seeds == 0) continue; // ignore the first line
			std::string::size_type pos = line.find_first_of(delim);
			int	prevpos = 0;

			UID u = 0;
			// get the user
			pos = line.find_first_of(delim, prevpos);
			if (pos == string::npos) 
				u = strToInt(line.substr(prevpos));
			else
				u = strToInt(line.substr(prevpos, pos-prevpos));

            seedsVec.push_back(u); // push into the vector
            cout << "Seed No. " << seedsVec.size() << ": " << u << endl;
			curSeedSet.insert(u);

		}
	}

	myfile.close();

	float cov = 0;
	if (model == IC) {
		cout << "Computing the cov under IC model" << endl;
		cov = ICCov(curSeedSet);
	} else if (model == LT || model == LTNew) {
		cout << "Computing the cov under LT model" << endl;
		//cov = LTCov(curSeedSet);

        UserList S;
        S.clear();
        for (vector<UID>::iterator i = seedsVec.begin(); i != seedsVec.end(); ++i) {
            UID v = *i;
            S.insert(v);

            if (S.size() >= 51)
                break;
            // 
            if (S.size() == 1 || S.size() % 5 == 0) {
                float cov1 = LTCov(S);
                outFile << S.size() << " " << v << " " << cov1 << endl;
                cout << S.size() << " " << v << " " << cov1 << endl;
            }

        }

        //outFile <<
        //cout <<
		//cout << "(CovNew, Cov): " << covNew << ", " << cov << endl;
	}

	//cout << "Coverage achieved: " << cov << endl;

	//outFile << cov << endl;

	outFile.close();

}


/**************** NEW ************************************/
float MC::mineSeedSetPlus() {
	cout << "In mineSeedSetPlus" <<endl;

	clear();
    int budget = strToInt(opt->getValue("budget"));
	
	totalCov = 0;
	Gains gainTable; // the five-column table we maintain (multimap<float, MGStruct*>) 
	
	MGStruct* pBestMG = NULL; // points to the current best node, initially nothing
	UID lastSeedID = -1;
	int countUsers = 0;
	
	// Do 1st pass without CELF.  Here only the top-1 will be added to Seed Set
	for (UserList::iterator i = users.begin(); i!=users.end(); ++i) {
		curSeedSet.clear(); // seed set is empty for now
		
		MGStruct* pMG = new MGStruct;
		pMG->nodeID = *i; // set "1st column"

		if (model == IC) {
            // IC model with CELF++
			bool isBest = false; // compute MG(u|empty)
			isBest = ICCovPlus(pMG, pBestMG);
			if (isBest == true) {
				pBestMG = pMG; 
			} 
		
        }  else if (model == LT) {
            // LT model with CELF++
            bool isBest = false;
            isBest = LTCovPlus(pMG, pBestMG, curSeedSet);
        
            if (isBest == true) {
                pBestMG = pMG;
            }
        } //endif
			
		pMG->flag = 1; // set "5th column" -- only one seed, so flag is only 1
		gainTable.insert(pair<float, MGStruct*>(pMG->gain, pMG));
		countUsers++;

		if (countUsers % 1000 == 0) {
			cout << "Number of users done in this iteration: " << countUsers << endl;
			// cout << "Time, cov, v : " << getTime() << ", " << pBestMG->gain << ", " << pBestMG->nodeID << endl;
			Gains::iterator iter = gainTable.end();
			iter--;
			MGStruct* pTop = (MGStruct *) (iter->second);
			if (pTop != NULL) {
				cout << "Time, cov, v :" << getTime() << ", " <<  pTop->gain << ", " << pTop->nodeID << endl;
			}
		}
		
	}
	
	// pick the 1st seed
	Gains::iterator iter = gainTable.end();
	iter--; 
	MGStruct* pTop = (MGStruct *) (iter->second);
	if (pTop == NULL) {
		cout << "Some problem: pTop is NULL pointer" << endl;		
	} else {
		lastSeedID = pTop->nodeID; // record the last selected seed
	
		curSeedSet.insert(pTop->nodeID); // the top-1 becomes a seed
		totalCov += pTop->gain; // update the current seeds' coverage
		
		writeInFile(pTop->nodeID, totalCov, totalCov, 0, 0, 0, countUsers);
		
		gainTable.erase(iter); // remove that seed from queue
	}
	
	
	// Optimized CELF, from 2nd pass onwards
	int totalUserNum = users.size();
	pBestMG = NULL;
	covBestNode.clear();
	MGStruct * pCurr = NULL;
	MGStruct * pNext = NULL;
	countUsers = 0; // # of users need recomputation!
	
	while (curSeedSet.size() < budget && totalCov < totalUserNum) {			

		Gains::iterator iter = gainTable.end();
		iter--;
		pCurr = (MGStruct *) (iter->second); // pointing to the current node
		
		if (pCurr == NULL) {
			cout << "Some problem: pCurr is NULL pointer" << endl;
		} 
		
		// cout << "Scanning node: " << pCurr->nodeID << " with MG = " << pCurr->gain << " ..... And MG_next = " << pCurr->gain_next << "....." << endl;
		
		// if it has been visited again, pick it as seed 
		if ( pCurr->flag == (curSeedSet.size() + 1) ) {	
			//cout << "Node " << pCurr->nodeID << " seen twice....." << endl;
			
			lastSeedID = pCurr->nodeID;
			curSeedSet.insert(pCurr->nodeID);
			totalCov += pCurr->gain;
			
			writeInFile(pCurr->nodeID, totalCov, pCurr->gain, 0, 0, 0, countUsers);
			// cout << "Seeds picked at No.1 place" << endl;
			
			gainTable.erase(iter);
			pBestMG = NULL;
			covBestNode.clear();
			countUsers = 0;
		
			continue;

		} else {  // it has NOT been vistied yet, in this iteration of seeking new seed
			pCurr->flag = curSeedSet.size() + 1;
			// countUsers++;

			iter--;
			pNext = (MGStruct *) (iter->second); // pointing to the next 		
		
			if (pCurr->v_best == lastSeedID && pCurr->gain_next >= pNext->gain) { 
			// pick it as seed	
					
				lastSeedID = pCurr->nodeID;
				
				curSeedSet.insert(pCurr->nodeID); // update the seed set
				totalCov += pCurr->gain_next; // update current seeds' coverage	

				writeInFile(pCurr->nodeID, totalCov, pCurr->gain_next, 0, 0, 0, countUsers);	
				//cout << "Seeds picked at No.2 place" << endl;

				iter++;
				gainTable.erase(iter); // delete new seed from queue

				pBestMG = NULL;
				covBestNode.clear();
				countUsers = 0;	
					
				continue;	

			} else {
			// recompute MG, or, just resort if 3rd column is the last picked seed

				// IF v_best is still the last seed, just move the previouly 4th column value to the 2nd column
				// To avoid re-computation
				if (pCurr->v_best == lastSeedID) {
					
					pCurr->gain = pCurr->gain_next;
					pCurr->v_best = 0;
					pCurr->gain_next = 0;
					pCurr->flag = curSeedSet.size() + 1;	
					
					
					if (pBestMG == NULL) {
						pBestMG = pCurr;
					} else {
						if (pCurr->gain > pBestMG->gain) {
							pBestMG = pCurr;
						}
					}							

					iter++;
					gainTable.erase(iter);
					gainTable.insert(pair<float, MGStruct *>(pCurr->gain, pCurr));
				
				} else {
					bool isBest; // compute Cov(S+u) and Cov(S+u+prevBest)	
                    if (model == IC) {
						isBest = ICCovPlus(pCurr, pBestMG);                    
                    } else if (model == LT) {
                        isBest = LTCovPlus(pCurr, pBestMG, curSeedSet);
                    } // endif
                    
                    pCurr->flag = curSeedSet.size() + 1;
                    countUsers++;

                    if(pCurr->gain >= pNext->gain) { // pick it as seed
                        lastSeedID = pCurr->nodeID; // record it to be the last added seed

                        curSeedSet.insert(pCurr->nodeID); // add it to seed set
                        totalCov += pCurr->gain; // update current seeds' coverage

                        writeInFile(pCurr->nodeID, totalCov, pCurr->gain, 0, 0, 0, countUsers);
                        // cout << "Seeds picked at No.4 place" << endl;
                        iter++;
                        gainTable.erase(iter); // remove new seed from queue

                        pBestMG = NULL;
                        covBestNode.clear();
                        countUsers = 0;

                        continue;
                    } else { 
                        // cannot be seed for now, re-insert it into table (at another position)	
                        if (isBest == true) {
                            pBestMG = pCurr;
                        }
                        iter++;
                        gainTable.erase(iter);
                        gainTable.insert(pair<float, MGStruct *>(pCurr->gain, pCurr));

                        continue;
                    } //endif
					
                }//endif									 
			} //endif

		}//endif
		
	} //endwhile
	
	return totalCov;

}


float MC::mineSeedSet(int t_ub) {
	cout << "In mineSeedSet" << endl;
	clear();
	int budget = strToInt(opt->getValue("budget")); 

	multimap<float, UID> covQueue; // needed to implement CELF
	totalCov = 0;

	int countUsers = 0 ;

	// first pass, without CELF
	for (UserList::iterator i = users.begin(); i!=users.end(); ++i) {
		UID v = *i;
        UserList S;
		S.insert(v);

		float newCov = 0; 
		
		if (model == IC) {
			newCov = ICCov(S);
		} else if (model == LT) {
			newCov = LTCov(S);
		} 

		covQueue.insert(pair<float, UID>(newCov, v));

		countUsers++;

		if (countUsers % 100 == 0) {
			cout << "Number of users done in this iteration: " << countUsers << endl;
			cout << "Time, cov, v : " << getTime() << ", " << totalCov << ", " << *curSeedSet.begin() << endl;
		}

		if (totalCov < newCov) {
			totalCov = newCov;
			curSeedSet.clear();
			curSeedSet.insert(v);
		} 
		

	} 

	if (curSeedSet.size() != 1) {
		cout << "Some problem" << endl ;
	}

	float actualCov = totalCov;

	float prevActualCov = actualCov;

	writeInFile(*curSeedSet.begin(), totalCov, totalCov, t_ub, actualCov, actualCov, countUsers);
	cout << "Number of nodes examined in this iteration: " << countUsers << endl;
	
	// remove the last element from covQueue
	// we have already picked the best node
	multimap<float, UID>::iterator i = covQueue.end();
	i--;
	covQueue.erase(i);

	// CELF
	countUsers = 0;
	UserList usersExamined; 

//	while (totalCov < eta) {
	while (curSeedSet.size() < budget) {
		bool flag = false;

		multimap<float, UID>::iterator i = covQueue.end();
		i--;

		UID v = i->second;

		curSeedSet.insert(v);
		float newCov = 0;

		if (usersExamined.find(v) == usersExamined.end()) {
			if (model == IC) {
				newCov = ICCov(curSeedSet);
			} else if (model == LT) {
				newCov = LTCov(curSeedSet);
			}

			usersExamined.insert(v);
			countUsers++;

		} else {
			newCov = i->first + totalCov;
			flag = true;
		}

		
		i--;
		float oldCov_nextElement = i->first;
		UID nextElement = i->second;

		if (flag || newCov - totalCov >= oldCov_nextElement) {
			// pick v in the seed set
			writeInFile(v, newCov, newCov - totalCov, t_ub, actualCov, actualCov - prevActualCov, countUsers);
			cout << "Number of nodes examined in this iteration: " << usersExamined.size() << endl;
			
			prevActualCov = actualCov;

			totalCov = newCov;
			// reset parameters
			countUsers = 1;
			usersExamined.clear();

			i++;
			covQueue.erase(i);
			continue;
		} 

		// move the element to another place
		i++;
		covQueue.erase(i);
		covQueue.insert(pair<float, UID>(newCov - totalCov, v));
		
		// its not in seed set
		curSeedSet.erase(v);
	}

	return totalCov;
}


/* pMG: the MG structue of node curV.  curV's ID is included */
bool MC::ICCovPlus(MGStruct* pMG, MGStruct* pBestMG) {

	startIt = strToInt(opt->getValue("startIt"));
	
	bool ret = false; // indicates if the current node is better than the previous best
	
	float cov = 0; // compute cov(S + currentNode)
	float covX = 0; // compute cov(S + currentNode + prevBestNode)
	
	int seedSetSize = (int) curSeedSet.size();
	
	// for each MC run:
	for (int b = 0; b < countIterations; b++) {
		queue<UID> Q;
		UserList activeNodes;
		
		// initialize Q and activeNodes using the Seed Set
		for(UserList::iterator i = curSeedSet.begin(); i != curSeedSet.end(); i++) {
			UID v = *i;
			Q.push(v);
			activeNodes.insert(v);
		}
		Q.push(pMG->nodeID); // also put the node being examined into queue
		activeNodes.insert(pMG->nodeID);
		
		while (Q.empty() == false) {
			UID v = Q.front();
			
			FriendsMap* neighbors = AM->find(v);
			if (neighbors != NULL) {
				for (FriendsMap::iterator j = neighbors->begin(); j != neighbors->end(); ++j) {
					UID u = j->first;
				
					if (activeNodes.find(u) == activeNodes.end()) {
						float toss = ((float)(rand() % 1001))/(float)1000;
						float p = j->second;

						if (p >= toss) {
							activeNodes.insert(u);
							Q.push(u);
						}
					}			
				}
			}
			
			Q.pop();
		}
		
		cov += (float)activeNodes.size()/countIterations;		

		// if x has not been activated, and this iteration allows computing 4th column
		if(pBestMG != NULL && seedSetSize >= (startIt - 1)  && activeNodes.find(pBestMG->nodeID) == activeNodes.end()) {
	
			Q.push(pBestMG->nodeID); // Q starts with x only
			activeNodes.insert(pBestMG->nodeID);			

			while (Q.empty() == false) {
				UID v = Q.front();

				FriendsMap* neighbors = AM->find(v);
				if (neighbors != NULL) {

					// for each inactive neighbour of v
					for (FriendsMap::iterator j = neighbors->begin(); j != neighbors->end(); ++j) {
						UID u = j->first;

						if (activeNodes.find(u) == activeNodes.end()) {
							float toss = ((float)(rand() % 1001))/(float)1000;
							float p = j->second;

							if (p >= toss) {
								activeNodes.insert(u);
								Q.push(u);
							}
						}			
					}
				}

				Q.pop();
			}
		
		} 
		
		covX += (float)activeNodes.size()/countIterations;

	} //endfor -- each MC run
	
	// After entire MC process
//	cov = cov / countIterations;
//	covX = covX / countIterations;
	
	pMG->gain = cov - totalCov; // set "2nd column", MG(u|S)...
	
	if (pBestMG == NULL) {
		// for first run of each iteration, Best Node is not specified
		pMG->v_best = pMG->nodeID;
		pMG->gain_next = 0;
	
		if (seedSetSize >= (startIt - 1)) {
			return true;
		} else {  
			return false;  // when it's not allowed to compute 4th column
		}
		
	} else { // if Best Node has been specified

		if (pMG->gain > pBestMG->gain) { 
			// this node is better than the previous best
			pMG->v_best = pMG->nodeID; // set "3rd column" to be itself
			pMG->gain_next = 0; // set "4th column" to be zero
			ret = true;
		} else { 
			// this node is NO better than the previous best
			pMG->v_best = pBestMG->nodeID;
			pMG->gain_next = covX - totalCov - pBestMG->gain; // MG(u|S+x)
			ret = false;
		}
	}
	
	return ret;
}


float MC::ICCov(UserList& S) {
	float cov = 0;
				
	for (int b = 0; b < countIterations; ++b) {
		// Q is the queue in the depth/breadth first search
		queue<UID> Q;
		// activeNodes is the set of nodes that are activated in the current
		// run of Monte Carlo simulation
		UserList activeNodes;

		// S is the seed set
		// for each seed node v is S, 
		// add it to activeNodes
		// add it to Q as well
		for (UserList::iterator i=S.begin(); i!=S.end(); ++i) {
			UID v = *i;
			Q.push(v);
			activeNodes.insert(v);
		}

		while(Q.empty() == false) {
			UID v = Q.front(); 
			
			// AM is adjacency matrix
			FriendsMap* neighbors = AM->find(v);
			if (neighbors != NULL) {
				for (FriendsMap::iterator j = neighbors->begin(); j!=neighbors->end(); ++j) {
					UID u = j->first;

					if (activeNodes.find(u) == activeNodes.end()) {
						float toss = ((float)(rand() % 1001))/(float)1000;
						float p = j->second;

						if (p >= toss) {
							activeNodes.insert(u);
							Q.push(u);
						}
					}

				}
			}
			
			Q.pop(); 

		}

		cov += (float)activeNodes.size()/countIterations;

	}

	// compute two things: cov(S) and cov(S+x)
//	cov = cov/countIterations;
	return cov;
	

}


void MC::clear() {
	covBestNode.clear();
	curSeedSet.clear();
	seedSetNeighbors.clear();
	totalCov = 0;
}


/* pMG: the MG structue of node curV.  curV's ID is included */
bool MC::LTCovPlus(MGStruct *pMG, MGStruct *pBestMG, UserList &S) {
// S is the current seed set, without the current candidate node or its previous best node

    startIt = strToInt(opt->getValue("startIt"));
    int seedSetSize = S.size();

    UID x = pMG->nodeID; // current candidate seed
    S.insert(x); // temporarily insert x into seed set (should remove it later in this function)

    bool ret = false; // indicates if the current node is better than the previous best
    double cov = 0; // compute cov(S + currentNode)
	double  covPlus = 0; // compute cov(S + currentNode + prevBestNode)
    float tol = 0.00001;

    for (int b = 0; b < countIterations; ++b) {
        
        queue<UID> T; // T is the set of nodes that are to be processed
        // Q is the set of nodes that have been seen until now
        // Thus, Q is a superset of T
        // Q contains the nodes that are processed and to be processed
        map<UID, NodeParams> Q;

//        cov += S.size(); // including S+x themselves
//        covPlus += S.size();
       	double cov1 = S.size(); // cov1 == coverage in one run .. that is, number of nodes reachable
       	double covPlus1 = S.size(); // cov1 == coverage in one run .. that is, number of nodes reachable
        
        // add neighorhood of S (current seeds, plus x) to queue
        for (UserList::iterator i = S.begin(); i != S.end(); ++i) {
            UID v = *i;
            
            FriendsMap *neighbors = AM->find(v);
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


                            /* generate secret number: */
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
        

        bool is_best_covered = false;

        while ( !T.empty()) {
            UID u = T.front();
//            cout << "T.size " << T.size() << endl;
            NodeParams& np = Q.find(u)->second;
            if (np.active == false && np.inWeight >= np.threshold + tol) {
                np.active = true;

				if (pBestMG != NULL && u == pBestMG->nodeID) {
					is_best_covered = true;
				}

                cov1++;
                covPlus1++;

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
                                } //endif
                            } //endfor
                        } //endif
                    } //endfor
                } //endif
            } ///////////////////////////////////////////// 
            // deletes the first element
            T.pop();

        } //endwhile

        // CELF++
        // if prev_best is not covered yet..
        if(pBestMG != NULL && seedSetSize >= (startIt-1)  && is_best_covered == false) {
            UID bestID = pBestMG->nodeID;
            covPlus1++;

            FriendsMap *neighbors = AM->find(bestID);
            if (neighbors != NULL) {
                // add all neighbors of bestNode to T and Q.
                for (FriendsMap::iterator j = neighbors->begin(); j != neighbors->end(); ++j) {
                    UID u = j->first;
                    if (S.find(u) != S.end()) continue;

                    if (Q.find(u) == Q.end()) {
                        // if the node u has not been seen before
                        // create a new NodeParams
                        // create a random threhsold
                        NodeParams& np = Q[u];
                        np.active = false;
                        np.inWeight = j->second;

                        /* generate secret number: */
                        np.threshold = ((float)(rand() % 1001))/(float)1000;
                        T.push(u);
                    } else {
                        NodeParams& np = Q[u];
                        if (np.active == false) {
                            np.inWeight += j->second;
                            T.push(u);
                        }//endif
                    }//endif 
                } //endfor

            } //endif
        
            // propagate through bestNode's neighborhood
             
			while ( !T.empty()) {
				UID u = T.front();
				NodeParams& np = Q.find(u)->second;
				if (np.active == false && np.inWeight >= np.threshold + tol) {
					np.active = true;

					covPlus1++;

					// add u's neighbors to T
					FriendsMap* neighbors = AM->find(u);
					if (neighbors != NULL) {
						// for each neighbor w of u
						for (FriendsMap::iterator k = neighbors->begin(); k!=neighbors->end(); ++k) {
							UID w = k->first;
							// is w is in S, no need to do anything
							if (w == bestID || S.find(w) != S.end()) continue;

							// if w is not in S, locate it in Q
							map<UID, NodeParams>::iterator it = Q.find(w);

							if (it == Q.end()) {
								// if it is not in Q, then
								NodeParams& np_w = Q[w];
								np_w.threshold = ((float)(rand() % 1001))/(float)1000;
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
										cout << "Something 2 wrong, the inweight for a node is > 1. (w, inweight) = " << w << ", " << np_w.inWeight - 1<< endl;
									} //endif
								} //endif
							} //endif
						} //endfor
					} //endif
				} //endif
				// deletes the first element
				T.pop();

			} //endwhile
        } //endif
   
		cov +=(double) 1.0*cov1/countIterations;
		covPlus += (double) 1.0*covPlus1/countIterations;

    } //endfor -- each MC run

    // After the whole MC process
	S.erase(x); // remove x from current seed set now

    //cout << "LTCovPlus debug: cov = " << cov << endl; 
    pMG->gain = cov - totalCov;

    if (pBestMG == NULL) {
        // for first run of each iteration, Best Node is not specified
        pMG->v_best = pMG->nodeID;
        pMG->gain_next = 0;

        if (seedSetSize >= (startIt-1)) {
            return true;
        } else {
            return false;
        } //endif

    } else {
        // if Best Node has been specified before
        if (pMG->gain > pBestMG->gain) {
            // this node is better than its prev. best
            pMG->v_best = pMG->nodeID;
            pMG->gain_next = 0;
            ret = true;
        } else {
            // this node is NO better than its prev. best
            pMG->v_best = pBestMG->nodeID;
            pMG->gain_next = covPlus - totalCov - pBestMG->gain; // MG(u|S+x)
            ret = false;
        } //endif

    } //endif

    return ret;

} // end of function


float MC::LTCov(UserList& S) {
//    cout << "In LTCov" << endl;
    float tol = 0.00001;
    float cov = 0;

//    map<UID, float> ppIn; // what is the prob with which the node is covered
   
    for (int b = 0; b < countIterations; ++b) {
        /* initialize random seed: */

        float cov1 = 0;
        // T is the set of nodes that are to be processed
        queue<UID> T;
        // Q is the set of nodes that have been seen until now
        // Thus, Q is a superset of T
        // Q contains the nodes that are processed and to be processed
        map<UID, NodeParams> Q;

//        cov += S.size();
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

                            /* generate secret number: */
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
//                cov++;
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
		cov += cov1/countIterations;
    }

	/*
    // coverage from ppIn
    float cov1 = 0;


    for (map<UID, float>::iterator i = ppIn.begin(); i!=ppIn.end(); ++i) {
        cov1 += i->second;
        cout << "ppIN: " << i->first << " " << i->second/countIterations << endl;
    }
    
    cov1 = cov1/countIterations;
	*/
//    double retCov = (double) cov/countIterations;

//    cout << "(cov, retCov, cov1) = " << cov << ", " << retCov << ", " << cov1 << endl;

    return cov;
   

}


void MC::printVector(vector<UID>& vec, float pp) {
	for (vector<UID>::iterator i=vec.begin(); i!=vec.end(); ++i) {
		cout << *i << " ";
	}

	cout << endl;
}


void MC::writeInFile(UID v, float cov, float marginal_gain, int curTimeStep, float actualCov, float actualMG, int countUsers) {
	
	outFile << v << " " << cov << " " << marginal_gain << " " << curTimeStep << " " << getCurrentMemoryUsage() << " " << getTime() <<  " " << getTime_cur() << " " << actualCov << " " << actualMG << " " << countUsers << endl;
	
	seedFile<<v<<endl;
	int size = curSeedSet.size();
	if(size==1 || size==2 || size==5||size==10||(size%25==0&& size>10)){
		statFile<<size<<" "<<cov<<" "<<getTime()<<" "<<getCurrentMemoryUsage()<<endl;
	}
	//cout << v << " " << cov << " " << marginal_gain << " " << curTimeStep << " " << getCurrentMemoryUsage() << " " << getTime() << " " << getTime_cur() <<  " " << actualCov << " " << actualMG << " " << countUsers << endl;
	/*cout << endl << endl << "Picked a seed node: " << v << ", total: " << curSeedSet.size() << endl;
	outFile << v << " " << cov << " " << marginal_gain << " " << curTimeStep << " " << getCurrentMemoryUsage() << " " << getTime() <<  " " << getTime_cur() << " " << actualCov << " " << actualMG << " " << countUsers << endl;
	cout << v << " " << cov << " " << marginal_gain << " " << curTimeStep << " " << getCurrentMemoryUsage() << " " << getTime() << " " << getTime_cur() <<  " " << actualCov << " " << actualMG << " " << countUsers << endl;
	cout << endl << endl;
*/

}


void MC::writeCovInFile(vector<UID> &seedVec, float tol) {

	cout << "In writeCovInFile. Number of mcruns: " << countIterations << endl;

	string filename = opt->getValue("outdir") + string("/LTCov_") + m + "_" + floatToStr(tol) + ".txt" ;

/*	if (model == LTNew) { 
		filename = outdir + "/" + m + "_MC_" + opt->getValue("topl") + "_" + opt->getValue("cutoff") + ".txt";
	} else if (strToInt(opt->getValue("phase")) == 15) { // LDAG
		filename = outdir + "/" + m + "_MC_" + opt->getValue("tol_ldag") + ".txt";
	} else if (strToInt(opt->getValue("phase")) == 20) { // Baseline
        filename = outdir + "/" + m + "_MC" + ".txt";  
    }

	ofstream outFile (filename.c_str(), ios::out);
*/	
	UserList S;
	for(vector<UID>::iterator i = seedVec.begin(); i != seedVec.end(); ++i) {
		UID v = *i; 
		S.insert(v);

		//if (S.size() % 5 != 0) continue;
		if (S.size() != 50) continue;
		
        float cov = LTCov(S);
		outFile << S.size() << " " << v << " " << cov << endl;
//		cout << S.size() << " " << v << " " << cov << endl;
		
	}

//	outFile.close();

	cout << "Done writeCovInFile" << endl;
}

void MC::openOutputFiles() {

	if (outFile.is_open()) {
		outFile.close(); 
	}
	if (seedFile.is_open()){
                seedFile.close();
        }
        if (statFile.is_open()){
                statFile.close();
        }
	string algorithm = "Greedy";
	cout << "problem : " << problem << endl;
	string filename = outdir + "/output_" + m + "_" + algorithm + ".txt";
	string statfilename = outdir + "/" + "stats_" + m + "_" + to_string(countIterations) + ".txt";
        string seedfilename = outdir + "/" + "seeds_" + m + "_" + to_string(countIterations) + ".txt";
	cout<<statfilename<<endl;
	 statFile.open (statfilename.c_str());
        seedFile.open (seedfilename.c_str());	
	outFile.open (filename.c_str());

	if (outFile.is_open() == false) {
		cout << "Can't open file " << filename << " for writing" << endl;
		exit(1);
	}
	if (statFile.is_open() == false) {
                cout << "Can't open file " << statfilename << " for writing" << endl;
                exit(1);
        }
        if (seedFile.is_open() == false) {
                cout << "Can't open file " << seedfilename << " for writing" << endl;
                exit(1);
        }
}

PropModels MC::getModel() {
	return model;
}

float MC::getTime_cur() const {
	time_t curTime;
	time(&curTime);

	float min = ((float)(curTime - stime_mintime))/60;
	return min;
}

float MC::getTime() const {
	time_t curTime;
	time(&curTime);

	float min = ((float)(curTime - startTime))/60;
	return min;
}

void MC::readInputData(float alpha) {
	cout << "in readInputData for model " << model << " with alpha " << alpha << endl;

	unsigned int edges = -1;

	string probGraphFile = opt->getValue("probGraphFile");
	cout << "Reading file " << probGraphFile << endl;
	ifstream myfile (probGraphFile.c_str(), ios::in);
	string delim = " \t";	
	if (myfile.is_open()) {
		while (! myfile.eof() )	{
			std::string line;
			getline (myfile,line);
			if (line.empty()) continue;
			edges++;
			//if (edges == 0) continue; // ignore the first line
			std::string::size_type pos = line.find_first_of(delim);
			int	prevpos = 0;

			// get first user
			string str = line.substr(prevpos, pos-prevpos);
			UID u1 = strToInt(str);

			// get the second user
			prevpos = line.find_first_not_of(delim, pos);
			pos = line.find_first_of(delim, prevpos);
			UID u2 = strToInt(line.substr(prevpos, pos-prevpos));
		
			if (u1 == u2) continue;

			// get the parameter
			float parameter1 = 0;
			prevpos = line.find_first_not_of(delim, pos);
			pos = line.find_first_of(delim, prevpos);
			if (pos == string::npos) 
				parameter1 = strToFloat(line.substr(prevpos));
			else
				parameter1 = strToFloat(line.substr(prevpos, pos-prevpos));

			if (parameter1 == 0) continue;

			parameter1 = parameter1 ;
			users.insert(u1);
			users.insert(u2);


			if (edges % 1000000 == 0) {
				cout << "(node1, node2, weight,  AM size till now, edges till now, mem) = " << u1 << ", " << u2 << ", " << parameter1 << ", " << AM->size() << ", " << edges << ", " << getCurrentMemoryUsage() << endl;
			}


			FriendsMap* neighbors = AM->find(u1);
			if (neighbors == NULL) {
				neighbors = new FriendsMap();
				neighbors->insert(pair<UID, float>(u2, parameter1));
				AM->insert(u1, neighbors);
			} else {
				//FriendsMap::iterator it = neighbors->find(u2);
				//if (it == neighbors->end()) {
					neighbors->insert(pair<UID, float>(u2, parameter1));
				//} else {
				//	cout << "WARNING: Edge redundant between users " << u1 << " and " << u2 << endl;
				//}
			}

            if (revAM != NULL) {
                multimap<float, UID> *revNeighbors = revAM->find(u1);
                if (revNeighbors == NULL) {
                    revNeighbors = new multimap<float, UID>();
                    revNeighbors->insert(std::make_pair(parameter1, u2));
                    revAM->insert(u1, revNeighbors);
                } else {
                    revNeighbors->insert(std::make_pair(parameter1, u2));
                }
            }
            /*
            if (AM_in != NULL) {
                FriendsMap *inNeighbors = AM_in->find(u2);
                if (inNeighbors == NULL) {
                    inNeighbors = new FriendsMap();
                    inNeighbors->insert(std::make_pair(u1, parameter1));
                    AM_in->insert(u2, inNeighbors);
                } else {
                    FriendsMap::iterator it = inNeighbors->find(u1);
                    if (it == inNeighbors->end()) {
                        inNeighbors->insert(std::make_pair(u1, parameter1));
                    }
                }
            }*/

            // also add the edges u2->u1 but done allocate Edge class to them
			// .. it is just to find friends efficiently
			if (graphType == UNDIRECTED) { 
				neighbors = AM->find(u2);
				if (neighbors == NULL) {
					neighbors = new FriendsMap();
					neighbors->insert(pair<UID, float>(u1, parameter1 ));
					AM->insert(u2, neighbors);
				} else {
					FriendsMap::iterator it = neighbors->find(u1);
					if (it == neighbors->end()) {
						neighbors->insert(pair<UID, float>(u1, parameter1 ));
					} else {
						//	cout << "WARNING: Edge redundant between users " << u1 << " and " << u2 << endl;
					}
				}
			}
		}
		myfile.close();
	} else {
		cout << "Can't open friendship graph file " << probGraphFile << endl;
	}

    this->numEdges = edges;

    cout << "BuildAdjacencyMatFromFile done" << endl;
	cout << "Size of friendship graph hashtree is : " << AM->size() << endl;
    //cout << "Size of NEW friendship graph hashtree is : " << revAM->size() << endl;
	cout << "Number of users are: " << users.size() << endl;
	cout << "Number of edges in the friendship graph are: " << edges << endl;
}
			


}
