
vector<double> dist;
vector<int> clean_helper;

iHeap<double> iheap;
int BuildHypergraphNode(int uStart, int hyperiiid, const Argument & arg){
    //Weibull::init();
    //Timer tm(4, "Build cont", true);
    //iheap.DeepClean();
    if(iheap.inited == false){
        iheap.initialize(n);
        INFO("init");
    }
    iheap.smart_clean(clean_helper);
    //if(hyperiiid %100 ==0)
        //INFO(hyperiiid);

    double T = arg.T;


    if((int)dist.size()!=n){
        dist=vector<double>(n);
        for(int i=0; i<n; i++)
            dist[i]=-1;
    }
    for(auto element : clean_helper)
        dist[element] = -1;
    clean_helper.clear();


    ASSERT(T>0);
    int n_visit_edge=1;


    dist[uStart]=0;
    clean_helper.push_back(uStart);

    iheap.insert(uStart, 0);


    //TRACE(hyperiiid);
    vector<int> edge_debug;
    int64 debug_time = 0;
    while(!iheap.empty()) {
        int expand=iheap.m_data[0].key;
        double cost=iheap.m_data[0].value;
        iheap.pop();
        //INFO(expand, cost);

        n_visit_edge+=gT[expand].size();
        edge_debug.push_back( gT[expand].size());

        if(cost > T)
        {
            INFO("cound not happen");
            break;
        }

            //hyperG[v].push_back(hyperiiid);
        ASSERT((int)hyperGT.size() > hyperiiid);
        hyperGT[hyperiiid].push_back(expand);

        for(int j=0; j<(int)gT[expand].size(); j++){
            //int u=expand;
            int v=gT[expand][j];
            //int64 now = rdtsc();
            double randDouble=double(rand())/double(RAND_MAX);
            double shape=probT[expand][j].F;
            double scale=probT[expand][j].S;
            //double randWeibull= scale * Weibull::f[int(randDouble*1000)][int(shape*1000)];
            double randWeibull=scale * pow( - log( randDouble), 1.0/shape);
            //TRACE(randWeibull);

            //debug_time += rdtsc() - now;
            if( ( dist[v]==-1 || dist[v] > dist[expand] + randWeibull ) && dist[expand] + randWeibull < T)
            {
                dist[v]=dist[expand]+randWeibull;
                iheap.insert( v, dist[v] );
                clean_helper.push_back(v);
            }
        }
    }
    return n_visit_edge;

    if(hyperGT[hyperiiid].size() > 1000){
        INFO(debug_time / TIMES_PER_SEC);
        INFO(hyperGT[hyperiiid].size());
        INFO(n_visit_edge);
    }
    return n_visit_edge;
}




