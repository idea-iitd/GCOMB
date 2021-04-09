#include <iostream>                  // for std::cout
#include <fstream>
#include <utility>                   // for std::pair
#include <algorithm>                 // for std::for_each
#include <sys/time.h>
//#include <boost/graph/graph_traits.hpp>
//#include <boost/graph/adjacency_list.hpp>
#include <cassert>
#include <vector>
#include <list>
#include <unordered_map>
#include <math.h>
#include<stdlib.h>
#include <ios>
#include <unistd.h>
using namespace std;

//#define DEBUG
//#define PRINTS
#define MIN(a,b) a>b?b:a
#define MAX(a,b) a>b?a:b
fstream detailedOpFile, statOpFile, spreadOpFile;
bool myfunction(double i, double j){return (i<j);}
int gType=1; /*********** 1 for DIRECTED and 0 for UNDIRECTED ***************/
int model=0; /*********** 0 for IC, 1 for WC, 2 for LT ***************/

/*
namespace std {
        template <>
        struct hash<std::pair<unsigned   int, unsigned   int> >{
        public:
                size_t operator()(std::pair<unsigned   int, unsigned   int> x) const throw() {
                   //  std::hash<boost::graph_traits<boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, vertex_props>>::vertex_descriptor> v_hash;
                   // size_t h1 = v_hash(x.first);//something with x   
                   // size_t h2 = v_hash(x.second);//something with x   
                   std::size_t seed = 0;
                   boost::hash_combine(seed, x.first);
                   boost::hash_combine(seed, x.second);
                   return seed;
        }
};
}
*/

class vertex_props{
	public:
		unsigned int id; 
		//int degree;
		bool covered;
		float score_old;
		float score_new;
};

class Timer {
	private:
		struct timeval* _s; 
	public:
		Timer();
		~Timer();
		void start();
		double stop();
};
Timer::Timer() : _s(0) {
}
//--------------------------------------------------------------------------------------------------
Timer::~Timer() {
	if (_s) delete _s;
}
//--------------------------------------------------------------------------------------------------
void Timer::start() {
	assert(!_s);
	_s = new timeval();
	gettimeofday(_s, 0);
}

double Timer::stop() {
	assert(_s);
	struct timeval* e = new timeval();
	gettimeofday(e, 0);
	long ms_s = _s->tv_sec * 1000000 + _s->tv_usec;
	long ms_e = e->tv_sec * 1000000 + e->tv_usec;
	double time = (ms_e - ms_s) / 1000000.0;
	delete e;
	delete _s;
	_s = 0;
	return time;
}

typedef pair<vertex_props, vector<unsigned int> > vertex;
typedef vector<vertex> Graph;

//////////////////////////////////////////////////////////////////////////////
//
// process_mem_usage(double &, double &) - takes two doubles by reference,
// attempts to read the system-dependent data for a process' virtual memory
// size and resident set size, and return the results in KB.
//
// On failure, returns 0.0, 0.0

void process_mem_usage(double& vm_usage, double& resident_set)
{
   using std::ios_base;
   using std::ifstream;
   using std::string;

   vm_usage     = 0.0;
   resident_set = 0.0;

   // 'file' stat seems to give the most reliable results
   //
   ifstream stat_stream("/proc/self/stat",ios_base::in);

   // dummy vars for leading entries in stat that we don't care about
   //
   string pid, comm, state, ppid, pgrp, session, tty_nr;
   string tpgid, flags, minflt, cminflt, majflt, cmajflt;
   string utime, stime, cutime, cstime, priority, nice;
   string O, itrealvalue, starttime;

   // the two fields we want
   //
   unsigned long vsize;
   long rss;

   stat_stream >> pid >> comm >> state >> ppid >> pgrp >> session >> tty_nr
               >> tpgid >> flags >> minflt >> cminflt >> majflt >> cmajflt
               >> utime >> stime >> cutime >> cstime >> priority >> nice
               >> O >> itrealvalue >> starttime >> vsize >> rss; // don't care about the rest

   stat_stream.close();

   long page_size_kb = sysconf(_SC_PAGE_SIZE) / 1024; // in case x86-64 is configured to use 2MB pages
   vm_usage     = vsize / 1024.0;
   resident_set = rss * page_size_kb;
}

void disp_mem_usage(string msg){
    double vm, rss;

    process_mem_usage(vm, rss);
    vm/=1024;
    rss/=1024;
    //cout<< "Memory Usage:" << msg << " vm:" << vm << " MB  rss:" << rss << " MB"<<endl;
    cout<< "Memory Usage:" << msg << " " << rss << " MB"<<endl;
}

double calculate_flow(Graph& h, vector<unsigned   int>& seed, float lambda ){
	int i=0;
	unordered_map<unsigned   int,bool> spread;
	Graph g=h;
	vector<unsigned   int> tempSeed=seed, neigh_list;
	float thresh;
	if(model==0){
		thresh=0.1;//IC
	}
	for(;i<tempSeed.size();i++){
		if(g[tempSeed[i]].first.covered ==false){
			neigh_list = g[tempSeed[i]].second;

			//it = neigh_list.begin();
			//while(it!=neigh_list.end()){
			for(int it=0;it<neigh_list.size();it++){
				if((double)rand()/RAND_MAX <= thresh && g[neigh_list[it]].first.covered==false){//added_now
				//if((((float)(rand() % 1001))/(float)1000) <= thresh && g[neigh_list[it]].first.covered==false){//added_now
				//if(g[it->first].first.covered==false){
					tempSeed.push_back(neigh_list[it]);
					if(spread.find(neigh_list[it])==spread.end()){
						spread[neigh_list[it]]=true;
						//cout<<spread[it->first]<<endl;
					}
				}
			}
			g[tempSeed[i]].first.covered=true;
		}
	}
	double spread_val=(double)spread.size();
	vector<unsigned int>().swap(tempSeed);
	unordered_map<unsigned int,bool>().swap(spread);
	return spread_val;
}
/*************************************************************************************/
double calculate_flow_update(Graph& g, vector<unsigned   int>& seed, float lambda ){
	int i=0;
	unordered_map<unsigned   int,bool> spread;
	vector<unsigned   int> tempSeed=seed, neigh_list;
	float thresh;
	if(model==0){
		thresh=0.1;//IC
	}
	for(;i<tempSeed.size();i++){
		if(g[tempSeed[i]].first.covered ==false){		
			neigh_list = g[tempSeed[i]].second;

			//it = neigh_list.begin();
			//while(it!=neigh_list.end()){
			for(int it=0;it<neigh_list.size();it++){
				if((double)rand()/RAND_MAX <= thresh && g[neigh_list[it]].first.covered==false){//added_now
				//if(g[it->first].first.covered==false){
					tempSeed.push_back(neigh_list[it]);
					if(spread.find(neigh_list[it])==spread.end()){
						spread[neigh_list[it]]=true;
						//cout<<spread[it->first]<<endl;
					}
				}
			}
			g[tempSeed[i]].first.covered=true;
		}
	}
	double spread_val=(double)spread.size();
	vector<unsigned int>().swap(tempSeed);
	unordered_map<unsigned int,bool>().swap(spread);
	return spread_val;
}

double kempe(Graph& g, int numSeeds, float lambda, vector<unsigned   int>& seed, int num_iter){
	int i=0;
	unsigned   int maxId;
	double diff,max,curFlow=0.0,spread=0.0,curTime=0.0;
	Timer tseed;
	cout<<"SeedNumber\t\tSeed-Id\t\tSpread\t\tTime"<<endl;
	detailedOpFile<<"SeedNumber\t\tSeed-Id\t\tSpread\t\tTime"<<endl;
	while(i<numSeeds){
		max=0;
		tseed.start();
		/*if(model==0||model==1){
		  curFlow=calculate_flow(g,seed,lambda);
		  }*/
		for(unsigned   int nodeId=0;nodeId<g.size();nodeId++){
			seed.push_back(nodeId);
			if(model==0||model==1){
				double new_flow = 0.0;
				for(int iter=0;iter<num_iter;iter++){
					new_flow+=calculate_flow(g,seed,lambda);
				}
				diff=new_flow*1.0/num_iter - curFlow;
			}
			//cout<<"The Spread and the NodeId respectively is:"<<diff<<","<<nodeId<<endl;
			seed.pop_back();
			if(diff>max){
				max=diff;
				maxId=nodeId;
			}
#ifdef DEBUG
			if(nodeId%1000==0){
				cout<<"[Kempe] Completed Iteration over "<<nodeId<<" nodes"<<endl;
			}
#endif

		}
		curFlow+=max;
		seed.push_back(maxId);
#ifdef PRINTS
		cout<<"[Kempe] Done Selecting Seed Number: "<<i<<endl;
#endif
		i++;
		curTime+=tseed.stop();
		//detailedOpFile<<"The spread with "<<i<<" seeds is: "<<curFlow<<" and time is: "<<curTime<<endl;
		//cout<<"The spread with "<<i<<" seeds is: "<<curFlow<<" and time is: "<<curTime<<endl;
		detailedOpFile<<i<<"\t\t"<<g[maxId].first.id<<"\t\t"<<curFlow<<"\t\t"<<curTime<<endl;
		cout<<i<<"\t\t"<<g[maxId].first.id<<"\t\t"<<curFlow<<"\t\t"<<curTime<<endl;
	}
	spread=curFlow;//max+
#ifdef PRINTS
	cout<<"[Kempe] The spread is: "<<spread<<endl;
#endif
	return spread;
}

unsigned   int assign_reachability_path_score(Graph &g, int depth)//call this with a new copy  of original graph but change covered in original graph only
	//initialise the score for each vertex to 1
	//maintain the node with the maximumm score and return it
{
	int i=0;
	unsigned int maxId;
	float prob;
	//Graph g=h;
	double maxScore=numeric_limits<double>::min();
	if(model==0){//IC
		prob=0.1;
	}
	for(;i<depth;i++){
		for(unsigned   int nodeId=0; nodeId<g.size(); nodeId++){
			if(g[nodeId].first.covered==false)
			{
				vector<unsigned int> neigh_list;
				/***************** Kuch thuka to change this ************************/
				g[nodeId].first.score_new=0;

				neigh_list = g[nodeId].second;
				//it = neigh_list.begin();
				//while(it!=neigh_list.end()){
				for(int it=0;it<neigh_list.size();it++){
					if(g[neigh_list[it]].first.covered==false  ){//initialisew the level to 0 and covered to 0
						if(i>0){
							g[nodeId].first.score_new += prob *(1.0+ g[neigh_list[it]].first.score_old);
						}
						else{
							g[nodeId].first.score_new += prob;
						}
					}
				}
				if((i==depth-1) && g[nodeId].first.score_new>maxScore){
				//if(g[nodeId].first.score_new>maxScore)
					maxId = nodeId;
					maxScore = g[nodeId].first.score_new;
				}
				/*if(g[nodeId].first.score>maxScore){
				  maxId = nodeId;
				  maxScore = g[nodeId].first.score;
				  }*/
			}
		}
		if(i<depth-1){
			for(unsigned   int nodeId=0; nodeId<g.size(); nodeId++){
				if(g[nodeId].first.covered==false){
					//g[nodeId].first.score_new += prob * g[nodeId].first.degree;
					g[nodeId].first.score_old = g[nodeId].first.score_new;
				}
			}
		}
	}
	return maxId;
}

double select_reachability_path_seed (Graph &h, int k, int num_seeds, vector<unsigned   int>& seed,float lambda)
{
	int i=0;
	double total_flow = 0.0;
	Graph g = h;
	unsigned   int maxId;
	for(;i<num_seeds;i++)
	{
		double curr_flow=0;
		maxId = assign_reachability_path_score(g, k);
		vector<unsigned   int> tmpSeed;
		tmpSeed.push_back(maxId);
		if(model==0||model==1){
			curr_flow = calculate_flow_update(g, tmpSeed, lambda);
		}
		seed.push_back(maxId);
		total_flow +=curr_flow;		
		#ifdef PRINTS
			cout<<"[Path] Done Selecting Seed Number: "<<i<<endl;
		#endif
	}
	#ifdef PRINTS
		cout<<"[Path] The spread is: "<<total_flow<<endl;
	#endif
	return total_flow;
}

unsigned int num_vertices(const Graph& g){
	unsigned int count =0;
	for(unsigned int i = 0;i<g.size();i++){
		//if(g[i].first.vsize!=0) count++;
		count++;
	}
	return count;
}
vector<double> processSpread(vector<double> &spreadVec, int BinSize){
    
    //std::sort(spreadVec.begin(),spreadVec.end());
BinSize = BinSize*spreadVec.size()/10000;
     vector<double> meanSpread;
    int numBins=100;
    for(int i=0;i<numBins;i++){
        double totalSpread=0.0;
        for(int j=0;j<BinSize;j++){
                int loc = rand()%spreadVec.size();
                totalSpread+=spreadVec[loc];

        }
        meanSpread.push_back(totalSpread*1.0/BinSize);
    }
    

    return meanSpread;
}
/************* Change parts of this function for Directed Graphs *********/
unsigned long int num_edges(const Graph& g){
	unsigned long int count =0;
	for(unsigned int i =0; i<g.size(); i++){
		count+=g[i].second.size();
	}
	if(gType==0){
		return (unsigned long int)count/2;
	}
	else{
		return (unsigned long  int)count;
		
	}
}	

/************* Change parts of this function for Directed Graphs *********/
int main(int argc,char *argv[]){
	//cout<<"The size of map is:"<<sizeof(map<unsigned int, double>)<<endl;
	if(argc!=9){
		cout<<"Usage:"<<endl;
		cout<<argv[0]<<" <path-to-graph-file> <seed-file>  <#num-seeds> <lambda> <#num-iterations> <#depth-for-reachability&path> <path-to-stat-file> <path-to-spreadFile>"<<endl;
		cout<<"Choice 0 -- Kempe"<<endl;
		cout<<"Choice 1 -- Reachability"<<endl;
		cout<<"Choice 2 -- Path"<<endl;
		
		cout<< "Choice for Model to be entered globablly"<<endl;
		cout<<"Choice 0 -- IC"<<endl;
		cout<<"Choice 1 -- WC"<<endl;
		cout<<"Choice 2 -- LT"<<endl;

		exit(1);
	}

	fstream inpFile,seedFile;
	float avg_deg=0.0;
	int ch=0;
    //atoi(argv[2]);
	int num_seeds=atoi(argv[3]);
	float lambda=atof(argv[4]);
	int num_iter=atoi(argv[5]),iter;
	int depth=atoi(argv[6]);
	
	vector<int> binSize;
	//binSize.push_back(1);binSize.push_back((int)(0.01*num_iter));binSize.push_back((int)(0.02*num_iter));binSize.push_back((int)(0.03*num_iter)),binSize.push_back((int)(0.04*num_iter));
	binSize.push_back(1);binSize.push_back(100);binSize.push_back(200);binSize.push_back(300),binSize.push_back(400);
	float decay_prob=0.1;

	inpFile.open(argv[1],ios::in);
	seedFile.open(argv[2],ios::in);
	string statFileName=argv[7], spreadFileName=argv[8];
	//detailedOpFile.open(argv[8],ios::out | ios_base::app);
	unsigned   int vert1,vert2,index=0,indexu,indexv;
	float prob;
	unordered_map<unsigned   int, unsigned   int> id2index;
	vector<unsigned   int> adj_list;
	Graph g;
	vertex_props u,v;
	vertex up,vp;
	srand(1991);
	//srand(time(NULL));
	cout<<"Reading File"<<endl;
	if(inpFile.is_open()){
		while(inpFile>>vert1>>vert2>>prob){
			adj_list.clear();
			if(id2index.find(vert1)==id2index.end()){
				u.id = vert1;
				u.score_old = 0;
				u.score_new = 0;
				u.covered = false;
				id2index[vert1] = index;
				up.first = u;
				up.second = adj_list;	
				//up.first.degree=0;//added_now
				g.push_back(up);
				indexu = index;
				index++;
			}
			else indexu = id2index[vert1];
			if(id2index.find(vert2)==id2index.end()){
				v.id = vert2;
				v.score_old = 0;
				v.score_new = 0;
				v.covered = false;
				id2index[vert2] = index;
				vp.first = v;
				//vp.first.degree=0;//added_now
				vp.second = adj_list;		
				g.push_back(vp);
				indexv = index;
				index++;
			}
			else indexv = id2index[vert2];
			//if((g[indexv].second.find(indexu)== g[indexv].second.end()) && (indexu!=indexv)){
			if(gType==0){ // Undirected
				//if((g[indexu].second.find(indexv)== g[indexu].second.end()) && (g[indexv].second.find(indexu)== g[indexv].second.end()) && (indexu!=indexv))
				if(indexu!=indexv){
					/*
					if(model==0){
						// **** thuka to change this back to ++ **** //
						//g[indexu].first.degree++;//added_now
						//g[indexv].first.degree++;//added_now
					}
					g[indexu].second[indexv] = true;
					g[indexv].second[indexu] = true;
					*/
					g[indexu].second.push_back(indexv);
					g[indexv].second.push_back(indexu);
				}
				//else if(!(g[indexu].second.find(indexv)!= adj_list.end() && (g[indexv].second.find(indexu)!= adj_list.end()))){
					//				cout<<"Serious Error in Edges!"; return 0;
				//}
			}
			else if(gType==1){ // directed
				if(indexu!=indexv){
					g[indexu].second.push_back(indexv);
					/*
					g[indexu].second[indexv] = true;
					if(model==0){
						//g[indexu].first.degree++;//added_now
					}
					*/
				}
				//else if(!(g[indexu].second.find(indexv)!= adj_list.end() && (g[indexv].second.find(indexu)!= adj_list.end()))){
					//				cout<<"Serious Error in Edges!"; return 0;
				//}
			}
		}
		inpFile.close();
		cout<<"Done reading file"<<endl;
	}
	else{
		cout << "Unable to open file"<<endl;
	}
	//id2index.clear();
	Timer tseed, tflow, t1;
	//	t1.start();
	unsigned int n=g.size();
	unsigned long int m=num_edges(g);
	avg_deg=2.0*(float)m/(float)n;
	//	std::cout << "Time to calculate length of graph params: (" << t1.stop() << " ms)" << std::endl;
	cout<<"The nodes of the graph G: "<<n<<endl;
	cout<<"The edges of the graph G: "<<m<<endl;
	cout<<"The average degree of the graph G: "<<avg_deg<<endl;
	/*
	   for(int i=0;i<n;i++){
	   cout<<g[i].first.id<<"\t"<<g[i].first.out_degree<<endl;
	   }
	 */
	double seed_time=0.0, sqdSeedTime=0.0, maxSeedTime=numeric_limits<double>::min(), minSeedTime=numeric_limits<double>::max(), meanSeedTime=0.0, stdSeedTime=0.0, flow_time=0.0, running_spread=0.0, spread=0.0, sqdSpread=0.0, maxSpread=numeric_limits<double>::min(), minSpread=numeric_limits<double>::max(), cur_time=0.0, cur_spread=0.0, meanSpread=0.0, stdSpread=0.0;
    
    vector<unsigned  int> seeds;
    if(seedFile.is_open()){
        while(seedFile>>vert1){
            seeds.push_back(id2index[vert1]);
        }
    }
	//vector<unsigned int> numSeeds {1,2,5,10,25,50,75,100,125,150,175,200};
	//vector<unsigned int> numSeeds {1,2,5};
	float flowTime;
//	for(unsigned int seed:numSeeds)
	{
		cur_spread=0.0;
		running_spread=0.0;
		sqdSpread=0.0;
		flowTime=0.0;
		//vector<unsigned int> flowSeeds(seeds.begin(),seeds.begin()+seed);
		vector<double> spreadVec;
		spreadOpFile.open(spreadFileName+"_"+to_string((long long int)num_seeds),ios::out | ios_base::app);
		for(int i=0;i<num_iter;i++){
			clock_t start = clock();
			running_spread=calculate_flow(g,seeds,lambda);
			clock_t end = clock();
                	float seconds = (float)(end - start) / CLOCKS_PER_SEC;
			flowTime+=seconds;
			spreadVec.push_back(running_spread+num_seeds);
			spreadOpFile<<running_spread+num_seeds<<endl;
			//sqdSpread+=running_spread*running_spread;
			//cur_spread+=running_spread;
		}
  
		for(int bin=0;bin<binSize.size();bin++){
			statOpFile.open(statFileName+"_"+to_string((long long int)binSize[bin]),ios::out | ios_base::app);
			sqdSpread=0.0;
			cur_spread=0.0;
			vector<double> meanSpread = processSpread(spreadVec, binSize[bin]);
			for(int i=0;i<meanSpread.size();i++){
				sqdSpread+=meanSpread[i]*meanSpread[i];
				cur_spread+=meanSpread[i];
			}
			spread=cur_spread/(double)meanSpread.size();
			stdSpread=sqrt((sqdSpread/(double)meanSpread.size()) - (spread*spread));
			//statOpFile<<"[Flow] [Stats] Spread with "<<seeds.size()<<" Seeds is: "<<spread<<" and time is: "<<flowTime<<endl;
			//statOpFile<<seeds.size()<<"\t"<<binSize[bin]<<"\t"<<spread<<"\t"<<stdSpread<<"\t"<<flowTime<<endl;
			statOpFile<<seeds.size()<<"\t"<<spread<<"\t"<<stdSpread<<"\t"<<flowTime<<endl;
			meanSpread.clear();
			statOpFile.close();
		}
	}
	//detailedOpFile.close();
	spreadOpFile.close();
	unordered_map<unsigned int, unsigned int>().swap(id2index);

    return 0;
	if(ch==0){
		/**************** Kempe Function Call Begins *********************/
		iter=1;
		while(iter>0){
			seeds.clear();
#ifdef PRINTS
			cout<<"[Kempe] Iteration Number: "<<iter<<endl;
#endif
			detailedOpFile<<"[Kempe] Iteration Number: "<<iter<<endl;
			tseed.start();
			cur_spread=kempe(g,num_seeds,lambda,seeds, num_iter);
			detailedOpFile<<"[Kempe] The spread is: "<<cur_spread<<endl;
			spread+=cur_spread;
			sqdSpread+=cur_spread*cur_spread;
			maxSpread=MAX(cur_spread,maxSpread);
			minSpread=MIN(cur_spread,minSpread);
#ifdef DEBUG
			detailedOpFile<<"[Kempe] The seeds are:\t";
			for(int i=0;i<seeds.size();i++){
				detailedOpFile<<g[seeds[i]].first.id<<"\t";
			}
			detailedOpFile<<endl;
#endif
			cur_time=tseed.stop();
			seed_time+=cur_time;
			sqdSeedTime+=cur_time*cur_time;
			maxSeedTime=MAX(cur_time,maxSeedTime);
			minSeedTime=MIN(cur_time,minSeedTime);
			detailedOpFile<<"[Kempe] The time taken for seed selection: "<<cur_time<<endl;
			detailedOpFile<<"[Kempe] The seed size is:"<<seeds.size()<<endl<<endl;
			iter--;
		}
		meanSeedTime=seed_time/(double)num_iter;
		stdSeedTime=sqrt((sqdSeedTime/(double)num_iter) - (meanSeedTime*meanSeedTime));

		meanSpread=spread/(double)num_iter;
		stdSpread=sqrt((sqdSpread/(double)num_iter) - (meanSpread*meanSpread));
		statOpFile<<"[Kempe] [Stats] Time taken for seed selection: "<<meanSeedTime<<","<<stdSeedTime<<","<<maxSeedTime<<","<<minSeedTime<<","<<num_iter<<endl;
		statOpFile<<"[Kempe] [Stats] Spread is: "<<meanSpread<<","<<stdSpread<<","<<maxSpread<<","<<minSpread<<","<<num_iter<<endl;	

		/**************** Kempe Function Call Ends *********************/
	}
	else if(ch==2)
	{
		/**************** Reachability Function Call Begins *********************/
		seed_time=0.0;
		flow_time=0.0;
		cur_time=0.0;
		spread=0.0;
		sqdSeedTime=0.0;
		maxSeedTime=numeric_limits<double>::min();
		minSeedTime=numeric_limits<double>::max();
		meanSeedTime=0.0;
		stdSeedTime=0.0;
		sqdSpread=0.0;
		maxSpread=numeric_limits<double>::min();
		minSpread=numeric_limits<double>::max();
		cur_spread=0.0;
		meanSpread=0.0;
		stdSpread=0.0;
		iter=num_iter;
		while(iter>0){
#ifdef PRINTS
			cout<<"[path] Iteration Number: "<<iter<<endl;
#endif
			detailedOpFile<<"[path] Iteration Number: "<<iter<<endl;
			seeds.clear();
			clock_t start = clock();
			cur_spread = select_reachability_path_seed(g,depth,num_seeds,seeds,lambda);
			detailedOpFile<<"[path] The spread is: "<<cur_spread<<endl;
			spread+=cur_spread;
			sqdSpread+=cur_spread*cur_spread;
			maxSpread=MAX(cur_spread,maxSpread);
			minSpread=MIN(cur_spread,minSpread);
#ifdef DEBUG
			detailedOpFile<<"[path] The seeds are:\t";
			for(int i=0;i<seeds.size();i++){
				detailedOpFile<<g[seeds[i]].first.id<<"\t";
			}
			detailedOpFile<<endl;
#endif
			clock_t end = clock();
	                float seconds = (float)(end - start) / CLOCKS_PER_SEC;

			cur_time=seconds;
			seed_time+=cur_time;
			sqdSeedTime+=cur_time*cur_time;
			maxSeedTime=MAX(cur_time,maxSeedTime);
			minSeedTime=MIN(cur_time,minSeedTime);
			detailedOpFile<<"[path] The time taken for seed selection: "<<cur_time<<endl;
			detailedOpFile<<"[path] The seed size is:"<<seeds.size()<<endl<<endl;
			iter--;
		}


		/*
		   statOpFile<<"[Reachability] [Stats] Time taken for seed selection: "<<seed_time<<","<<sqdSeedTime<<","<<maxSeedTime<<","<<minSeedTime<<","<<num_iter<<endl;
		   statOpFile<<"[Reachability] [Stats] Spread is: "<<spread<<","<<sqdSpread<<","<<maxSpread<<","<<minSpread<<","<<num_iter<<endl;
		 */
		meanSeedTime=seed_time/(double)num_iter;
		stdSeedTime=sqrt((sqdSeedTime/(double)num_iter) - (meanSeedTime*meanSeedTime));

		meanSpread=spread/(double)num_iter;
		stdSpread=sqrt((sqdSpread/(double)num_iter) - (meanSpread*meanSpread));
		statOpFile<<"[path] [Stats] Time taken for seed selection: "<<meanSeedTime<<","<<stdSeedTime<<","<<maxSeedTime<<","<<minSeedTime<<","<<num_iter<<endl;
		statOpFile<<"[path] [Stats] Spread is: "<<meanSpread<<","<<stdSpread<<","<<maxSpread<<","<<minSpread<<","<<num_iter<<endl;
		/**************** Reachability Function Call Ends *********************/
		seeds.clear();	
		disp_mem_usage("");
	}

	detailedOpFile.close();
	statOpFile.close();
	return 0;
}
