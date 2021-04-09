
class VV
{
    public:
        vector<int> head;
        vector<int> next;
        vector<int> data;
        vector<int> vsize;
        void clear()
        {
            head.clear();
            next.clear();
            data.clear();
            vsize.clear();
        }
        // trick for not change code
        void push_back(vector<int> x){
            ASSERT(x.size()==0);
            addVector();
        }
        void addVector()
        {
            head.push_back(-1);
            vsize.push_back(0);
        }
        int size(int t){
            return vsize[t];
        }
        //vv[a].push_back(b)
        void addElement( int a, int b)
        {
            //a.push_back(b);
            vsize[a]++;
            data.push_back(b);
            next.push_back(head[a]);
            head[a]=next.size()-1;

        }
        //loop
        //for(int t:vv[a])
        //for (int x=vv.head[a]; x!=-1; x=vv.next[x])
        //{
        //t=vv.data[x]
        //}
};




class InfGraph:public Graph
{
    public:
        vector<vector<int>> hyperG;
        //VV hyperG;
        vector<vector<int>> hyperGT;

        InfGraph(string folder, string graph_file):Graph(folder, graph_file){
            hyperG.clear();
            for(int i=0; i<n; i++)
                hyperG.push_back(vector<int>());
            for(int i=0; i<12; i++)
                sfmt_init_gen_rand(&sfmtSeed, i+1234);
        }

        enum ProbModel {TR, WC, TR001};
        ProbModel probModel;

        void BuildHypergraphR(int64 R){
            hyperId=R;
            //for(int i=0; i<n; i++)
                //hyperG[i].clear();
            hyperG.clear();
            for(int i=0; i<n; i++)
                hyperG.push_back(vector<int>());
            hyperGT.clear();
            while((int)hyperGT.size() <= R)
                hyperGT.push_back( vector<int>() );

            for(int i=0; i<R; i++){
                BuildHypergraphNode(sfmt_genrand_uint32(&sfmtSeed)%n, i, true);
            }
            int totAddedElement=0;
            for(int i=0; i<R; i++){
                for(int t:hyperGT[i])
                {
                    hyperG[t].push_back(i);
                    //hyperG.addElement(t, i);
                    totAddedElement++;
                }
            }
            ASSERT(hyperId == R);
        }

        int BuildHypergraphNode(int uStart, int hyperiiid, bool addHyperEdge){
            int n_visit_edge=1;
            if(addHyperEdge)
            {
                ASSERT((int)hyperGT.size() > hyperiiid);
                hyperGT[hyperiiid].push_back(uStart);
            }

            int n_visit_mark=0;
            //for(int i=0; i<12; i++) ASSERT((int)visit[i].size()==n);
            //for(int i=0; i<12; i++) ASSERT((int)visit_mark[i].size()==n);
            //hyperiiid ++;
            q.clear();
            q.push_back(uStart);
            ASSERT(n_visit_mark < n);
            visit_mark[n_visit_mark++]=uStart;
            visit[uStart]=true;
            while(!q.empty()) {
                int expand=q.front();
                q.pop_front();
                if(influModel==IC){
                    int i=expand;
                    for(int j=0; j<(int)gT[i].size(); j++){
                        //int u=expand;
                        int v=gT[i][j];
                        n_visit_edge++;
                        double randDouble=double(sfmt_genrand_uint32(&sfmtSeed))/double(RAND_MAX)/2;
                        if(randDouble > probT[i][j])
                            continue;
                        if(visit[v])
                            continue;
                        if(!visit[v])
                        {
                            ASSERT(n_visit_mark < n);
                            visit_mark[n_visit_mark++]=v;
                            visit[v]=true;
                        }
                        q.push_back(v);
                        //#pragma omp  critical 
                        //if(0)
                        if(addHyperEdge)
                        {
                            //hyperG[v].push_back(hyperiiid);
                            ASSERT((int)hyperGT.size() > hyperiiid);
                            hyperGT[hyperiiid].push_back(v);
                        }
                    }
                }
                else if(influModel==LT){
                    if(gT[expand].size()==0)
                        continue;
                    ASSERT(gT[expand].size()>0);
                    n_visit_edge+=gT[expand].size();
                    double randDouble=double(sfmt_genrand_uint32(&sfmtSeed))/double(RAND_MAX)/2;
                    for(int i=0; i<(int)gT[expand].size(); i++){
                        ASSERT( i< (int)probT[expand].size());
                        randDouble -= probT[expand][i];
                        if(randDouble>0)
                            continue;
                        //int u=expand;
                        int v=gT[expand][i];

                        if(visit[v])
                            break;
                        if(!visit[v])
                        {
                            visit_mark[n_visit_mark++]=v;
                            visit[v]=true;
                        }
                        q.push_back(v);
                        if(addHyperEdge)
                        {
                            ASSERT((int)hyperGT.size() > hyperiiid);
                            hyperGT[hyperiiid].push_back(v);
                        }
                        break;
                    }
                }
                else
                    ASSERT(false);
            }
            for(int i=0; i<n_visit_mark; i++)
                visit[visit_mark[i]]=false;
            return n_visit_edge;
        }

        //return the number of edges visited
        int64 hyperId = 0;
        deque<int> q;
        sfmt_t sfmtSeed;
        set<int> seedSet;
        void BuildSeedSet() {
            vector< int > degree;
            vector< int> visit_local(hyperGT.size());
            //sort(ALL(degree));
            //reverse(ALL(degree));
            seedSet.clear();
            for(int i=0; i<n; i++)
            {
                degree.push_back( hyperG[i].size() );
                //degree.push_back( hyperG.size(i) );
            }
            ASSERT(k > 0);
            ASSERT(k < (int)degree.size());
            for(int i=0; i<k; i++){
                auto t=max_element(degree.begin(), degree.end());
                int id=t-degree.begin();
                seedSet.insert(id);
                degree[id]=0;
                for(int t:hyperG[id]){
                    if(!visit_local[t]){
                        visit_local[t]=true;
                        for(int item:hyperGT[t]){
                            degree[item]--;
                        }
                    }
                }
            }
        }
        double InfluenceHyperGraph(){

            set<int> s;
            for(auto t:seedSet){
                for(auto tt:hyperG[t]){
                //for(int index=hyperG.head[t]; index!=-1; index=hyperG.next[index]){
                    //int tt=hyperG.data[index];
                    s.insert(tt);
                }
            }
            double inf=(double)n*s.size()/hyperId;
            return inf;
        }


















};



