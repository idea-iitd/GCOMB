class Math{
    public:
        static double log2(int n){
            return log(n) / log(2);
        }
        static double logcnk(int n, int k) {
            double ans = 0;
            for (int i = n - k + 1; i <= n; i++)
            {
                ans += log(i);
            }
            for (int i = 1; i <= k; i++)
            {
                ans -= log(i);
            }
            return ans;
        }
};
class Imm
{
    private:
        //static InfGraph g;
        //static int k;
        //static map<string, string> arg;

        static double step1(InfGraph &g, const Argument & arg)
        {
            double epsilon_prime = arg.epsilon * sqrt(2);
            Timer t(1, "step1");
            for (int x = 1; ; x++)
            {
                int64 ci = (2+2/3 * epsilon_prime)* ( log(g.n) + Math::logcnk(g.n, arg.k) + log(Math::log2(g.n))) * pow(2.0, x) / (epsilon_prime* epsilon_prime);
                g.build_hyper_graph_r(ci, arg);
                g.build_seedset(arg.k);
                double ept = g.InfluenceHyperGraph()/g.n;
                double estimate_influence = ept * g.n;
                INFO(x, estimate_influence);
                if (ept > 1 / pow(2.0, x))
                {
                    double OPT_prime = ept * g.n / (1+epsilon_prime);
                    //INFO("step1", OPT_prime);
                    //INFO("step1", OPT_prime * (1+epsilon_prime));
                    return OPT_prime;
                }
            }
            ASSERT(false);
            return -1;
        }
        static double step2(InfGraph &g, const Argument & arg, double OPT_prime)
        {
            Timer t(2, "step2");
            ASSERT(OPT_prime > 0);
            double e = exp(1);
            double alpha = sqrt(log(g.n) + log(2));
            double beta = sqrt((1-1/e) * (Math::logcnk(g.n, arg.k) + log(g.n) + log(2)));

            int64 R = 2.0 * g.n *  sqr((1-1/e) * alpha + beta) /  OPT_prime / arg.epsilon / arg.epsilon ;

            //R/=100;
            g.build_hyper_graph_r(R, arg);
            cout<<"training_for_gain"<<arg.training_for_gain;
                   g.build_seedset(arg.k,1);
     //       g.build_seedset(arg.k,arg.training_for_gain);
            double opt = g.InfluenceHyperGraph();
            return opt;
        }
    public:
        static void InfluenceMaximize(InfGraph &g, const Argument &arg)
        {
            Timer t(100, "InfluenceMaximize(Total Time)");

            INFO("########## Step1 ##########");

            // debugging mode lalala
            double OPT_prime;
            OPT_prime = step1(g, arg ); //estimate OPT



            INFO("########## Step2 ##########");


            double opt_lower_bound = OPT_prime;
            INFO(opt_lower_bound);
            step2(g, arg, OPT_prime);
            INFO("step2 finish");
	    //disp_mem_usage("");

        }

};

