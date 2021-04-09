#include "IMRank.h"
#include "graph.h"
#include "heap.h"
#include "general_cascade.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <vector>

int imrank::n = 0;
int imrank::top = 0;
double imrank::d[MAX_K];
int imrank::list[MAX_K];
char imrank::file[] = "imrank.txt";
RankNode* imrank::rankNodes;
bool imrank::debug = false;

vector<int> imrank::dd(MAX_NODE,0);
vector<double> imrank::dp(MAX_NODE,1.0);
vector<Edge> imrank::edges;
vector<int> imrank::correspond(MAX_EDGE,0);
vector<int> imrank::index(MAX_NODE,0);
vector<bool> imrank::visited(MAX_NODE, false);
vector<double> imrank::pp(MAX_NODE,0);
long imrank::timer;
//vector<RankNode> imrank::rankNodes(MAX_NODE,NULL);


int imrank::GetMax(int round)
{
	double max = -1000000.0;
	int mp = -1;
	for (int j=0; j<n; j++)
	{
		double tmp = dp[j];
		if (tmp >max)
		{
			max = tmp;
			mp = j;
		}
	}
	return mp;

}

void imrank::constructEdges(){
	int m = Graph::GetM();
	int n = Graph::GetN();
	edges.resize(m);
	correspond.resize(m);


	//inverse all the edges
	for(int i=0; i<m; i++){
		edges[i].u = Graph::edges[i].v;
		edges[i].v = Graph::edges[i].u;
		edges[i].w1 = Graph::edges[i].w1;
		edges[i].w2 = Graph::edges[i].w2;
		edges[i].c = Graph::edges[i].c;
		correspond[i] = Graph::correspond[i];
	}

	qsort_edges(0, m-1);

	index.resize(n);
	for (int i=0; i<n; i++)
		index[i] = 0;
	for (int i=0; i<m; i++)
		index[edges[i].u] = i;
	for (int i=1; i<n; i++)
		if (index[i] < index[i-1])
			index[i] = index[i-1];

}


void imrank::qsort_edges(int h, int t)
{
	if (h<t) 
	{
		int  i = h, j = t;
		Edge mid = edges[(i+j)/2];
		int midc = correspond[(i+j)/2];
		edges[(i+j)/2] = edges[i];
		if (midc != i)
		{
			correspond[correspond[i]] = (i+j)/2;
			correspond[(i+j)/2] = correspond[i];
		}
		else
			midc = (i+j)/2;

		while (i<j) 
		{
			while ((i<j) && ((edges[j].u>mid.u)||((edges[j].u==mid.u)&&(edges[j].v>mid.v))))
				j--;
			if (i<j) {
				edges[i] = edges[j];
				if (j!=midc)
				{
					correspond[i] = correspond[j];
					correspond[correspond[j]] = i;
				}
				else 
					midc = i;
				i++;
			}
			while ((i<j) && ((edges[i].u<mid.u)||((edges[i].u==mid.u)&&(edges[i].v<mid.v))))
				i++;
			if (i<j) {
				edges[j] = edges[i];
				if (i!=midc) {
					correspond[j] = correspond[i];
					correspond[correspond[i]] = j;
				}
				else
					midc = j;
				j--;
			}
		}

		edges[i] = mid;
		correspond[i] = midc;
		correspond[midc] = i;
		qsort_edges(h, i-1);
		qsort_edges(i+1, t);
	}
}


int imrank::GetLeaders(int node)
{
	if (node == 0)
		return index[node]+1;
	else 
		return index[node]-index[node-1];
}


double imrank::Build(char *initialRank, int l)
{
	constructEdges();
	clock_t start_time, end_time;
	start_time = clock();
	end_time = clock();
	n = Graph::GetN();	
	dp.resize(n);
	dd.resize(n);

	rankNodes = (RankNode*)malloc(sizeof(RankNode)*n);

	int* rankIDs = (int*)malloc(sizeof(int)*n);
    for(int i=0; i<n; i++){
	    rankIDs[i] = i;
		dd[i] = GetLeaders(i);
	}
	int* oldRank = (int*)malloc(sizeof(int)*n);
	double* oldScores = (double*)malloc(sizeof(double)*n);
	for(int i=0; i<n; i++){
		oldRank[i] = 100;
		oldScores[i] = 1;
		rankNodes[i].rank = -1000;
	}

	int set[SET_SIZE];
	InitialRank(initialRank, rankNodes);

	int round = 1;
	do{
		qsort(rankIDs, n, sizeof(int), Compare);

		bool diff_rank_top_k = true;
		 
		for(int o=n-SET_SIZE; o<n; o++){

			if((n-o)!=oldRank[rankIDs[o]])
			{
				diff_rank_top_k = false;
				break;
			}
		}

		double rank_modify_count = 0.0;
		for(int o=n-SET_SIZE; o<n; o++){
			if((n-o)!=oldRank[rankIDs[o]])
			{
				rank_modify_count++;
			}
		}

		bool diff_top_k = true;
		for(int o=n-SET_SIZE; o<n; o++){
			if(abs(oldRank[rankIDs[o]]) > SET_SIZE){
				diff_top_k = false;
				break;
			}
		}
		//EDITED AS PER REQUEST FROM AUTHOR
		if( round == IMRank_LOOP){
		//if(diff_top_k || round == IMRank_LOOP){
		//if(diff_top_k){
		//if(round > 20){
			end_time = clock();
			timer = end_time - start_time;
				for(int j=1; j<SET_SIZE+1; j++)
				{
					list[j-1] = rankIDs[n-j];
				}
			break;
		}

		//set the rank of the node, and reset the score of each node
		for(int i=0; i<n; i++){
			oldRank[rankIDs[i]] = n-i;
			rankNodes[rankIDs[i]].rank = n-i;
			oldScores[rankIDs[i]] = rankNodes[rankIDs[i]].score;
			rankNodes[rankIDs[i]].score = 1.0;
		}
		round++;
		DeliverCertainLeaders(n, l, rankIDs, rankNodes);

	}while(true);
	//********
	//free(oldScores);
	//free(oldRank);
	//free(rankIDs);

	timer = (double)(end_time-start_time);
	return 1;
}

void imrank::DeliverCertainLeaders(int n, int lorder, int *rankIDs, RankNode *rankNodes){
	//int leadersNum = 0;
	RankLeaders* tempLeaders = (RankLeaders*)malloc(sizeof(RankLeaders)*n);
	int* state = (int*)malloc(sizeof(int)*n);
	//int* index = (int*)malloc(sizeof(int)*n);
	for(int i=0; i<n; i++){
		state[i] = -1;//unselected
		//index[i] = -1;
	}
	
	for (int i=0; i<n-1; i++)
	{
		state[n-1] = 0;
		int nodeid = rankIDs[i];
		DeepSearch(n, rankNodes, nodeid, nodeid, 1, 0, tempLeaders, state, lorder);
		int c = state[n-1];
		//sort and allocate score
		if(c>0){
				qsort(tempLeaders, c, sizeof(RankLeaders), Compare2);
				double send;
				for(int j=1; j<=c; j++){
					send = rankNodes[nodeid].score * tempLeaders[c-j].p;
					rankNodes[tempLeaders[c-j].id].score += send;
					rankNodes[nodeid].score -= send;
					if(rankNodes[nodeid].score < EPS) continue;
				}
			}

		int j=0;
		while(j<state[n-1]){
	    	state[tempLeaders[j].id] = -1;//unselected
			j++;
	    }
	}

	//free(tempLeaders);
	//free(state);
}

void imrank::DeepSearch(int n, RankNode *rankNodes, int seedid, int nodeid, double current_p, int current_l, RankLeaders* tempLeaders, int* state, int lorder){
    
	int c = state[n-1];
	if(rankNodes[seedid].rank > rankNodes[nodeid].rank){
		int truec = c;
		if(state[nodeid]!=-1){
			int truec = state[nodeid];
			//tempLeaders[truec].id = nodeid;
			tempLeaders[truec].p = tempLeaders[truec].p+current_p;
		}else{
	    	tempLeaders[c].id = nodeid;
	        tempLeaders[c].p = current_p;
			state[n-1]++;//c
			state[nodeid] = c;
		}
	}else{
		if(current_l==lorder)
			return;
		if(current_l!=0 && seedid==nodeid)
			return;
		int begin = -1;
		if(nodeid>0)
		begin = index[nodeid-1];

		int edgeIndex;
		int trueLeaderID;

	    for (int j=0;j<dd[nodeid];j++){
	    	edgeIndex = begin+1+j;
	    	trueLeaderID = edges[edgeIndex].v;
	    	double p = exp(-edges[edgeIndex].w1);
	    	if(p < EPS) continue;//0
	     	DeepSearch(n, rankNodes, seedid, trueLeaderID, current_p*p, current_l+1, tempLeaders, state, lorder);
	    }
	}
}

void imrank::InitialRank(char *initialRank, RankNode *rankNodes){

	if(strcmp(initialRank,"Degree")==0){//score = outDegree
        for(int i=0; i<n; i++)
		   rankNodes[i].score = Graph::GetOutDegree(i);
	}else if(strcmp(initialRank,"DegreeInverse")==0){
		for(int i=0; i<n; i++)
			rankNodes[i].score = (double)(1.0/Graph::GetOutDegree(i));
	}
	else if(strcmp(initialRank,"WeightedDegree")==0){//score = weighted degree
	   for(int i=0; i<n; i++){
		   rankNodes[i].score = 0;
		   int degree = Graph::GetNeighbor(i);
		   int begin = -1;
		   if(i>0)
			  begin = Graph::index[i-1];
		   for(int j=0; j<degree; j++){
			   rankNodes[i].score += exp(-Graph::edges[begin+1+j].w1);
		   }
	   }
	}else if(strcmp(initialRank,"PageRank")==0){// pagerank
   	    vector<int> dd(n,0);
        vector<double> dp(n,0);
    	//vector<double> newdp(n,0);
    	vector<double> self(n,0.0);
		double dampen = 0.15;
     	int i=0;
	    for (i=0; i<n; i++)
    	{
	    	dp[i]=1.0;
     		dd[i]=Graph::GetNeighbor(i);
    		for (int j=0;j<dd[i];j++)
	   		self[i]+=exp(-Graph::GetEdge(i,j).w2);
    	}
    	double theta=1e-3;				
    	double delta;
    	long count=0;
    	do
    	{
	    	for (i=0;i<n;i++)
		    	rankNodes[i].score=0;
    		for (i=0;i<n;i++)
	  		for (int j=0;j<dd[i];j++)
			{
				Edge e=Graph::GetEdge(i,j);
				rankNodes[e.v].score+=dp[i]*exp(-e.w2)/self[i];
			}
	    	delta=0;
	    	for (i=0;i<n;i++)
	    	{
		    	delta+=fabs(dampen+(1-dampen)*rankNodes[i].score-dp[i]);
		    	dp[i]=dampen+(1-dampen)*rankNodes[i].score;
	    	}		
    	} while (delta>theta &&++count<10000);

	}else if(strcmp(initialRank,"Random")==0){
		for(int i = 0; i<n; i++)
			rankNodes[i].score = i;
		double tmp;
		int t;
		for(int i=0; i<n; i++)
		{	
			t = rand()%n;
			tmp = rankNodes[i].score;
			rankNodes[i].score = rankNodes[t].score;
			rankNodes[t].score = tmp;
		}
	}else{
		printf("wrong setting\n");
	}
}

int imrank::Compare(const void * id1, const void * id2)
{
    double diff = rankNodes[*(int*)id1].score - rankNodes[*(int*)id2].score;
	if(diff > 0) 
		return 1;
	else if(diff <0)
		return -1;
	else
		return 0;
}

int imrank::Compare2(const void * tempLeader1, const void * tempLeader2)
{
	return -(rankNodes[(*(RankLeaders*)tempLeader1).id].rank - rankNodes[(*(RankLeaders*)tempLeader2).id].rank);
}


Edge imrank::GetEdge(int minNode,int j){
    if (minNode == 0)
		return edges[j];
	else
		return edges[index[minNode-1]+1+j];
}

int imrank::GetNeighbor(int node)
{
	if (node == 0)
		return index[node]+1;
	else 
		return index[node]-index[node-1];
}

int imrank::Compare3(const void * tempSecondLeader1, const void * tempSecondLeader2)
{
	return -(rankNodes[(*(RankSecondLeaders*)tempSecondLeader1).id].rank - rankNodes[(*(RankSecondLeaders*)tempSecondLeader2).id].rank);
}


int imrank::GetNode(int i)
{
	return list[i];
}
