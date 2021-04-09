#include "LTSimplePath.h"
#include <iomanip>

namespace _LTSimplePath {

LTSimplePath::LTSimplePath(AnyOption *opt1) : freeTuples(NULL), freeTuplesSize(0) {
    opt = opt1;

    outdir = opt->getValue("outdir");
	probGraphFile = opt->getValue("probGraphFile");
//	tol = strToFloat(opt->getValue("tol_ldag"));
	graphType = DIRECTED;

    userNodes = new map<UID, xNode *>();
    //adjMap = new map<UID, set<UID> *>();
    outsideCovMap.clear();

    //path_tol = strToDouble(opt->getValue("path_tol"));
    cutoff_tol = strToDouble(opt->getValue("cutoff"));
    countIterations = strToInt(opt->getValue("mcruns"));
    setModel(opt);

    topl = strToInt(opt->getValue("topl"));
    celfPlus = strToInt(opt->getValue("celfPlus"));

    //srand(time(NULL));

    optimal = 1;
    //optimal = 0;

}


LTSimplePath::~LTSimplePath() {
    cout << "Total time taken : " << getTime() << endl;
    for (map<UID, xNode *>::iterator i = userNodes->begin(); i != userNodes->end(); ++i) {
        xNode *p = i->second;
        delete p->N_out;
        delete p->N_in;
        delete p;
    }
    delete userNodes;
}


void LTSimplePath::setModel(AnyOption *opt) {
    m = opt->getValue("propModel");
    model = LTNew; // just by default
    
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


void LTSimplePath::doAll() {

    setprecision(8);
    init_linked_list(); // allocate memory space for storing nodes
    readInputData(); // read graph date from a file

    //time(&startTime);
    clock_t startTime = clock();

    if (optimal == 1) {
        vertexCoverMaxDeg();
    }    
    
    int phase = strToInt(opt->getValue("phase"));

    openOutputFiles();

    if (phase == 17) {
        problem = "SimPath";
        cout << "model is : " << model << ". problem is : " << problem << endl;
        
        seedVec.clear(); 
       
        if (celfPlus == 1) {
            cout << "CELF++" << endl;
            mineSeedSetPlus(); // CELF++
        } else {
            cout << "CELF" << endl;
            mineSeedSet(); // CELF
        }
    } 

    outFile.close();
	hopsOutFile.close();

    // true: do MC to evaluate coverage; false: will not do.
    bool flag = true;
    if (optimal == 1 && flag == true) {
        MC* mc = new MC(opt);    
        mc->readInputData();    
        mc->writeCovInFile(seedVec, 0);
    }
}

// mineSeedSet, SimPath + VC + CELF + Look-Ahead Optimization
double LTSimplePath::mineSeedSet() {

    cout << "In mineSeedSet()..." << endl;

    curSeedSet.clear();
    int budget = strToInt(opt->getValue("budget"));
    totalCov = 0;
    int countUsers = 0;

    multimap<double, UID> covQueue;

    if (optimal == 0) {
        cout << "Optimizations are NOT used..." << endl;
        outsideNodes.clear();
        outsideCovMap.clear();
        vertexCover = users; 
    }

    // initialize coverage of nodes outside VC to be zero
    for (UserList::iterator i = outsideNodes.begin(); i != outsideNodes.end(); ++i) {
        outsideCovMap.insert(std::make_pair(*i, 0.0)); 
    }
  
    // first pass, without CELF, computing simple-path-based coverage for nodes in vertex cover
    // for each Vertex Cover node, compute its coverage by enumerating simple paths 
    for (UserList::iterator i = vertexCover.begin(); i != vertexCover.end(); ++i) {
        UID v = *i;

        double newCov = 0;
        UserList S;
        S.insert(v);
        UserList T;
        
        if (model == LTNew) {
            newCov = simPath(S, T, 1); // iteration is 1
        }//endif

        covQueue.insert(std::make_pair(newCov, v));
        ++countUsers;

        if (newCov > totalCov) {
            totalCov = newCov;
            curSeedSet.clear();
            curSeedSet.insert(v);
        }

    } //endfor

    // for each Non Vertex Cover node...
    for (map<UID, double>::iterator i = outsideCovMap.begin(); i != outsideCovMap.end(); ++i) {
        UID v = i->first;
        double newCov = 1 + i->second; // don't forget to add 1 to vertex cover based coverage: very important

        covQueue.insert(std::make_pair(newCov, v));
        ++countUsers;

        if (newCov > totalCov) {
            totalCov = newCov;
            curSeedSet.clear();
            curSeedSet.insert(v);
        }
    }
    
    // $$$$$$$ END OF 1ST ITERATION $$$$$$$$//
    cout << "1st iteration done... time: " << getTime() << endl;

    if (curSeedSet.size() != 1) {
        cout << "Some problem: first pass: seed set size != 1... it is " << curSeedSet.size() << endl;
    }

    seedVec.push_back(*curSeedSet.begin());
    writeInFile(*curSeedSet.begin(), totalCov, totalCov, 0, 0, 0, countUsers);
    cout << "Number of nodes examined in this iteration: " << countUsers << endl << endl;

    multimap<double, UID>::iterator i = covQueue.end();
    i--;
    covQueue.erase(i);

    // first seed picked, now subsequent iteration starts! CELF / CELF++
    countUsers = 0;
    UserList usersExamined;
    int l = topl;  // top-l elements from CELF queue

    if (covQueue.size() < l) {
        l = covQueue.size() - 1;
    }

    // repeated evaluate MG and pick seed until budget is exhausted
    while (curSeedSet.size() < budget) {
        bool flag = false;
        UserList T;
       
        if (covQueue.empty()) {
            cout << "CELF already empty, no more seed to pick" << endl;
            break;
        } //endif

        multimap<double, UID>::iterator iter = covQueue.end();
        iter--; // top position at CELF queue

        UID node_to_compare = 0;
        double MG_to_compare = 0;
        bool examined_reached = false;
        for (int i = 0; i < l; i++) {
            UID x = iter->second;

            if (usersExamined.find(x) == usersExamined.end()) {
                // if this node has not been exmained yet, it should be a candidate
                T.insert(x);
                usersExamined.insert(x);
                
                //cout << "pick a candidate node to T: " << x << endl;

                countUsers++;
            } else {
                // reach the first examined node, stop adding nodes to T
                examined_reached = true;
                node_to_compare = x;
                MG_to_compare = iter->first;
    
                if (i == 0) {
                    flag = true; // if the top-1 node has been already examined
                } //endif

                break;
            } //endif

            iter--; // move to the next position
        } //endfor

        // if the top-1 node is already examined, then it's automatically seed!
        if (flag == true) {
            totalCov += MG_to_compare;
            curSeedSet.insert(node_to_compare);
            seedVec.push_back(node_to_compare);

            writeInFile(node_to_compare, totalCov, MG_to_compare, 0, 0, 0, countUsers);
            countUsers = 0;
            usersExamined.clear();

            if (iter->second != node_to_compare) {
                cout << "Some trouble in CELF...." << endl;
                exit(0);
            }

            covQueue.erase(iter);
            continue;
        }

        // if all top-l have not examined before, then we need to compare with the l+1 guy
        if (examined_reached == false) {
            iter--;
            node_to_compare = iter->second;
            MG_to_compare = iter->first;
        }

        if (node_to_compare == 0 || MG_to_compare == 0) {
            cout << "Some trouble 2 in CELF...." << endl;
        }

        // delete all candidate from covQueue now
        for (int i = 0; i < T.size(); i++) {
             multimap<double, UID>::iterator iter = covQueue.end();
             iter--;
             covQueue.erase(iter);
        }

        // compute cov(S) and cov(S) on V-x for all x \in T
        covSMap.clear();
        covXMap.clear();
        double cov_S = simPath(curSeedSet, T, curSeedSet.size() + 1);
        // covSMap should be already updated within simPath;
        
        // compute cov(x) on V-S for all x in T
        UID winnerID = 0;
        double winnerMG = 0;

        for (UserList::iterator i = T.begin(); i != T.end(); ++i) {
            UID x = *i;
            double cov_X = backtrackBasic(x, curSeedSet);

            //cout << "candidate id, cov(x) = " << x << ", " << cov_X << endl;

            //covXMap.insert(std::make_pair(x, cov_X));

            // compute node x's marginal gain;
            double xMG = covSMap.find(x)->second + cov_X - totalCov;
            covXMap.insert(std::make_pair(x, xMG)); // pair(ID, newMG)
            
            // find the node with max new MG among all top-l candidates
            if (xMG > winnerMG) {
                winnerMG = xMG;
                winnerID = x;
            } 
        } //endfor

        // Examine whether we can claim the next seed node or not
        if (winnerMG >= MG_to_compare) {
            // if one of top-l is larger than the next node is seed
            totalCov += winnerMG;
            curSeedSet.insert(winnerID);
            seedVec.push_back(winnerID);

            writeInFile(winnerID, totalCov, winnerMG, 0, 0, 0, countUsers);
            countUsers = 0;
            usersExamined.clear();

            // re-insert other losers into the queue
            for (UserList::iterator k = T.begin(); k != T.end(); ++k) {
                if (*k != winnerID) {
                    UID loserID = *k;
                    double loserMG = covXMap.find(loserID)->second;
                    covQueue.insert(std::make_pair(loserMG, loserID));
                } //endif
            } //endfor
    
            continue;
             
        } else {
            // otherwise, re-insert all losers into the queue
            for (UserList::iterator k = T.begin(); k != T.end(); ++k) {
                UID loserID = *k;
                double loserMG = covXMap.find(loserID)->second;
                covQueue.insert(std::make_pair(loserMG, loserID));
            } //endfor

        } //endif
    
    } //endwhile


    return totalCov;
}


// an interface function to call backtrack algo
double LTSimplePath::simPath(UserList &S, UserList &T, int iteration) {

    //cout << "enter simPath" << endl;

    double cov = 0.0;
    
    if (iteration == 1) {
        // first iteration, only one node in set S
        UID u = *S.begin();
        T.clear();

        xNode *startNode = NULL; // xNode struct ptr for v
        startNode = userNodes->find(u)->second; // look up v in the global node map

        multimap<double, xNode *> *inNeighbors = startNode->N_in; // get u's in-neighbors x
        map<UID, double> probMap; // map<x, b(x,u)>
        for (multimap<double, xNode *>::iterator j = inNeighbors->begin(); j != inNeighbors->end(); ++j) {
            UID x = j->second->id;
            probMap.insert(std::make_pair(x, j->first));
            
            if (outsideNodes.find(x) != outsideNodes.end()) {
                T.insert(x);  // if an in-neighbor of u is not in VC, add it into T
            } //endif
        } //endfor

        // S-current seed set, T-the nodes we want partial coverage
        cov = backtrackFirst(startNode, T, probMap);     

    } else if (iteration > 1) {
        // subsequent iterations...
        // mineSeedSet() will give seed candidates T from CELF queue. \
        // S is the curSeedSet
        // then for each x \in T, compute cov(S) on V-x and cov(x) on V-S
        //map<UID, double> covSMap; // store cov(S) on V-x for each x
        
        // initialize all coverages to be zero
        for (UserList::iterator k = T.begin(); k != T.begin(); ++k) {
            covSMap.insert(std::make_pair(*k, 0.0));
        } //endfor
        
        // compute coverage of each seed node and accumulate them...
        // within backtrackSubSequent, covSMap is updated 
        for (UserList::iterator i = S.begin(); i != S.end(); ++i) {
            UID u = *i; // one seed node
            xNode *startNode = userNodes->find(u)->second;
            cov +=  backtrackSubsequent(startNode, curSeedSet, T);
        } //endfor

        //cout << "Coverage of S = " << cov<< endl;
    
    } else {
        cerr << "Something is wrong, iteration number should be >= 1" << endl;
    } //endif

    return cov;

}


// mineSeedSetPlus: using CELF++
double LTSimplePath::mineSeedSetPlus() {

    cout << "In mineSeedSetPlus" <<endl;

    curSeedSet.clear();
    int budget = strToInt(opt->getValue("budget"));
    
    totalCov = 0;
    int countUsers = 0;

    Gains gainTable;
    MGStruct *pBestMG = NULL;
    UID lastSeedID = -1;
    double lastSeedMG = 0;
    
    // initialize coverage of nodes outside VC to be zero
    for (UserList::iterator i = outsideNodes.begin(); i != outsideNodes.end(); ++i) {
        outsideCovMap.insert(std::make_pair(*i, 0.0)); 
    }

    // first pass, without CELF, computing simple path based coverage for nodes in vertex cover
    // for each Vertex Cover node, compute its coverage by enumerating simple paths 
    for (UserList::iterator i = vertexCover.begin(); i != vertexCover.end(); ++i) {
        //UID v = *i;
        MGStruct* pMG = new MGStruct;
		pMG->nodeID = *i; // set "1st column"
    
        UserList S;
        S.insert(*i);
        UserList T;

        if (model == LTNew) {
            // computer pMG->gain now
            simPathPlus(pMG, pBestMG, S, T, 1);
        }

        pMG->flag = 1; // set "5th column" -- only one seed, so flag is only 1
        gainTable.insert(std::make_pair(pMG->gain, pMG));
        ++countUsers;
    } //endfor

    // for each Non Vertex Cover node...
    for (map<UID, double>::iterator i = outsideCovMap.begin(); i != outsideCovMap.end(); ++i) {
        MGStruct* pMG = new MGStruct;
        pMG->nodeID = i->first;
        pMG->gain = 1 + i->second;

        pMG->flag = 1;
        gainTable.insert(std::make_pair(pMG->gain, pMG));
        ++countUsers;
    } //endfor

    // pick the 1st seed
	Gains::iterator iter = gainTable.end();
	iter--; 
	MGStruct* pTop = (MGStruct *) (iter->second);
    if (pTop == NULL) {
		cout << "Some problem: pTop is NULL pointer" << endl;		
	
    } else {
		lastSeedID = pTop->nodeID; // record the last selected seed
        lastSeedMG = pTop->gain;

		curSeedSet.insert(pTop->nodeID); // the top-1 becomes a seed
		seedVec.push_back(pTop->nodeID);
        totalCov += pTop->gain; // update the current seeds' coverage
		
        writeInFile(pTop->nodeID, totalCov, totalCov, 0, 0, 0, countUsers);
		gainTable.erase(iter); // remove that seed from queue
	}


    // CELF++, from 2nd pass
    pBestMG = NULL;
    MGStruct * pCurr = NULL;
	MGStruct * pNext = NULL;
	countUsers = 0; // # of users need recomputation!

    while (curSeedSet.size() < budget) {
        Gains::iterator iter = gainTable.end();
		iter--;
		pCurr = (MGStruct *) (iter->second); // pointing to the current node
		
		if (pCurr == NULL) {
			cout << "Some problem: pCurr is NULL pointer" << endl;
		} //endif 

        // if it has been visited again, pick it as seed 
        if ( pCurr->flag == (curSeedSet.size() + 1) ) {
            lastSeedID = pCurr->nodeID;
            lastSeedMG = pCurr->gain;

			curSeedSet.insert(pCurr->nodeID);
            seedVec.push_back((pCurr->nodeID));
			totalCov += pCurr->gain;
			writeInFile(pCurr->nodeID, totalCov, pCurr->gain, 0, 0, 0, countUsers);
			
            gainTable.erase(iter);
			pBestMG = NULL;
			countUsers = 0;
			continue;
        
        } else {  // it has NOT been vistied yet, in this iteration of seeking new seed
            int old_flag = pCurr->flag;
            pCurr->flag = curSeedSet.size() + 1;
            
            iter--;
			pNext = (MGStruct *) (iter->second); // pointing to the next

            if (pCurr->v_best == lastSeedID && old_flag == curSeedSet.size()) {

                curSeedSet.insert(pCurr->nodeID);
                double cov1 = backtrackBasic(lastSeedID, curSeedSet); // compute cov(prev_best) on G-S-u;
                curSeedSet.erase(pCurr->nodeID);
                double cov2 = pCurr->gain_next;  // placeholder: cov(S+x) on G-prev_bext
                pCurr->gain_next = cov2 + cov1 - totalCov - lastSeedMG;

                if (pCurr->gain_next >= pNext->gain) {
                    lastSeedID = pCurr->nodeID;
                    lastSeedMG = pCurr->gain_next;

				    curSeedSet.insert(pCurr->nodeID); // update the seed set
                    seedVec.push_back((pCurr->nodeID));
				    totalCov += pCurr->gain_next; // update current seeds' coverage	
				    writeInFile(pCurr->nodeID, totalCov, pCurr->gain_next, 0, 0, 0, countUsers);	
			   
                    iter++;
				    gainTable.erase(iter); // delete new seed from queue
				    pBestMG = NULL;
				    countUsers = 0;	
				    continue;	
                
                } else {
                    // IF v_best is still the last seed, just move the previouly 4th column value to the 2nd column
				    // To avoid re-computation
                    pCurr->gain = pCurr->gain_next;
                    pCurr->v_best = 0;
                    pCurr->gain_next = 0;
					pCurr->flag = curSeedSet.size() + 1;

                    if (pBestMG == NULL) {
						pBestMG = pCurr;
					} else {
						if (pCurr->gain > pBestMG->gain) {
							pBestMG = pCurr;
						} //endif
					} //endif	

                    iter++;
					gainTable.erase(iter);
					gainTable.insert(pair<float, MGStruct *>(pCurr->gain, pCurr));

                } //endif

			} else {
                // recompute MG
                bool isBest;

                if (model == LTNew) {
                    UserList T;
                    isBest = simPathPlus(pCurr, pBestMG, curSeedSet, T, curSeedSet.size()+1);
                } //endif

                pCurr->flag = curSeedSet.size() + 1;
                countUsers++;

                if(pCurr->gain >= pNext->gain) {  // pick it as seed
                    lastSeedID = pCurr->nodeID; // record it to be the last added seed
                    lastSeedMG = pCurr->gain;
                    
                    curSeedSet.insert(pCurr->nodeID); // add it to seed set
                    seedVec.push_back(pCurr->nodeID);
                    totalCov += pCurr->gain; // update current seeds' coverage
                    writeInFile(pCurr->nodeID, totalCov, pCurr->gain, 0, 0, 0, countUsers);
                        
                    iter++;
                    gainTable.erase(iter); // remove new seed from queue
                    pBestMG = NULL;
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
                
            } //endif
        } //endif
    } //endwhile

    return totalCov;

} // end of function


bool LTSimplePath::simPathPlus(MGStruct *pMG, MGStruct *pBest, UserList &S, UserList &T, int iteration) {

    bool ret = false;
    
    if (iteration == 1) {
        UID v = pMG->nodeID;
        T.clear();

        xNode *startNode = NULL; // xNode struct ptr for v
        startNode = userNodes->find(v)->second; // look up v in the global node map

        multimap<double, xNode *> *inNeighbors = startNode->N_in; // get in-neighbors of u
        map<UID, double> probMap; // map<x, b(x,v)>
        
        for (multimap<double, xNode *>::iterator j = inNeighbors->begin(); j != inNeighbors->end(); ++j) {
            UID x = j->second->id;
            probMap.insert(std::make_pair(x, j->first));
            
            if (outsideNodes.find(x) != outsideNodes.end()) {
                T.insert(x);  // if an in-neighbor of u is not in VC, add it into T
            } //endif
        } //endfor

        // S-current seed set, T-the nodes we want partial coverage
        double cov1 = backtrackFirst(startNode, T, probMap);
        pMG->gain = cov1;

    } else if (iteration > 1) {
        T.clear();
        UID u; // ID of the prev best node
        if (pBest != NULL) {
            u = pBest->nodeID;
            T.insert(u);
        }

        // calculate cov(S+x) on V-u
        S.insert(pMG->nodeID); // S = S + x now
        covPlus.clear();
        covPlus.push_back(0); // cov(S+x) on V
        covPlus.push_back(0); // cov(S+x) on V-u
        for (UserList::iterator i = S.begin(); i != S.end(); ++i) {
            UID u = *i;
            xNode *startNode = userNodes->find(u)->second;
            covPlus.at(0) += backtrackPlus(startNode, curSeedSet, T);
        } //endfor
        S.erase(pMG->nodeID); // remove x from S

        pMG->gain = covPlus.at(0) - totalCov;

        // decide whether pMG is better than pBest
        if (pBest == NULL) {
            pMG->v_best = pMG->nodeID;
            pMG->gain_next = 0;
            ret = false;
        
        } else {
            if (pMG->gain > pBest->gain) {
                // this node is better than the prev. best
                pMG->v_best = pMG->nodeID;
                pMG->gain_next = 0;
                ret = true;
            } else {
                // this node is no better than the prev. best
                pMG->v_best = pBest->nodeID;
                //S.insert(pMG->nodeID);
                //double cov1 = backtrackBasic(u, S); // cov(prevBest) on V-S-curNode
                //S.erase(pMG->nodeID);
                //pMG->gain_next = covPlus.at(1) + cov1 - totalCov - pBest->gain;
                pMG->gain_next = covPlus.at(1);
                ret = false;
            } //endif

        } // endif

    } else {
        cerr << "Something is wrong, iteration number should be >= 1" << endl;
    } // endif

} //end of function



// BACKTRACK and FORWARD with manual (naive) cut off
double LTSimplePath::backtrackFirst(xNode *startNode, UserList &T, map<UID, double> &probMap) {

    //double init_tol = 0.1; // initial tolerance that can be beared for the start node:
    double cov = 1; // cov(u), initially 1 (counting itself first)
    double pp = 1; // path prob. 

    UID v = startNode->id;
    startNode->flag = true;
    startNode->cov = 1;
    startNode->pp = 1;

    map<UID, xNode *> TNodes;
    for (UserList::iterator j = T.begin(); j != T.end(); ++j) {
        xNode *u = userNodes->find(*j)->second; 
        u->flag = false; // not on path right now
        u->cov = 1; // initialize cov^{V-u}(v) to be 1 (i.e., count v itself)
        TNodes.insert(std::make_pair(*j, u));
    }

    vector<xNode *> Q;
    set<UID> Q1;
    map< UID, set<UID> > D;

    Q.push_back(startNode);
    Q1.insert(startNode->id);
    int numOutNeighbors = startNode->N_out->size(); // number of out-neighbors of the start (seed) node 
   
    uint64_t numCutOffs = (uint64_t)0;
    uint64_t numHops = (uint64_t)0;
	int maxHops = 0;
	int minHops = 10000;
    while (Q.empty() == false) {
        UID last_v = 0;

        while (true) {
            // FORWARD starts here!!!
            xNode *currentNode = Q.back(); // get the latest node on the path
            UID x = currentNode->id; // get its id
            //cout  << setprecision(15) << "** Q.last() = " << x << ", Q.size() =" << Q.size() << ", tol = " <<currentNode->tol << endl;

            if (x == last_v) {
                break;
            }
            last_v = x;

            pp = currentNode->pp; // get the current path prob. till this node

            multimap<double, xNode *> *outNeighbors = currentNode->N_out;
            if (outNeighbors == NULL || outNeighbors->empty() == true) {
                continue; // if the current node has no out-neighbors, do nothing....
            }

            for (multimap<double, xNode*>::iterator k = outNeighbors->begin(); k != outNeighbors->end(); ++k) {
                xNode *nextNode = k->second; 
                UID y = nextNode->id;
           
                if (y == v) { // y is just v
                    D[x].insert(y);
                    continue;
                } else if (Q1.find(y) != Q1.end()) { // y already on path before, would create cycle
                    continue;
                } else if (D[x].find(y) != D[x].end()) { // y is an already-explored neighbor of x
                    continue;
                }

                double ppnext = pp * k->first; // pp = pp * b_{x,y}
                // cut off the path here
                if (ppnext < cutoff_tol ) {
                    cov += ppnext;
                    D[x].insert(y); // y is explored
                    
					// for counting avg #hops
                    numCutOffs += 1;
					int curPathLen = (int) Q.size();
					numHops += curPathLen;  // add to cumulative length
					if (curPathLen > maxHops) // find max path length
						maxHops = curPathLen; 
					if (curPathLen < minHops) // find min path length
						minHops = curPathLen; 
					
                    continue;
                } 
                   
                // cout << setprecision(15) << "  next node = " << y << ", ppnext = " << pp * k->first << endl;
                cov += ppnext;

                nextNode->flag = true; // it's on the current path now
                nextNode->pp = ppnext; // path prob till this "nextNode"

                Q.push_back(nextNode); // add to the current path (queue)
                Q1.insert(y); // add to the current path (queue)
                D[x].insert(y); // y is explored

                // update partial coverage for nodes in T but not on this path (flag == false guys)
                for (map<UID, xNode *>::iterator it = TNodes.begin(); it != TNodes.end(); ++it) {
                    //xNode *tempNode = it->second;
                    if (it->second->flag == false) {
                        it->second->cov += ppnext;
                    }
                } //endfor

                break;
            } //endfor

        } // endwhile

        // BACKTRACK starts here!!!
        xNode *u = Q.back(); // pick the last node in the current path
        UID pop_id = u->id;
        if (Q.size() == 1) {
            // when you are backtracking to the start node now        
            if (pop_id != v) {
                cout << "The only remaining node in Q is: " << pop_id << ", but not: " << v << endl;
                exit(1);
            } 

            if (D[pop_id].size() < numOutNeighbors) {
                // have not explored all out neighbors of the start node, continue!!
                continue;
            } 
        }

        // reset flag, as the node is being deleted from the current path
        map<UID, xNode *>::iterator it = TNodes.find(pop_id);
        if (it != TNodes.end()) {
            it->second->flag = false;
        }

        Q1.erase(pop_id);
        D.erase(pop_id);
        Q.pop_back();

        //cout << "## Q.pop() = " << pop_id << ", Q.size() =" << Q.size() << endl;

    } //endwhile


    // update partial coverage for its non Vertex Cover in-neighbors
    for (map<UID, xNode *>::iterator it = TNodes.begin(); it != TNodes.end(); ++it) {
        UID x = it->first;
        xNode *pX = it->second;
        outsideCovMap[x] += probMap[x] * pX->cov; 

    } 

	if (optimal == 0) {
		// store avg. number of hops for paths exiting this node (v).
		double avgNumHops = 1.0 * (double)numHops / (double)numCutOffs;
		//hopsMap.insert(std::make_pair(v, avgNumHops));
		
		// write into file
		hopsOutFile << v << " " << avgNumHops << " " << maxHops << " " << minHops << " " << cov << endl;
		cout << v << " " << avgNumHops << " " << maxHops << " " << minHops << " " << cov << endl;
		//cout << v << " " << numHops << " "  << numCutOffs << " " << avgNumHops << endl;
	}
    
    return cov;
}

// compute cov(u) on V-S-x+U, for subsequent iterations 
double LTSimplePath::backtrackSubsequent(xNode *startNode, UserList &S, UserList &T) {

    double cov = 1; // cov(u), initially 1 (include itself first)
    double pp = 1; // path prob. 

    UID v = startNode->id;
    startNode->flag = true;
    startNode->cov = 1;
    startNode->pp = 1;

    // initialize partial coverage for all node u in T
    map<UID, xNode *> TNodes;
    for (UserList::iterator j = T.begin(); j != T.end(); ++j) {
        xNode *u = userNodes->find(*j)->second; 
        u->flag = false; // not on path right now
        u->cov = 1; // initialize cov^{V-u}(v) to be 1 (i.e., count v itself)
        TNodes.insert(std::make_pair(*j, u));
    }

    vector<xNode *> Q;
    set<UID> Q1;
    map< UID, set<UID> > D;

    Q.push_back(startNode);
    Q1.insert(startNode->id);
    int numOutNeighbors = startNode->N_out->size(); // number of out-neighbors of the start (seed) node 
  
    while (Q.empty() == false) {
        UID last_v = 0;

        while (true) {
            // FORWARD starts here!!!
            xNode *currentNode = Q.back(); // get the latest node on the path
            UID x = currentNode->id; // get its id
            //cout  << setprecision(15) << "** Q.last() = " << x << ", Q.size() =" << Q.size() << ", tol = " <<currentNode->tol << endl;

            if (x == last_v) {
                break;
            }
            last_v = x;

            pp = currentNode->pp; // get the current path prob. till this node

            multimap<double, xNode *> *outNeighbors = currentNode->N_out;
            if (outNeighbors == NULL || outNeighbors->empty() == true) {
                continue; // if the current node has no out-neighbors, do nothing....
            }

            for (multimap<double, xNode*>::iterator k = outNeighbors->begin(); k != outNeighbors->end(); ++k) {
                xNode *nextNode = k->second; 
                UID y = nextNode->id;
           
                if (y == v) { // y is just v
                    D[x].insert(y);
                    continue;
                } else if (Q1.find(y) != Q1.end()) { // y already on path before, would create cycle
                    continue;
                } else if (D[x].find(y) != D[x].end()) { // y is an already-explored neighbor of x
                    continue;
                } else if (S.find(y) != S.end()) { // y is a seed
                    D[x].insert(y);
                    continue;
                }

                double ppnext = pp * k->first; // pp = pp * b_{x,y}
                // cut-off here
                if (ppnext < cutoff_tol ) {
                    cov += ppnext;
                    D[x].insert(y); // y is explored
                    continue;
                } 
                   
                // cout << setprecision(15) << "  next node = " << y << ", ppnext = " << pp * k->first << endl;
                cov += ppnext;

                nextNode->flag = true; // it's on the current path now
                nextNode->pp = ppnext; // path prob till this "nextNode"

                Q.push_back(nextNode); // add to the current path (queue)
                Q1.insert(y); // add to the current path (queue)
                D[x].insert(y); // y is explored

                // update partial coverage for nodes in T but not on this path (flag == false guys)
                for (map<UID, xNode *>::iterator it = TNodes.begin(); it != TNodes.end(); ++it) {
                    //xNode *tempNode = it->second;
                    if (it->second->flag == false) {
                        it->second->cov += ppnext;
                    }
                } //endfor

                break;
            } //endfor

        } // endwhile

        // BACKTRACK starts here!!!
        xNode *u = Q.back(); // pick the last node in the current path
        UID pop_id = u->id;
        if (Q.size() == 1) {
            // when you are backtracking to the start node now        
            if (pop_id != v) {
                cout << "The only remaining node in Q is: " << pop_id << ", but not: " << v << endl;
                exit(1);
            } //endif

            if (D[pop_id].size() < numOutNeighbors) {
                // have not explored all out neighbors of the start node, continue!!
                continue;
            } //endif
        } //endif

        // reset flag, as the node is being deleted from the current path
        map<UID, xNode *>::iterator it = TNodes.find(pop_id);
        if (it != TNodes.end()) {
            it->second->flag = false;
        }

        Q1.erase(pop_id);
        D.erase(pop_id);
        Q.pop_back();

        //cout << "## Q.pop() = " << pop_id << ", Q.size() =" << Q.size() << endl;
    } //endwhile
     
    // update partial coverage for its non Vertex Cover in-neighbors
    for (map<UID, xNode *>::iterator it = TNodes.begin(); it != TNodes.end(); ++it) {
        UID x = it->first;
        xNode *pX = it->second;
        //outsideCovMap[x] += probMap[x] * pX->cov; 
        covSMap[x] += pX->cov;
    } 

    return cov;
}


// very basic backtrack, simply compute cov(v) on V-S
double LTSimplePath::backtrackBasic(UID v, UserList &S) {

    //cout << "In backtrackBasic... ID = " << v << endl;

    double cov = 1; // cov(u), initially 1 (include itself first)
    double pp = 1; // path prob. 

    xNode *startNode = NULL; // xNode struct ptr of the start node u
    map<UID, xNode *>::iterator i = userNodes->find(v); // look up u in the global node map
    if (i != userNodes->end()) {
        startNode = i->second;
        //startNode->flag = true;
        //startNode->cov = 1;
        startNode->pp = 1;
    } else {
        cerr << "Cannot find xNode struct for start node: " << v << endl; 
        return 0.0;
    } //endif

    vector<xNode *> Q;
    set<UID> Q1;
    map< UID, set<UID> > D;

    Q.push_back(startNode);
    Q1.insert(startNode->id);
    int numOutNeighbors = startNode->N_out->size(); // number of out-neighbors of the start (seed) node 
    
    while (Q.empty() == false) {
        UID last_v = 0;

        while (true) {
            // FORWARD starts here!!!
            xNode *currentNode = Q.back(); // get the latest node on the path
            UID x = currentNode->id; // get its id
            //cout  << setprecision(15) << "** Q.last() = " << x << ", Q.size() =" << Q.size() << ", tol = " <<currentNode->tol << endl;

            if (x == last_v) {
                break;
            }
            last_v = x;

            pp = currentNode->pp; // get the current path prob. till this node

            multimap<double, xNode *> *outNeighbors = currentNode->N_out;
            if (outNeighbors == NULL || outNeighbors->empty() == true) {
                continue; // if the current node has no out-neighbors, do nothing....
            }

            for (multimap<double, xNode*>::iterator k = outNeighbors->begin(); k != outNeighbors->end(); ++k) {
                xNode *nextNode = k->second; 
                UID y = nextNode->id;
           
                if (y == v) { // y is just v
                    D[x].insert(y);
                    continue;
                } else if (Q1.find(y) != Q1.end()) { // y already on path before, would create cycle
                    continue;
                } else if (D[x].find(y) != D[x].end()) { // y is an already-explored neighbor of x
                    continue;
                } else if (S.find(y) != S.end()) { // y is a seed
                    D[x].insert(y);
                    continue;
                }

                double ppnext = pp * k->first; // pp = pp * b_{x,y}
                // cut-off here
                if (ppnext < cutoff_tol ) {
                    cov += ppnext;
                    D[x].insert(y); // y is explored
                    continue;
                } 
                   
                //cout << setprecision(15) << "  next node = " << y << ", ppnext = " << pp * k->first << endl;
                cov += ppnext;

                nextNode->pp = ppnext; // path prob till this "nextNode"

                Q.push_back(nextNode); // add to the current path (queue)
                Q1.insert(y); // add to the current path (queue)
                D[x].insert(y); // y is explored

                break;
            } //endfor

        } // endwhile

        // BACKTRACK starts here!!!
        xNode *u = Q.back(); // pick the last node in the current path
        UID pop_id = u->id;
        if (Q.size() == 1) {
            // when you are backtracking to the start node now        
            if (pop_id != v) {
                cout << "The only remaining node in Q is: " << pop_id << ", but not: " << v << endl;
                exit(1);
            } //endif

            if (D[pop_id].size() < numOutNeighbors) {
                // have not explored all out neighbors of the start node, continue!!
                //cout << "AMIT AMIT AMIT" << endl;
                continue;
            } //endif
        } //endif

        Q1.erase(pop_id);
        D.erase(pop_id);
        Q.pop_back();

        //cout << "## Q.pop() = " << pop_id << ", Q.size() =" << Q.size() << endl;
    } //endwhile

    return cov;

}

// When CELF++ takes place, use this
double LTSimplePath::backtrackPlus(xNode *startNode, UserList &S, UserList &T) {
 
    double cov = 1; // cov(u), initially 1 (include itself first)
    double pp = 1; // path prob. 

    UID v = startNode->id;
    startNode->flag = true;
    startNode->cov = 1;
    startNode->pp = 1;

    // initialize partial coverage for all node u in T
    map<UID, xNode *> TNodes;
    for (UserList::iterator j = T.begin(); j != T.end(); ++j) {
        xNode *u = userNodes->find(*j)->second; 
        u->flag = false; // not on path right now
        u->cov = 1; // initialize cov^{V-u}(v) to be 1 (i.e., count v itself)
        TNodes.insert(std::make_pair(*j, u));
    }

    vector<xNode *> Q;
    set<UID> Q1;
    map< UID, set<UID> > D;

    Q.push_back(startNode);
    Q1.insert(startNode->id);
    int numOutNeighbors = startNode->N_out->size(); // number of out-neighbors of the start (seed) node 
  
    while (Q.empty() == false) {
        UID last_v = 0;

        while (true) {
            // FORWARD starts here!!!
            xNode *currentNode = Q.back(); // get the latest node on the path
            UID x = currentNode->id; // get its id
            //cout  << setprecision(15) << "** Q.last() = " << x << ", Q.size() =" << Q.size() << ", tol = " <<currentNode->tol << endl;

            if (x == last_v) {
                break;
            }
            last_v = x;

            pp = currentNode->pp; // get the current path prob. till this node

            multimap<double, xNode *> *outNeighbors = currentNode->N_out;
            if (outNeighbors == NULL || outNeighbors->empty() == true) {
                continue; // if the current node has no out-neighbors, do nothing....
            }

            for (multimap<double, xNode*>::iterator k = outNeighbors->begin(); k != outNeighbors->end(); ++k) {
                xNode *nextNode = k->second; 
                UID y = nextNode->id;
           
                if (y == v) { // y is just v
                    D[x].insert(y);
                    continue;
                } else if (Q1.find(y) != Q1.end()) { // y already on path before, would create cycle
                    continue;
                } else if (D[x].find(y) != D[x].end()) { // y is an already-explored neighbor of x
                    continue;
                } else if (S.find(y) != S.end()) { // y is a seed
                    D[x].insert(y);
                    continue;
                }

                double ppnext = pp * k->first; // pp = pp * b_{x,y}
                // cut-off here
                if (ppnext < cutoff_tol ) {
                    cov += ppnext;
                    D[x].insert(y); // y is explored
                    continue;
                } 
                   
                // cout << setprecision(15) << "  next node = " << y << ", ppnext = " << pp * k->first << endl;
                cov += ppnext;

                nextNode->flag = true; // it's on the current path now
                nextNode->pp = ppnext; // path prob till this "nextNode"

                Q.push_back(nextNode); // add to the current path (queue)
                Q1.insert(y); // add to the current path (queue)
                D[x].insert(y); // y is explored

                // update partial coverage for nodes in T but not on this path (flag == false guys)
                for (map<UID, xNode *>::iterator it = TNodes.begin(); it != TNodes.end(); ++it) {
                    //xNode *tempNode = it->second;
                    if (it->second->flag == false) {
                        it->second->cov += ppnext;
                    }
                } //endfor

                break;
            } //endfor

        } // endwhile

        // BACKTRACK starts here!!!
        xNode *u = Q.back(); // pick the last node in the current path
        UID pop_id = u->id;
        if (Q.size() == 1) {
            // when you are backtracking to the start node now        
            if (pop_id != v) {
                cout << "The only remaining node in Q is: " << pop_id << ", but not: " << v << endl;
                exit(1);
            } //endif

            if (D[pop_id].size() < numOutNeighbors) {
                // have not explored all out neighbors of the start node, continue!!
                continue;
            } //endif
        } //endif

        // reset flag, as the node is being deleted from the current path
        map<UID, xNode *>::iterator it = TNodes.find(pop_id);
        if (it != TNodes.end()) {
            it->second->flag = false;
        }

        Q1.erase(pop_id);
        D.erase(pop_id);
        Q.pop_back();

    } //endwhile
     
    // update partial coverage for its non Vertex Cover in-neighbors
    for (map<UID, xNode *>::iterator it = TNodes.begin(); it != TNodes.end(); ++it) {
        UID x = it->first;
        xNode *pX = it->second;
        //outsideCovMap[x] += probMap[x] * pX->cov; 
        covPlus.at(1) += pX->cov;
    } 

    return cov;   

}

void LTSimplePath::vertexCoverMaxDeg() {
    cout << "In vertexCoverMaxDeg()" << endl;

    multimap<int, xNode *> degQueue;
    for (map<UID, xNode *>::iterator i = userNodes->begin(); i != userNodes->end(); ++i) {
        xNode *uNode = i->second;  // node struct
        degQueue.insert(std::make_pair(uNode->in_deg, uNode)); // insert in-degree into sorted map
    } //endfor
    
    int deletedEdges = 0;
    multimap<int, xNode *>::reverse_iterator rit = degQueue.rbegin(); 
    while (vertexCover.size() + outsideNodes.size() < users.size()) {
        xNode *topNode = rit->second;
        UID v = topNode->id;
        ++rit;

        // insert v into VC if not already outside VC
        if (outsideNodes.find(v) == outsideNodes.end()) {
            vertexCover.insert(v);
        } //endif

        // delete incoming edges;
        multimap<double, xNode *> *in = topNode->N_in;
        for (multimap<double, xNode *>::iterator i = in->begin(); i != in->end(); ++i) {
            xNode *x = i->second;
            x->out_deg--;
            deletedEdges++;
            
            if (x->out_deg == 0 && x->in_deg == 0) {
                if (vertexCover.find(x->id) == vertexCover.end()) {
                    outsideNodes.insert(x->id);
                } //endif
            } //endif
        } //endfor

        multimap<double, xNode *> *out = topNode->N_out;
        for (multimap<double, xNode *>::iterator i = out->begin(); i != out->end(); ++i) {
            xNode *x = i->second;
            x->in_deg--;
            deletedEdges++;
            
            if (x->out_deg == 0 && x->in_deg == 0) {
                if (vertexCover.find(x->id) == vertexCover.end()) {
                    outsideNodes.insert(x->id);
                } //endif
            } //endif
        } //endfor

    }//endwhile


    cout << "Number of VC nodes: " << vertexCover.size() << endl;
    cout << "Number of outside nodes: " << outsideNodes.size() << endl;

}


float LTSimplePath::getTime() const {
	//time_t curTime;
	//time(&curTime);
	clock_t curTime = clock();

	float min = ((float)(curTime - startTime) / CLOCKS_PER_SEC)/60;
	return min;
}


void LTSimplePath::openOutputFiles() {

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


void LTSimplePath::writeInFile(UID v, float cov, float marginal_gain, int curTimeStep, float actualCov, float actualMG, int countUsers) {

        seedFile<<v<<endl;
        int size = curSeedSet.size();
        if(size==1 || size==2 || size==5||size==10||(size%25==0&& size>10)){
                statFile<<size<<" "<<cov<<" "<<getTime()<<" "<<getCurrentMemoryUsage()<<endl;
        }
	//cout << endl << endl;
}


void LTSimplePath::readInputData() {
    edges = -1;

    string probGraphFile = opt->getValue("probGraphFile");
    cout << "Reading file " << probGraphFile << endl;
    ifstream myfile (probGraphFile.c_str(), ios::in);
	string delim = " \t";	

    if (myfile.is_open()) {
        while (! myfile.eof() )	{
            std::string line;
			getline (myfile,line);
			if (line.empty()) {continue;}
			edges++;
		//	if (edges == 0) {continue;} // ignore the first line
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
			double parameter1 = 0;
			prevpos = line.find_first_not_of(delim, pos);
			pos = line.find_first_of(delim, prevpos);
			if (pos == string::npos) 
				parameter1 = strToDouble(line.substr(prevpos));
			else
				parameter1 = strToDouble(line.substr(prevpos, pos-prevpos));

			if (parameter1 == 0) {
                //cout << "Zero degree edge: " << u1 << " " << u2 << " " << parameter1 << endl;
                edges--; 
                continue;
            }
            
            // ************************************************//
            std::pair<set<UID>::iterator,bool> ret1, ret2;
            ret1 = users.insert(u1);
            ret2 = users.insert(u2);

            if (edges % 1000000 == 0) {
				cout << "(node1, node2, weight, nodes till now, edges till now, mem) = " << u1 << ", " << u2 << ", " << parameter1 << ", " << userNodes->size() << ", " << edges << ", " << getCurrentMemoryUsage() << endl;
			}

            xNode *user1 = NULL;
            xNode *user2 = NULL;
            //set<UID> *nlist1 = NULL;
            //set<UID> *nlist2 = NULL;
            // get xNode struct for u1 here
            if (ret1.second == true) {
                // u1 is new, create a xNode struct for her
                user1 = getNewTuple(u1, false, 1, 1, 1, 1); 
                user1->N_in = new multimap<double, xNode *>();
                user1->N_out = new multimap<double, xNode *>();
                userNodes->insert(std::make_pair(u1, user1));
            } else {
                // u1 already been encountered, just pull its xNode struct from the global map
                map<UID, xNode *>::iterator i1 = userNodes->find(u1);
                if (i1 != userNodes->end()) {
                    user1 = i1->second;
                } else {
                    cout << "cannot find xNode struct for user " << u1 << endl;
                }

           
            } //endif

            // get xNode struct for u2 here
            if (ret2.second == true) {
                // u2 is new, create a xNode struct for her
                user2 = getNewTuple(u2, false, 1, 1, 1, 1); 
                user2->N_in = new multimap<double, xNode *>();
                user2->N_out = new multimap<double, xNode *>();
                userNodes->insert(std::make_pair(u2, user2));
            
                // create an adjacency list
                //nlist2 = new set<UID>();
                //adjMap->insert(std::make_pair(u2, nlist2));

            } else {
                // u2 already been encountered, just pull its xNode struct from the global map
                map<UID, xNode *>::iterator i2 = userNodes->find(u2);
                if (i2 != userNodes->end()) {
                    user2 = i2->second;
                } else {
                    cout << "cannot find xNode struct for user " << u2 << endl;
                }

            } //endif

            // now we should have xNode struct for both u1 and u2
            // so insert neighbor relationships here:
            user1->N_out->insert(std::make_pair(parameter1, user2));
            user2->N_in->insert(std::make_pair(parameter1, user1));

        } //endwhile

        myfile.close();

    } else {
        cout << "CANNOT OPEN friendship graph file, PROGRAM TERMINATED!" << probGraphFile << endl;
		exit(1);
    } //endif

    // gather in/out-degree information
    for (map<UID, xNode *>::iterator i = userNodes->begin(); i != userNodes->end(); ++i) {
        xNode *user = i->second;
        user->in_deg = user->N_in->size();
        user->out_deg = user->N_out->size();
    } //endfor

    cout << "BuildAdjacencyMatFromFile done" << endl;
    cout << "Size of the user set: " << users.size() << endl;
    cout << "Size of the global map: " << userNodes->size() << endl;
    cout << "Number of edges in the friendship graph are: " << edges << endl;

}

// end of name space here
}
