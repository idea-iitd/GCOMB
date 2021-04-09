#include "staticgreedy_directed_new.h"
#include "limit.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <time.h>

int StaticGreedyDirectedNew::n = 0;
int StaticGreedyDirectedNew::m = 0;
bool StaticGreedyDirectedNew::flag_forward[MAX_EDGE];
int StaticGreedyDirectedNew::seed[MAX_NODE];
int StaticGreedyDirectedNew::index[MAX_NODE];
bool StaticGreedyDirectedNew::finish[MAX_NODE];
bool StaticGreedyDirectedNew::flag[MAX_NODE];
bool StaticGreedyDirectedNew::update[MAX_NODE]; 
int  StaticGreedyDirectedNew::TempRecord_reach[MAX_NODE];
bool  StaticGreedyDirectedNew::flag_temp[MAX_NODE];
int  StaticGreedyDirectedNew::RecordID_reach[MAX_NODE];
int  StaticGreedyDirectedNew::TempRecord_upstream[MAX_NODE];
int  StaticGreedyDirectedNew::RecordID_upstream[MAX_NODE];
int  StaticGreedyDirectedNew::otherupstream_id[MAX_NODE];

NewSnapshot* StaticGreedyDirectedNew::newsnapshots;

int* StaticGreedyDirectedNew::GetSeedsForWICUNDirectedG(int R, int k)
{

	int* seeds = (int*)malloc(sizeof(int)*k);
	n = Graph::GetN();
	int m = Graph::GetM();
	int i = 0;

	int scores[MAX_NODE] = {0};

	newsnapshots = (NewSnapshot*) malloc(sizeof(NewSnapshot)*R);

	//Generate R Snapshots
	bool forward[MAX_EDGE] = {false};
	bool reverse[MAX_EDGE] = {false};
	//int* corresponding_temp = (int*)malloc(sizeof(int)*m);//record all the new location
	int* TempRecord = (int*)malloc(sizeof(int)*n);
	int* index_temp = (int*)malloc(sizeof(int)*n);
	int* corresponding_temp = (int*)malloc(sizeof(int)*m);//record all the new location

	for (int yy=0; yy<R; yy++)
	{
			//printf("generate %d\n", yy);

			int i = 0;

			//for directed networks
			int count_mm = 0;
			for (i=0; i<m; i++)
		    	{
					//if(Graph::edges[i].w1<15000){
						forward[i] = false;
						reverse[Graph::correspond[i]] = false;

		         		double temp_p = 0;
							//if ((double)rand()/(double)RAND_MAX < exp(-Graph::edges[i].w1))
							if ((double)rand()/(double)RAND_MAX < Graph::edges[i].w1)
			        		{
								forward[i] = true;
		        				reverse[Graph::correspond[i]] = true;
								//printf("test: %d  %d  %d  %d\n", forward[i], reverse[i], true, false);
								count_mm++;
			        		}
		         	//}
	     	 }

			
			int* corresponding_true = (int*)malloc(sizeof(int)*m);//record the final correspond
			for(int aa=0; aa<m; aa++){
				corresponding_temp[aa] = -1;
				corresponding_true[aa] = 0;
			}
			
			count_mm = 2*count_mm;
			RandEdge* myedges_temp = (RandEdge*)malloc(sizeof(RandEdge)*count_mm);

			int mm=0;
			for (int vv=0; vv<m; vv++){
				
				//if (edges[vv].forward || edges[vv].reverse)
				if (forward[vv] || reverse[vv])
				{
					myedges_temp[mm].e = &Graph::edges[vv];
					myedges_temp[mm].forward = forward[vv];
					myedges_temp[mm].reverse = reverse[vv];
					//myEdges[mm] = edges[vv];
					corresponding_temp[vv] = mm;
				    //printf("vv (%d): edge: u = %d, v = %d, forward = %d, reverse = %d\n", vv, Graph::edges[vv].u, Graph::edges[vv].v, myedges_temp[mm].forward, myedges_temp[mm].reverse);
				
					mm++;
					
				}
			}

			int cc = 0;
			for(int vv=0; vv<m; vv++){
				if(corresponding_temp[vv]>-1){
					corresponding_true[cc] = corresponding_temp[Graph::correspond[vv]];
					cc++;
				}
			}

			for (i=0; i<n; i++)
					index_temp[i] = -1;
			for (i=0; i<mm; i++)
					index_temp[(*myedges_temp[i].e).u] = i;
			for (i=0; i<n; i++)
				if (i!=0)
				{
					if (index_temp[i] < 0)
					    index_temp[i] = index_temp[i-1];
				}

			//caculate the reachability of every node
			for(i=0; i<n; i++){
				flag[i]=false;
			}

			int* reach_temp = (int*)malloc(sizeof(int)*n);
			ComplexNode* complexNode = (ComplexNode*)malloc(sizeof(ComplexNode)*n);

			//get downstream nodes
			for(i=0; i<n; i++){
				reach_temp[i] = 0;
				//printf("caculte %d\n", i);
				GetReachability(i,i, myedges_temp, index_temp,  reach_temp, TempRecord, flag);
				int* downstreamNodes = (int*)malloc(sizeof(int)*(reach_temp[i]+1));

				//update node flag
				downstreamNodes[0] = reach_temp[i];
			

				for(int l=0; l<reach_temp[i]; l++){
					downstreamNodes[l+1] = TempRecord[l];
					flag[TempRecord[l]] = false;
				
				}
				scores[i] += reach_temp[i];
				complexNode[i].downstreamNodes = downstreamNodes;
			}
			
			
			//printf("Caculate all the downstream nodes\n");

			//get upstream nodes
			

			for(int xx=0; xx<n; xx++){
				reach_temp[xx] = 0;
				GetUpStreamNodes(xx, xx, myedges_temp, index_temp,  reach_temp, TempRecord, flag);
				int* upstreamNodes = (int*)malloc(sizeof(int)*(reach_temp[xx]+1));

				//update node flag
				upstreamNodes[0] = reach_temp[xx];

				for(int l=0; l<reach_temp[xx]; l++){
					upstreamNodes[l+1] = TempRecord[l];
					flag[TempRecord[l]] = false;
			
				}
			
				complexNode[xx].upstreamNodes = upstreamNodes;
			}

			//printf("Caculate all the downstream nodes\n");

			newsnapshots[yy].complexNodes = complexNode;
			newsnapshots[yy].flag_del = (bool*)malloc(sizeof(bool)*n);
			memset(newsnapshots[yy].flag_del,false,sizeof(bool)*n);
			//printf("New a snapshots\n");
			free(myedges_temp);
		}


		free(TempRecord);
		//free(corresponding_temp);
		free(index_temp);

		//select seeds
		double aveScore = 0;
		for(int ss=0; ss<k; ss++){
			//select the node with largest scores
        	int max_score = 0;
        	int max_score_node = 0;

         	for(int h=0; h<n; h++){
     	    	if(max_score < scores[h]){
    		    	max_score = scores[h];
	     	    	max_score_node = h;
                }
          	}
			aveScore += max_score;
	    	seeds[ss] = max_score_node;
			//if(debug)
	    	    //printf("max_score_node: %d  \n", max_score_node);
			if(ss==(k-1))
				break;
			//update scores
			
			for(int bb=0; bb<R; bb++){
				UpdateScore(max_score_node, newsnapshots[bb], scores);
			}
	  	}
		//clock_t time_start = clock();
		for(int nn=0; nn<R; nn++){
			free(newsnapshots[nn].complexNodes);
			free(newsnapshots[nn].flag_del);
		}
		free(newsnapshots);
		return seeds;
}

int* StaticGreedyDirectedNew::GetSeedsForDirectedG(int R, int k)
{

	int* seeds = (int*)malloc(sizeof(int)*k);

	n = Graph::GetN();
	int m = Graph::GetM();
	int i = 0;

	int scores[MAX_NODE] = {0};

	newsnapshots = (NewSnapshot*) malloc(sizeof(NewSnapshot)*R);

	//Generate R Snapshots
	bool forward[MAX_EDGE] = {false};
	bool reverse[MAX_EDGE] = {false};
	//int* corresponding_temp = (int*)malloc(sizeof(int)*m);//record all the new location
	int* TempRecord = (int*)malloc(sizeof(int)*n);
	int* index_temp = (int*)malloc(sizeof(int)*n);
	int* corresponding_temp = (int*)malloc(sizeof(int)*m);//record all the new location
	RandEdge* myedges_temp;
	for (int yy=0; yy<R; yy++)
	{
		//if(debug)
			//printf("generate %d\n", yy);

			int i = 0;

			//for directed networks
			int count_mm = 0;
			for (i=0; i<m; i++)
		    	{
					if(Graph::edges[i].w1 > -1.0){
						forward[i] = false;
						reverse[Graph::correspond[i]] = false;

		         		double temp_p = 0;
							//if ((double)rand()/(double)RAND_MAX < exp(-Graph::edges[i].w1))
							if ((double)rand()/(double)RAND_MAX < Graph::edges[i].w1)
			        		{
								forward[i] = true;
		        				reverse[Graph::correspond[i]] = true;
								//printf("test: %d  %d  %d  %d\n", forward[i], reverse[i], true, false);
								count_mm++;
			        		}
		         	}
	     	 }

			
			int* corresponding_true = (int*)malloc(sizeof(int)*m);//record the final correspond
			for(int aa=0; aa<m; aa++){
				corresponding_temp[aa] = -1;
				corresponding_true[aa] = 0;
			}
			
			count_mm = 2*count_mm;
			myedges_temp = (RandEdge*)malloc(sizeof(RandEdge)*count_mm);

			int mm=0;
			for (int vv=0; vv<m; vv++){
				
				//if (edges[vv].forward || edges[vv].reverse)
				if (forward[vv] || reverse[vv])
				{
					myedges_temp[mm].e = &Graph::edges[vv];
					myedges_temp[mm].forward = forward[vv];
					myedges_temp[mm].reverse = reverse[vv];
					//myEdges[mm] = edges[vv];
					corresponding_temp[vv] = mm;
				
					mm++;
					
				}
			}

			int cc = 0;
			for(int vv=0; vv<m; vv++){
				if(corresponding_temp[vv]>-1){
					corresponding_true[cc] = corresponding_temp[Graph::correspond[vv]];
					cc++;
				}
			}

			for (i=0; i<n; i++)
					index_temp[i] = -1;
			for (i=0; i<mm; i++)
					index_temp[(*myedges_temp[i].e).u] = i;
			for (i=0; i<n; i++)
				if (i!=0)
				{
					if (index_temp[i] < 0)
					    index_temp[i] = index_temp[i-1];
				}

			//caculate the reachability of every node
			for(i=0; i<n; i++){
				flag[i]=false;
			}

			int* reach_temp = (int*)malloc(sizeof(int)*n);
			ComplexNode* complexNode = (ComplexNode*)malloc(sizeof(ComplexNode)*n);

			//get downstream nodes
			for(i=0; i<n; i++){
				reach_temp[i] = 0;
				//printf("caculte %d\n", i);
				GetReachability(i,i, myedges_temp, index_temp,  reach_temp, TempRecord, flag);
				int* downstreamNodes = (int*)malloc(sizeof(int)*(reach_temp[i]+1));

				//update node flag
				downstreamNodes[0] = reach_temp[i];
				
				for(int l=0; l<reach_temp[i]; l++){
					downstreamNodes[l+1] = TempRecord[l];
					flag[TempRecord[l]] = false;
				
				}
			
				scores[i] += reach_temp[i];
				complexNode[i].downstreamNodes = downstreamNodes;
			}
			
			
			//printf("Caculate all the downstream nodes\n");

			//get upstream nodes
			

			for(int xx=0; xx<n; xx++){
				reach_temp[xx] = 0;
				GetUpStreamNodes(xx, xx, myedges_temp, index_temp,  reach_temp, TempRecord, flag);
				int* upstreamNodes = (int*)malloc(sizeof(int)*(reach_temp[xx]+1));

				//update node flag
				upstreamNodes[0] = reach_temp[xx];

				for(int l=0; l<reach_temp[xx]; l++){
					upstreamNodes[l+1] = TempRecord[l];
					flag[TempRecord[l]] = false;
					 
				}
		
				complexNode[xx].upstreamNodes = upstreamNodes;
			}

			//printf("Caculate all the downstream nodes\n");

			newsnapshots[yy].complexNodes = complexNode;
			newsnapshots[yy].flag_del = (bool*)malloc(sizeof(bool)*n);
			memset(newsnapshots[yy].flag_del,false,sizeof(bool)*n);
			//printf("New a snapshots\n");
			free(myedges_temp);
		}


		free(TempRecord);
		free(corresponding_temp);
		free(index_temp);

		//select seeds
		double aveScore = 0;
		for(int ss=0; ss<k; ss++){
			//select the node with largest scores
        	int max_score = 0;
        	int max_score_node = 0;

         	for(int h=0; h<n; h++){
     	    	if(max_score < scores[h]){
    		    	max_score = scores[h];
	     	    	max_score_node = h;
                }
          	}
			aveScore += max_score;
	    	seeds[ss] = max_score_node;
			//if(debug)
	    	//printf("%d-th max_score_node: %d  \n", ss, max_score_node);
			if(ss==(k-1))
				break;
			//update scores
			
			for(int bb=0; bb<R; bb++){
			
				UpdateScore(max_score_node, newsnapshots[bb], scores);
			}

	  	}
		//clock_t time_start = clock();
		for(int nn=0; nn<R; nn++){
			free(newsnapshots[nn].complexNodes);
			free(newsnapshots[nn].flag_del);
		}
		free(newsnapshots);
	
		return seeds;
}

void StaticGreedyDirectedNew::UpdateScore(int seed, NewSnapshot newSnapshot, int* scores){
	
    //find the upstream nodes of the seed, cut one score
	//printf("delete the seed : %d\n", seed);
	int number_down =  newSnapshot.complexNodes[seed].downstreamNodes[0];

	for(int i=1; i<=number_down; i++){
		if(newSnapshot.flag_del[newSnapshot.complexNodes[seed].downstreamNodes[i]])
			continue;

		DeleteNode(newSnapshot.complexNodes[seed].downstreamNodes[i], newSnapshot, scores);
	}

	for(int i=1; i<=number_down; i++){
    	newSnapshot.flag_del[newSnapshot.complexNodes[seed].downstreamNodes[i]] = true;
	}

}


void StaticGreedyDirectedNew::DeleteNode(int nodeid, NewSnapshot newSnapshot, int* scores){


	ComplexNode node = newSnapshot.complexNodes[nodeid];

	//all the upstream nodes cut one, and update their downstream nodes
	int number_up = node.upstreamNodes[0];
	int upnode;
	int count=0;

	for(int i=1; i<=number_up; i++)
	{
		upnode = node.upstreamNodes[i];
		if(newSnapshot.flag_del[upnode])
			continue;
		scores[upnode]--;
	}

	//delete its score
	//scores[nodeid] -= node.downstreamNodes[0];
	// printf("cut the score of node %d  to %d\n", nodeid, scores[nodeid]);
	
}


void StaticGreedyDirectedNew::GetUpStreamNodes(int node, int present, RandEdge* myedges_temp, int* index, int* upstreams_temp, int* TempRecord, bool* flag_temp){

	//printf("%d-->%d\n",node, present);
	int a = 0;
	if (present != 0)
		a = index[present-1]+1;
	flag_temp[present] = true;
	int temp = upstreams_temp[node];
	//printf("node is %d,temp is %d\n", node, temp);
	TempRecord[temp]=present;
	upstreams_temp[node]++;
	//printf("check nei: %d\t%d\n",a,index[present]);
	for (int i=a; i<=index[present]; i++)
		if (myedges_temp[i].reverse){
			//printf("%d can reverse to %d and this node is %d\n",myedges_temp[i].e.u, myedges_temp[i].e.v, flag[myedges_temp[i].e.v]);
			if (!flag_temp[(*myedges_temp[i].e).v])
			{
				//flag[myEdges[i].e.v] = true;
				GetUpStreamNodes(node, (*myedges_temp[i].e).v, myedges_temp, index, upstreams_temp, TempRecord, flag_temp);
			}
		}
}

void StaticGreedyDirectedNew::GetReachability(int node, int present,RandEdge* myedges_temp, int* index_temp, int* reach_temp, int* TempRecord, bool* flag){


	int a = 0;
	if (present != 0)
		a = index_temp[present-1]+1;
	//printf("%d-->%d  a=%d\n",node, present, a);
	flag[present] = true;
	int temp = reach_temp[node];
	TempRecord[temp]=present;
	reach_temp[node]++;
	//printf("a=%d\n",a);
	for (int i=a; i<=index_temp[present]; i++)
		if (myedges_temp[i].forward)
			if (!flag[(*myedges_temp[i].e).v])
			{
				//flag[myEdges[i].e.v] = true;
				GetReachability(node, (*myedges_temp[i].e).v, myedges_temp, index_temp, reach_temp, TempRecord, flag);
			}
}
