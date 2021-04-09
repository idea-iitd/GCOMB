#include "LDAG.h"

namespace _LDAG {

LDAG::LDAG(AnyOption* opt1) {

	opt = opt1;
	
	cout << "In testing phase" << endl;
	
	AM_out = new HashTreeCube(89041);
	AM_in = new HashTreeCube(80941);
	alphaMap = new HashTreeCube(80941);
	apMap = new HashTreeCube(80941);
	
	outdir = opt->getValue("outdir");
	probGraphFile = opt->getValue("probGraphFile");
	tol = strToFloat(opt->getValue("tol_ldag"));
	graphType = DIRECTED;
	
	setModel(opt);
	
	cout << "User specified options : " << endl;
	cout << "model : " << m << " or " << model << endl;
	cout << "outdir : " << outdir << endl;
	cout << "Input graph file : " << probGraphFile << endl;
	
	//srand(time(NULL)); 

}


LDAG::~LDAG() {

	cout << "total time taken before freeing alloacted memory : " << getTime() << endl;

	delete AM_out;
	delete AM_in;
	delete alphaMap;
	delete apMap;
	
	for (map< UID, deque<UID> * >::iterator i = LDAGNodes.begin(); i != LDAGNodes.end(); i++) {
		deque<UID> *p = i->second;
		delete p;
	}

	for (map< UID, set<UID> * >::iterator i = infSets.begin(); i != infSets.end(); i++) {
		set<UID> *p = i->second;
		delete p;
	}

}


void LDAG::setModel(AnyOption* opt) {

	m = opt->getValue("propModel");
	model = LT;
	if (m.compare("LT") == 0) {
		model = LT;	
	} else if (m.compare("IC") == 0) {
		model = IC;
	} else if (m.compare("PC") == 0) {
		model = PC;
	}

}


void LDAG::doAll() {

	readInputData();

	//time (&startTime);
	clock_t startTime = clock();
	
	int phase = strToInt(opt->getValue("phase"));
	problem = "LDAG";
	//cout << "problem:: " << problem << endl;
	
	openOutputFile();
	
	if (phase == 15) {
		mineSeedSet();
	} else {
		cerr << "Warning: phase should be 15 in order to execute LDAG." << endl;
	}
	
	outFile.close();	

	MC* mc = new MC(opt);
	mc->setAM(AM_out) ;
	mc->writeCovInFile(seedVec, tol);
}


float LDAG::mineSeedSet() {

    cout << endl << "In mineSeedSet()......" << endl;

	clearAll();

    	//const int budget = 50;
    	int budget = strToInt(opt->getValue("budget"));
	
	 // needed to implement CELF
	totalCov = 0;
	currentMG = 0;
	UID s = 0; // the currently picked seed
	
	initSeedSet();

    cout << "Done initSeedSet(), Start to pick seeds!" << endl;

	for (int i = 1; i <= budget; i++) {
		multimap<float, MGStruct *>::iterator qit = revMGHeap.end();
		qit--; // node with the largest MG: picking it as seed
		
		currentMG = qit->first; 
		MGStruct *pSeed = qit->second;
		s = pSeed->uid; 
		totalCov += currentMG;
		
		// remove it from queue
		revMGHeap.erase(qit);
	
		set<UID> *pSet = NULL;
		map<UID, set<UID>*>::iterator sit = infSets.find(s);
		if (sit != infSets.end()) {
			pSet = sit->second;
		} 
		if (pSet == NULL) {
			cerr << "error 01: cannot find the InfSet of seed. " << "seed ID = " << s << endl;
			continue;
		}
		
		/*// DEBUG
		cout << endl << "@@@ Nodes in InfSet(" << s << ")" << ", size = " << pSet->size() << ": ";
		for (set<UID>::iterator vit = pSet->begin(); vit != pSet->end(); vit++) {
			cout << *vit << " ";
		}
		cout << endl; */
		
		// for each v in InfSet(s) but is not seed already
		for (set<UID>::iterator vit = pSet->begin(); vit != pSet->end(); vit++) {
			UID v = *vit;
			if (curSeedSet.count(v) == 1)
				continue;
			
			deque<UID> *pLDAG = NULL;
			FriendsMap *pAlpha = NULL;
			FriendsMap *pAP = NULL;
			
			// retrieve the LDAG of v
			map<UID, deque<UID> *>::iterator iter1 = LDAGNodes.find(v);
			if (iter1 != LDAGNodes.end()) {
				pLDAG = iter1->second;	
			} 
			if (pLDAG == NULL) {
				cerr << "error 02: cannot find its LDAG. " << "node ID = " << v << endl;
				continue;
			}
			
			// retrieve this LDAG's alpha values
			pAlpha = alphaMap->find(v);
			if (pAlpha == NULL) {
				cerr << "error 03: cannot find its alpha map. " << "node ID = " << v << endl;
				continue;
			}
			
			// retrieve this LDAG's activation probability values
			pAP = apMap->find(v);
			if (pAP == NULL) {
				cerr << "error 04: cannot find its ap map. " << "node ID = " << v << endl;
				continue;
			}
			
			//  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
			// update alpha, i.e., ap(v,{u}), for all u that can reach s in LDAG(v)
			deque<UID> alphaQueue;
			map<UID, float> deltaAlphaMap;

			for(deque<UID>::iterator uit = pLDAG->begin(); uit != pLDAG->end(); uit++) {
				UID u = *uit;
				
				if (u != s && curSeedSet.count(u) == 1) {  // u already in Seed Set
					deltaAlphaMap.insert(std::make_pair(u, 0));	
					alphaQueue.push_back(u);
				
				} else if (u != s && curSeedSet.count(u) == 0) {
					set<UID> *pInfSet = NULL;
					map<UID, set<UID> *>::iterator sit = infSets.find(u);
					
					if (sit != infSets.end()) {
						pInfSet = sit->second;
						if (pInfSet != NULL && pInfSet->count(s)) {  // u can reach s: s \in InfSet(u)
							alphaQueue.push_back(u);
						}
					
					} else {
						cerr << "error 05: cannot find u's InfSet. " << "node ID = " << u << endl;
						continue;
					}
				} 	
			}
			
			// ensure s is sorted first
			FriendsMap::iterator it = pAlpha->find(s);
			if (it != pAlpha->end()) {  // must be true, because v \in InfSet(s)
				float alpha_s = it->second; 
				deltaAlphaMap.insert(std::make_pair(s, -1*alpha_s));
				// update alpha: ap(v,{s}) = 0, as s is chosen as seed.
				pAlpha->erase(it);
				pAlpha->insert(std::make_pair(s, 0));
				
				alphaQueue.push_back(s);
			
			} else {
				cerr << "error 06: s (" << s << ") is not in the LDAG of v (" << v  << "). " << endl;
			}

			/*// DEBUG
            cout << "alphaQueue of " << v << ": " ;
            for (deque<UID>::reverse_iterator ait = alphaQueue.rbegin(); ait != alphaQueue.rend(); ait++) {
                cout << *ait << ", ";
            }
            cout << endl;
			*/

			// computing delta-alpha using algorithm 4.  Reversely iterate the topological order.
			for (deque<UID>::reverse_iterator ait = alphaQueue.rbegin(); ait != alphaQueue.rend(); ait++) {
				UID u = *ait;
				float delta_alpha_u = 0;
				
				if (u == s || curSeedSet.count(u) == 1)
					continue;

				FriendsMap *neighbors = AM_out->find(u);
				if (neighbors == NULL) {
					deltaAlphaMap.insert(std::make_pair(u, 0));	
				} else {
					for (deque<UID>::iterator xit = alphaQueue.begin(); xit != alphaQueue.end(); xit++) {
						UID x = *xit;
						
						if (neighbors->count(x) == 1) {
							FriendsMap::iterator niter = neighbors->find(x);
							float weight = niter->second; // edge weights (u, x)							
							map<UID, float>::iterator deltaIter = deltaAlphaMap.find(x);
							if (deltaIter != deltaAlphaMap.end()) {
								float delta_alpha_x = deltaIter->second;
								delta_alpha_u += weight * delta_alpha_x;
							}
						}
					}
					
			        deltaAlphaMap.insert(std::make_pair(u, delta_alpha_u));
					// deltaAlphaMap[u] = delta_alpha_u;
				}
				
				// updating new alpha = old alpha + delta alpha
				FriendsMap::iterator tmpIt = pAlpha->find(u);
				if (tmpIt != pAlpha->end()) {
					float new_alpha = tmpIt->second + delta_alpha_u;
					pAlpha->erase(tmpIt);
					pAlpha->insert(std::make_pair(u, new_alpha));
				}
				
				// update marginal gain from new alpha
				FriendsMap::iterator apIter = pAP->find(u);
				if (apIter != pAP->end()) {
					float ap_v_u = apIter->second;
					
					map<UID, MGStruct *>::iterator iter = MGHeap.find(u);
					MGStruct *pMG = iter->second;
					float old_gain = pMG->gain;
					pMG->gain += delta_alpha_u * (1 - ap_v_u);
					
					// update marginal gain to heap
					MGHeap.erase(iter);
					MGHeap.insert(std::make_pair(u, pMG));
					
					// update in multimap, as well
					multimap<float, MGStruct *>::iterator revIt;
					pair< multimap<float, MGStruct *>::iterator, multimap<float, MGStruct *>::iterator > revRet;
					revRet = revMGHeap.equal_range(old_gain);	
					
					MGStruct *pRevMG;
					for (revIt = revRet.first; revIt != revRet.second; revIt++) {
						pRevMG = (*revIt).second;
						if (pRevMG->uid == u) {
							revMGHeap.erase(revIt);
							revMGHeap.insert(std::make_pair(pRevMG->gain, pRevMG));
							break;
						}
					}
					
				} else {
					cerr << "error 07: while updating MG, cannot find its activation probability" << "node ID = " << u << ", in LDAG of " << v << endl;
					continue;
				}

			}
			
			//  $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
			//  update activitation prob ap(u) in LDAG(v) for all u reachable from s	
			deque<UID> apQueue;
			map<UID, float> deltaAPMap;
			for(deque<UID>::iterator uit = pLDAG->begin(); uit != pLDAG->end(); uit++) {
				UID u = *uit;
				if (u != s && curSeedSet.count(u) == 1) {
					deltaAPMap.insert(std::make_pair(u, 0));
					apQueue.push_back(u);
			
				} else if (u != s && curSeedSet.count(u) == 0) {
					if (pSet->count(u) == 1) { // u is reachable from s: u \in InfSet(s), or s \in LDAG(u)
						apQueue.push_back(u);
					}
				}
			}
			
			// ensure s is sorted first
			FriendsMap::iterator it2 = pAP->find(s);
			if (it2 != pAP->end()) {
				float old_ap = it2->second;
				deltaAPMap.insert(std::make_pair(s, 1-old_ap));
				apQueue.push_front(s);
				pAP->erase(it2);
				pAP->insert(std::make_pair(s, 1));
			} else {
				cerr << "error 08: s (" << s << ") is not in the LDAG of v (" << v << "). " << endl;
			}
			
			// update acti. prob. using algorithm 2
			for(deque<UID>::iterator ait = apQueue.begin(); ait != apQueue.end(); ait++) {
				UID u = *ait;
				float delta_ap_u = 0;
				
				if (u == s || curSeedSet.count(u) == 1) 
					continue;
			
				FriendsMap *neighbors = AM_in->find(u);
				if (neighbors == NULL) {
					deltaAPMap.insert(std::make_pair(u, 0));
				} else {
					for(deque<UID>::iterator xit = apQueue.begin(); xit != apQueue.end(); xit++) {
						UID x = *xit;
						if (neighbors->count(x) == 1) {
							FriendsMap::iterator niter = neighbors->find(x);
							float weight = niter->second;
							map<UID, float>::iterator deltaIter = deltaAPMap.find(x);
							if (deltaIter != deltaAPMap.end()) {
								float delta_ap_x = deltaIter->second;
								delta_ap_u += weight * delta_ap_x;
							}
						}
					}
					
					deltaAPMap.insert(std::make_pair(u,delta_ap_u));
				
				}		
					
				// new ap = old ap + delta ap
				FriendsMap::iterator tmpIt = pAP->find(u);
				if (tmpIt != pAP->end()) {
					float new_ap = tmpIt->second + delta_ap_u;
					pAP->erase(tmpIt);
					pAP->insert(std::make_pair(u, new_ap));
				} 
				
				// update marginal gain based on new acti. prob.
				FriendsMap::iterator alphaIter = pAlpha->find(u);
				if (alphaIter != pAlpha->end()) {
					float alpha_v_u = alphaIter->second;
					
					map<UID, MGStruct *>::iterator iter = MGHeap.find(u);
					MGStruct *pMG = iter->second;
					float old_gain = pMG->gain;
					pMG->gain += delta_ap_u * alpha_v_u;
					
					MGHeap.erase(iter);
					MGHeap.insert(std::make_pair(u, pMG));
					
					multimap<float, MGStruct *>::iterator revIt;
					pair< multimap<float, MGStruct *>::iterator, multimap<float, MGStruct *>::iterator > revRet;
					revRet = revMGHeap.equal_range(old_gain);	
					MGStruct *pRevMG;
					for (revIt = revRet.first; revIt != revRet.second; revIt++) {
						pRevMG = (*revIt).second;
						if (pRevMG->uid == u) {
							revMGHeap.erase(revIt);
							revMGHeap.insert(std::make_pair(pRevMG->gain, pRevMG));
							break;
						}
					}
					
				} else {
					cerr << "error 09: cannot find its ap. " << "node ID = " << u << " in LDAG of " << v << endl;
					continue;
				}
			}
		}
		
		curSeedSet.insert(s);
		seedVec.push_back(s);
        writeOutputFile(s, totalCov, currentMG, 0);
		
		// compute actual LT coverage of the seed set mined by LDAG algo.
		/*
		if (curSeedSet.size() % 10 == 0) {
			float actual_cov = computeActualCov(curSeedSet);
			writeActualCovToFile(actual_cov);
		}*/
		
	}

}


// initialize the seed set;
void LDAG::initSeedSet() {

	// set initial marginal gains to be 0 for all
	for (UserList::iterator i = users.begin(); i != users.end(); ++i) {
		MGStruct *pMG = new MGStruct;
		pMG->uid = (UID) *i;
		pMG->gain = 0;
		MGHeap.insert(std::make_pair(pMG->uid, pMG));
	}

    cout << "Initial MGHeap constructed, with size: " << MGHeap.size() << endl;
	
//	float tol = 1.0 / 320;
	
	// construct LDAGs, compute initial activation probabilities and alpha values.
	for (UserList::iterator i = users.begin(); i!=users.end(); ++i) {
		UID v = *i;
		
		// construct LDAG
        greedyLDAG(v, tol); 
		
		deque<UID> *pLDAG;
		map<UID, deque<UID> *>::iterator iter = LDAGNodes.find(v);
		if (iter != LDAGNodes.end()) {
			pLDAG = iter->second;	
		} 
		if (pLDAG == NULL) {
			cerr << "error 10: cannot find its LDAG. " << "node ID = " << v << endl;
			continue;
		}
		
		// initialize activatitaion probability
		FriendsMap *pAP = new FriendsMap();	
		for (deque<UID>::iterator it = pLDAG->begin(); it != pLDAG->end(); it++) {
			pAP->insert(std::make_pair(*it, 0));  // ap_v(u)
		}
		apMap->insert(v, pAP);
		
		// compute alpha value
		initAlpha(v);
		
		// update marginal gain from alpha values
		FriendsMap *pAlpha = alphaMap->find(v);
		if (pAlpha == NULL) {
			cerr << "error 10.5: cannot find its alpha map. " << "node ID = " << v << endl;
			continue;
		}
		for (deque<UID>::iterator it = pLDAG->begin(); it != pLDAG->end(); it++) {
			UID u = *it;
			float alpha_u = pAlpha->find(u)->second; // ap(v,{u}) value in this LDAG;
			
			map<UID, MGStruct *>::iterator mgit = MGHeap.find(u);
			if (mgit != MGHeap.end()) {
				MGStruct *pMG = mgit->second;
				pMG->gain += alpha_u;
			} else {
				cerr << "error 11: cannot find mg of u" << endl;
			}
		}
	}	
	
	// construct the heap indexing on marginal gains.
	for(map<UID, MGStruct *>::iterator mgit = MGHeap.begin(); mgit != MGHeap.end(); mgit++) {
		MGStruct *pMG = mgit->second;
		revMGHeap.insert(std::make_pair(pMG->gain, pMG));
	}

}

/* Construct LDAG using the greedy algorithm for node v with tolerance tol */
void LDAG::greedyLDAG(UID v, float tol) {

	vector<UID> nodeList;  // X - LDAG nodes
	deque<UID> *sortList = new deque<UID>(); // X in topological order
	map< UID, set<UID> > edgeMap; // Y - LDAG edges
	map<UID, int> degMap;  // in-degrees for nodes in X 	
	
    FriendsMap infMap;  // store all Inf(u,v)
	infMap.insert(std::make_pair(v, 1));  // Inf(v,v) = 1

    multimap<float, UID> infHeap;
	infHeap.insert(std::make_pair(1, v));
    
    float max_inf;
    UID max_id;

    while(true) {
		max_inf = -1;
		max_id = 0;

         multimap<float, UID>::iterator hit;
        if (infHeap.size() >= 1) {
            hit = infHeap.end();
            hit--;
        } else {
            // cout << "No more nodes to select from infHeap!" << endl;
            break;
        }
        
        max_id = hit->second;
        max_inf = hit->first;
        // cout << ">> max node id = " << max_id << ", with influence of " << max_inf << endl;
	
        infHeap.erase(hit);

		if (max_id == 0 || max_inf == -1) {
			cerr << "error 12: no node gets selected...exit..." << endl;
			break;
		} else if (max_inf < tol) {
			// cout << "Below tolerance so exits" << endl;
			break;  // terminate if no node with influence that is above the threshold
		} 
	
        // x should not be in X already 
		vector<UID>::iterator iter = find(nodeList.begin(), nodeList.end(), max_id);
		if (iter != nodeList.end())
			continue;
		
		if (nodeList.empty()) { // v is the first node added into its LDAG
			nodeList.push_back(v); 
			degMap.insert(std::make_pair(v, 0));
			
		} else {
			FriendsMap *neighbors = AM_out->find(max_id);
			if (neighbors != NULL) { 
				set<UID> endPoints;
                // add edges (x,u) into LDAG, for all u already in LDAG
				for (FriendsMap::iterator it = neighbors->begin(); it != neighbors->end(); it++) {
					UID u = it->first;	
					vector<UID>::iterator tempIt = find(nodeList.begin(), nodeList.end(), u);
					
					if (tempIt != nodeList.end()) {
						endPoints.insert(u); 
						//cout << "    Edge " << max_id << "," << u << " added into Y." << endl;
						map<UID, int>::iterator degIter = degMap.find(u);
						if (degIter != degMap.end()) {
							degIter->second += 1;
						} else {
							degMap.insert(std::make_pair(u, 1));
						}	
					}
				}	
				
				edgeMap.insert(std::make_pair(max_id, endPoints));	// after adding all edges		
			} 
		
			nodeList.push_back(max_id); // X = X + {x};
			// add its degree info
			map<UID, int>::iterator tmpIt = degMap.find(max_id);
			if (tmpIt == degMap.end()) {
				degMap.insert(std::make_pair(max_id, 0));
			}

            // cout << "Selected a LDAG node: " << max_id << " with influence " << max_inf << endl;
		}
		
		// insert v into x's InfSet
		map<UID, set<UID> *>::iterator it = infSets.find(max_id);
		if (it == infSets.end()) {
			set<UID>* infset = new set<UID>();
			infset->insert(v);
			infSets.insert(std::make_pair(max_id,infset));
		} else {
			it->second->insert(v);
		}
			
		// update x's in-neighbors' influence on v;
		FriendsMap *inNeighbors = AM_in->find(max_id);
		if (inNeighbors != NULL) {
            for (FriendsMap::iterator it = inNeighbors->begin(); it != inNeighbors->end(); it++) {
				UID u = it->first;
				float weight = it->second;
				// cout << "    Edge (" << u << "," << max_id << ") = " << weight << endl;
				
                if (infMap.count(u) == 0) {
                    float new_inf = weight * max_inf;
                    infMap.insert(std::make_pair(u, new_inf));
                    infHeap.insert(std::make_pair(new_inf, u));
                
                } else {
                    FriendsMap::iterator uit = infMap.find(u);
                    float old_inf = uit->second;
                    
                    // updating infMap
                    uit->second += weight * max_inf; 
                    
                    // updating infHeap
                    multimap<float, UID>::iterator rit;
                    pair< multimap<float, UID>::iterator, multimap<float, UID>::iterator > ret;
                    
                    ret = infHeap.equal_range(old_inf);
                    for (rit = ret.first; rit != ret.second; rit++) {
                        if (u == (*rit).second) {
                            float new_inf = old_inf + weight * max_inf;
                            infHeap.insert(std::make_pair(new_inf, u));
                            break;
                        }
                    }

                }

			}
	
        } 	
	}
	
	// safety check
//	cout << "Node " << v << ": its LDAG has " << nodeList.size() << " vertices." << endl;
	if (nodeList.size() != degMap.size()) {
		cerr << "error 13: The sizes of nodeList and degMap do not match!" << endl;
		exit(1);
	}
	
	// Topological sort LDAG nodes in X
	queue<UID> S;
	for (map<UID, int>::iterator it = degMap.begin(); it != degMap.end(); it++) {
		if (it->second == 0) {
			S.push(it->first); // add zero in-degree nodes into S
			it->second = -1; // Just mark it as processed
		}
	}
	
	while(!S.empty()) {
		UID n = S.front();
		sortList->push_back(n); // remove one zero in-deg node from S, add it into L
		S.pop();
		
		map< UID, set<UID> >::iterator iter = edgeMap.find(n);
		
		if (iter != edgeMap.end()) {
			set<UID> endPoints = iter->second;
			for (set<UID>::iterator it = endPoints.begin(); it != endPoints.end(); it++) {
				UID m = *it;
				map<UID,int>::iterator tmpIt = degMap.find(m);
				tmpIt->second -= 1;
				if (tmpIt->second == 0) {
					S.push(tmpIt->first);
				}
			}
		
		} /*else {
			cout << "in topological sort, this node " << n << " has no outgoing edges" << endl;
		}*/
	}
	
	// to ensure v is at the very end of the deque!!!
	if (sortList->back() != v) {
		deque<UID>::iterator it = find(sortList->begin(), sortList->end(), v);
		if (it != sortList->end()) {
			sortList->erase(it);
			sortList->push_back(v);
		} else {
			cerr << "error 14: v was not added to deque!" << endl;
		}	
	}
	
	/*// Debug
	cout << "Topological order of LDAG: ";
	for (deque<UID>::iterator it = sortList->begin(); it != sortList->end(); it++) {
		cout << *it << " ";
	}
	cout << endl;
	*/
	
	if (sortList->size() < nodeList.size()) {
		cerr << "error 15: graph has circles!" << endl;
		cout << "size of topological list: " << sortList->size() << endl;
		cout << "size of LDAG nodes: " << nodeList.size() << endl;
	}

    // cout << "Topological sorting is done for LDAG of " << v << endl << endl;

	// update the hashtreecube
	// put infMap and sortList into the global data structure...
	LDAGNodes.insert(std::make_pair(v, sortList));
	//LDAGInfs.insert(pair<UID, FriendsMap*>(v,infMap));	

}


// compute ap(v,{u}) for the first time.
void LDAG::initAlpha(UID v) {

	map<UID, deque<UID> *>::iterator iter = LDAGNodes.find(v);
	if (iter == LDAGNodes.end()) {
		cerr << "error 15: cannot find its LDAG: " << "node ID = " << v << endl;
		exit(1);
	}
	deque<UID> *pLDAG = iter->second;
	
	FriendsMap *pAlpha = new FriendsMap();
	pAlpha->insert(std::make_pair(v, 1));
	
	for(deque<UID>::reverse_iterator rit = pLDAG->rbegin(); rit != pLDAG->rend(); rit++) {
		UID u = *rit;
		if (u != v) {
			pAlpha->insert(std::make_pair(u, 0));
		}
	}
	
	for(deque<UID>::reverse_iterator rit = pLDAG->rbegin(); rit != pLDAG->rend(); rit++) {
		UID u = *rit;
		if (u != v) {
			FriendsMap* neighbors = AM_out->find(u);
			if (neighbors != NULL) {
				for (FriendsMap::iterator oit = neighbors->begin(); oit != neighbors->end(); oit++) {
					UID x = oit->first;
					float weight = oit->second;
					pAlpha->find(u)->second += weight * pAlpha->find(x)->second;
				}
				
				// cout << "#### node: " << u << ", alpha value = " << pAlpha->find(u)->second << endl;
			}
		}
	}
	
	alphaMap->insert(v, pAlpha);

}



// construct both in and out AM;
void LDAG::readInputData() {

	cout << "in readInputData for model " << model << endl;

	unsigned int edges = -1;

	cout << "LDAG: Reading file " << probGraphFile << "; tol_ldag: " << tol << endl;
	ifstream myfile(probGraphFile, ios::in);
	string delim = " \t";
	//string delim = " ";
	
	if (myfile.is_open()) {
		while (!myfile.eof())	{
			std::string line;
			getline (myfile,line);
			if (line.empty())  continue;
            edges++;
		//	if (edges == 0)  continue; // ignore the first line
			
			std::string::size_type pos = line.find_first_of(delim);
			int	prevpos = 0;
			
			// get first user
			string str = line.substr(prevpos, pos-prevpos);
			UID u1 = strToInt(str);
			
			// get the second user
			prevpos = line.find_first_not_of(delim, pos);
			pos = line.find_first_of(delim, prevpos);
			UID u2 = strToInt(line.substr(prevpos, pos-prevpos));
			u1++;
			u2++;
            if (u1 == u2) continue;
            
			// get the parameter
			float parameter1 = 0;
			prevpos = line.find_first_not_of(delim, pos);
			pos = line.find_first_of(delim, prevpos);
			if (pos == string::npos) {
				parameter1 = strToFloat(line.substr(prevpos));
			} else {
				parameter1 = strToFloat(line.substr(prevpos, pos-prevpos));
			}
			
			if (parameter1 == 0) {
				continue;
			}
			users.insert(u1);
			users.insert(u2);

			if (edges % 5000 == 0) {
				cout << "(node1, node2, weight,  AM size till now, edges till now, mem) = " << u1 << ", " << u2 << ", " << parameter1 << ", " << AM_out->size() << " + " << AM_in->size() << ", " << edges << ", " << getCurrentMemoryUsage() << endl;
			}
				
			// add edges into the adjacency matrix (*** out neighbours)!
			FriendsMap *neighbors = AM_out->find(u1);
			if (neighbors == NULL) {
				neighbors = new FriendsMap();
				neighbors->insert(std::make_pair(u2, parameter1));
				AM_out->insert(u1, neighbors);
			} else {
				FriendsMap::iterator it = neighbors->find(u2);
				if (it == neighbors->end()) {
					neighbors->insert(std::make_pair(u2, parameter1));
				} else {
				cout << "WARNING: Edge redundant between users " << u1 << " and " << u2 << endl;
				}
			}
			
			// add edges into the adjacencty matrix (*** in neighbours)!
			FriendsMap *inNeighbors = AM_in->find(u2);
			if (inNeighbors == NULL) {
				inNeighbors = new FriendsMap();
				inNeighbors->insert(std::make_pair(u1, parameter1));
				AM_in->insert(u2, inNeighbors);
			} else {
				FriendsMap::iterator it = inNeighbors->find(u1);
				if (it == inNeighbors->end()) {
					inNeighbors->insert(std::make_pair(u1, parameter1));
				} else {
				cout << "WARNING: Edge redundant between users " << u1 << " and " << u2 << endl;
				}
			}
			
			// also add the edges u2->u1 but done allocate Edge class to them
			// .. it is just to find friends efficiently
			/*
			if (graphType == UNDIRECTED) { 
				neighbors = AM_out->find(u2);
				if (neighbors == NULL) {
					neighbors = new FriendsMap();
					neighbors->insert(pair<UID, float>(u1, parameter1 ));
					AM_out->insert(u2, neighbors);
				} else {
					FriendsMap::iterator it = neighbors->find(u1);
					if (it == neighbors->end()) {
						neighbors->insert(pair<UID, float>(u1, parameter1 ));
					} else {
						//	cout << "WARNING: Edge redundant between users " << u1 << " and " << u2 << endl;
					}
				}
			}*/
		}
	
		myfile.close();
	
	} else {
			cout << "Can't open friendship graph file " << probGraphFile << endl;
	}

	cout << "BuildAdjacencyMatFromFile done" << endl;
	cout << "Size of friendship graph hashtree is : " << AM_out->size() << endl;
	cout << "Number of users are: " << users.size() << endl;
	cout << "Number of edges in the friendship graph are: " << edges << endl;

}


/*
void LDAG::writeCovInFile(UserList& curSeedSet, float tol, MC* mc) {

	cout << "In writeCovInFile" << endl;

	string filename = opt->getValue("outdir") + string("/LTCov_") + m + "_" + floatToStr(tol) + ".txt" ;
	ofstream outFile (filename.c_str(), ios::out);
	
	UserList S;
	for(UserList::iterator i=curSeedSet.begin(); i!=curSeedSet.end(); ++i) {
		UID v = *i; 
		S.insert(v);

		if (S.size() % 25 != 0) continue;
		float cov = mc->LTCov(S);
		outFile << S.size() << " " << v << " " << cov << endl;
		
	}

	outFile.close();

	cout << "Done writeCovInFile" << endl;
}
*/

void LDAG::clearAll() {
	
	// covBestNode.clear();
	curSeedSet.clear();
	//seedSetNeighbors.clear();
	totalCov = 0;
	LDAGNodes.clear();
	infSets.clear();
	//alphaMap.clear();
	//apMap.clear();
	MGHeap.clear();
	revMGHeap.clear();

}




float LDAG::getTime() const {
	
	//time_t curTime;
	//time(&curTime);
	clock_t curTime = clock();

	float min = ((float)(curTime - startTime) / CLOCKS_PER_SEC)/60;

	//float min = ((float)(curTime - startTime))/60;
	return min;

}


void LDAG::openOutputFile() {
	
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
        string statfilename = outdir + "/" + "stats_" + m + ".txt";
        string seedfilename = outdir + "/" + "seeds_" + m +  ".txt";
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


void LDAG::writeOutputFile(UID v, float cov, float marginal_gain, int curTimeStep) {

	seedFile<<v-1<<endl;
        int size = curSeedSet.size();
        if(size==1 || size==2 || size==5||size==10||(size%25==0&& size>10)){
                statFile<<size<<" "<<cov<<" "<<getTime()<<" "<<getCurrentMemoryUsage()<<endl;
        }

}




/*
// to find edge weight w(u,v)
float LDAG::weight(UID u, UID v) {
	
	FriendsMap *neighbors = AM_out->find(u);
	if (neighbors == NULL) {
		cout << "node " << u << " does not have any outgoing edges!" << endl;
		return (float) 0; // node u is not found
	} else {
		FriendsMap::iterator it = neighbors->find(v);
		if (it == neighbors->end()) {
			cout << "edge: " << u << "," << v << " does not exist!" << endl;
			return (float) 0; // edge (u,v) is not found
		} else {
			cout << "edge: " << u << "," << v << " does not exist!" << endl;
			return it->second;
		}
	}

} */



}

