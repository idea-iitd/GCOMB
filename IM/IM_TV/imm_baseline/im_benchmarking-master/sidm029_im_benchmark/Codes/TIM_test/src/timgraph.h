class TimGraph: public InfGraph
{
    public:
        TimGraph(string folder, string graph_file):InfGraph(folder, graph_file ){
        }
        double MgT(int u){
            //static int i=0;
            //i++;
            //TRACE_LINE("mgt", i);
            ASSERT(u>=0);
            ASSERT(u<n);
            return (double)BuildHypergraphNode(u, 0, false);
        }
        void DistPu()
        {
            for(int i=0; i<100; i++)
            {
                int u=rand()%n;
                for(int j=0; j<10; j++)
                {
                    double pu=MgT(u)/m;
                    cout << pu <<" ";
                }
                cout<<endl;
            }

        }
        int loopEstimateEPT=0;
        double EstimateEPT2(){
            double lb=1/2.0;
            //double ub=1;
            double c=0;
            int64 lastR=0;
            while(true){
                int loop= (6 * log(n)  +  6 * log(log(n)/ log(2)) )* 1/lb  ;
                c=0;
                lastR=loop;
                IF_TRACE(int64 now=rdtsc());
                double sumMgTu=0;
                for(int i=0; i<loop; i++){
                    int u=rand()%n;
                    double MgTu=MgT(u);
                    double pu=MgTu/m;
                    sumMgTu+=MgTu;
                    c+=1-pow((1-pu), k);
                    loopEstimateEPT++;
                }
                c/=loop;
                if(c>lb) break;
                //if(lb<0.01)
                    //break;
                lb /= 2;
            }
            BuildHypergraphR(lastR);
            //TRACE_LINE_END();
            return c * n;
        }
        //double ept=-1;
        double EstimateEPT()
        {
            Timer t(1, "step1");
            double ept=EstimateEPT2();
            ept/=2;
            return ept;
        }
        void BuildHyperGraph2(double epsilon, double ept){
            Timer t(2, "step2" );
            ASSERT(ept > 0);
            //int64 R = 4.0 * n * log(n) / epsilon / epsilon / ept;
            int64 R = (8+2 * epsilon) * ( n * log(n) +  n * log(2)  ) / ( epsilon * epsilon * ept)/4;
            //R/=100;
            BuildHypergraphR(R);
        }
        double logcnk(int n, int k){
            double ans=0;
            for(int i=n-k+1; i<=n; i++){
                ans+=log(i);
            }
            for(int i=1; i<=k; i++){
                ans-=log(i);
            }
            return ans;
        }
        void BuildHyperGraph3(double epsilon, double opt){
            Timer t(3, "step3");
            ASSERT(opt > 0);
            //int64 R = 16.0 * k * n * log(n) / epsilon / epsilon / opt;
            int64 R = (8+2 * epsilon) * ( n * log(n) + n * log(2) +  n * logcnk(n, k) ) / ( epsilon * epsilon * opt);
            //R/=100;
            BuildHypergraphR(R);
        }
        void EstimateOPT(double epsilon){
            Timer t(100,"EstimateOPT");
            //double ep_step3=0.1;
            double ep_step2, ep_step3;
            ep_step2=ep_step3=epsilon;
            ep_step2=5*pow(sqr(ep_step3)/k, 1.0/3.0);
            double ept;
            ept=EstimateEPT();
            
            BuildSeedSet();
            //ep_step2=1;
            BuildHyperGraph2(ep_step2, ept);
            ept=InfluenceHyperGraph();
            ept/=1+ep_step2;
            


            BuildHyperGraph3(ep_step3, ept);
            BuildSeedSet();
            ept=InfluenceHyperGraph();

            disp_mem_usage("");
        }

};

