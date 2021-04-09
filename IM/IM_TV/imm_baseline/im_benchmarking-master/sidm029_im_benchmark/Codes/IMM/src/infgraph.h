#define SEED 1993
typedef pair<double,int> dipair;


#include "iheap.h"

class InfGraph: public Graph
{
private:
    vector<bool> visit;
    vector<int> visit_mark;
public:
    vector<vector<int>> hyperG;
    vector<vector<int>> hyperGT;

    InfGraph(string folder, string graph_file, int seed_random): Graph(folder, graph_file)
    {
        sfmt_init_gen_rand(&sfmtSeed , seed_random
        );
        init_hyper_graph();
        visit = vector<bool> (n);
        visit_mark = vector<int> (n);
    }


    void init_hyper_graph(){
        hyperG.clear();
        for (int i = 0; i < n; i++)
            hyperG.push_back(vector<int>());
        hyperGT.clear();
    }
    void build_hyper_graph_r(int64 R, const Argument & arg)
    {
        if( R > INT_MAX ){
            cout<<"Error:R too large"<<endl;
            exit(1);
        }
        //INFO("build_hyper_graph_r", R);



        int prevSize = hyperGT.size();
        while ((int)hyperGT.size() <= R)
            hyperGT.push_back( vector<int>() );



        vector<int> random_number;
        for (int i = 0; i < R; i++)
        {
            random_number.push_back(  sfmt_genrand_uint32(&sfmtSeed) % n);
        }

        //trying BFS start from same node
        

        for (int i = prevSize; i < R; i++)
        {
#ifdef CONTINUOUS
            BuildHypergraphNode(random_number[i], i, arg );
#endif
#ifdef DISCRETE
            BuildHypergraphNode(random_number[i], i );
#endif
        }


        int totAddedElement = 0;
        for (int i = prevSize; i < R; i++)
        {
            for (int t : hyperGT[i])
            {
                hyperG[t].push_back(i);
                //hyperG.addElement(t, i);
                totAddedElement++;
            }
        }
    }

#ifdef DISCRETE
#include "discrete_rrset.h"
#endif
#ifdef CONTINUOUS
#include "continuous_rrset.h"
#endif

    //return the number of edges visited
    deque<int> q;
    sfmt_t sfmtSeed;
    set<int> seedSet;
    map<int, int> dict_node_id_gain;

    void build_seedset(int k,int test=0)
    {
        Counter cnt(1);
        vector< int > degree;
        vector< int> visit_local(hyperGT.size());

        seedSet.clear();
        for (int i = 0; i < n; i++)
        {
            degree.push_back( hyperG[i].size() );
        }
        ASSERT(k > 0);
        ASSERT(k < (int)degree.size());

        cout<<" get reverse reachability set "<<test;

        for (int i = 0; i < k; i++)
        {

            auto t = max_element(degree.begin(), degree.end());

            int id = t - degree.begin();
            if ( test==1)
              {
              cout<<" id "<<id<<" t "<<*t;
              dict_node_id_gain[id]= int(*t);
              }


            seedSet.insert(id);
            degree[id] = 0;
            for (int t : hyperG[id])
            {
                if (!visit_local[t])
                {
                    visit_local[t] = true;
                    for (int node : hyperGT[t])
                    {
                        degree[node]--;
                    }
                }
            }
        }
        TRACE(seedSet);

    }
    double InfluenceHyperGraph()
    {
        set<int> s;
        TRACE(seedSet);
        for (auto t : seedSet)
        {
            for (auto tt : hyperG[t])
            {
                s.insert(tt);
            }
        }
        double inf = (double)n * s.size() / hyperGT.size();
        return inf;
    }

};
