#define HEAD_INFO

#include "sfmt/SFMT.h"
#include "head.h"
#include "memoryusage.h"
#include <stdio.h>

class Argument{
public:
    int k;
    string dataset;
    string outputFolder;
    double epsilon;
    string model;
    double T;
    int seed_random;
    int training_for_gain;
};


#include "graph.h"
#include "infgraph.h"
#include "imm.h"



void run_with_parameter(InfGraph &g, const Argument & arg)
{
        cout << "--------------------------------------------------------------------------------" << endl;
	//double epsilonPrint = floor(arg.epsilon *100.)/100.;

        char epsilonConvert[64];
        double epsilonPrint;
        sprintf(epsilonConvert, "%.*g", 2, arg.epsilon);
        epsilonPrint = strtod(epsilonConvert, 0);

        cout << arg.dataset << " k=" << arg.k << " epsilon=" << epsilonPrint <<   " " << arg.model << endl;

	clock_t time_start = clock();
        Imm::InfluenceMaximize(g, arg);
	clock_t time_end = clock();
	ofstream seedFile, outputFile, statFile;
	string outputFilename, statFilename, seedFilename;

       statFilename = arg.outputFolder + "/" + "stat_IMM_" + arg.model + "_" + to_string(arg.k)+"_"+epsilonConvert+ ".txt";
       seedFilename = arg.outputFolder + "/" + "seeds_IMM_" + arg.model + "_" + to_string(arg.k)+"_"+epsilonConvert+ ".txt";
       cout << "==>" << arg.outputFolder << endl;
       cout << "==>" << statFilename << endl;
       cout << "==>" << seedFilename << endl;
       statFile.open (statFilename.c_str());
       seedFile.open (seedFilename.c_str());

       cout << "Item Set Start" << endl;
       string f_name = arg.outputFolder + ".txt";
       FILE* fp = fopen(f_name.c_str(), "w+");
       for(int item:g.seedSet){
                seedFile << item << endl;
                fprintf(fp, "%d\n", item);
                cout << item << endl;
        }
       cout << "Item Set End" << endl;
       fclose(fp);

        //statFile<<g.InfluenceHyperGraph()<<" "<<Timer::timeUsed[100]/TIMES_PER_SEC<<" "<< disp_mem_usage("")<<endl;//Add memory usage and check time
        statFile<<g.InfluenceHyperGraph()<<" "<<(float)(time_end - time_start) / CLOCKS_PER_SEC<<" "<< disp_mem_usage("")<<endl;//Add memory usage and check time
        cout<<"===" << g.InfluenceHyperGraph()<<" "<<(float)(time_end - time_start) / CLOCKS_PER_SEC<<" "<< disp_mem_usage("")<<endl;//Add memory usage and check time
        statFile.close();
        seedFile.close();
        outputFile.close();



        INFO(g.seedSet);
        INFO(g.InfluenceHyperGraph());
    Timer::show();

        if (arg.training_for_gain ==1)
        {           ofstream nodeGainFile;
                    string nodeGainFilename ;

                   nodeGainFilename = arg.outputFolder +    "_dict_node_gain"         + ".txt";
                   cout << "==>" << nodeGainFilename << endl;

                    nodeGainFile.open (nodeGainFilename.c_str());

                   //FILE* fp = fopen(nodeGainFilename.c_str(), "w+");
                   for (std::map<int,int>::iterator it=g.dict_node_id_gain.begin(); it!=g.dict_node_id_gain.end(); ++it)
                    {
                            nodeGainFile << it->first << " "<<  it->second<<endl;
                            cout << it->first << " "<<  it->second<<endl;

                    }
                nodeGainFile.close();
        }
   //  for (std::map<int,int>::iterator it=g.dict_node_id_gain.begin(); it!=g.dict_node_id_gain.end(); ++it)
     //      std::cout << it->first << " => " << it->second << '\n';




}
void Run(int argn, char **argv)
{
    Argument arg;


    for (int i = 0; i < argn; i++)
    {
        if (argv[i] == string("-help") || argv[i] == string("--help") || argn == 1)
        {
            cout << "./imm -dataset *** -output *** -epsilon *** -k ***  -model IC|LT|TR|CONT " << endl;
            return ;
        }
        if (argv[i] == string("-dataset")) 
            arg.dataset = argv[i + 1];
        if (argv[i] == string("-epsilon")) 
            arg.epsilon = atof(argv[i + 1]);
        if (argv[i] == string("-T")) 
            arg.T = atof(argv[i + 1]);
        if (argv[i] == string("-k")) 
            arg.k = atoi(argv[i + 1]);
        if (argv[i] == string("-model"))
            arg.model = argv[i + 1];

        if (argv[i] == string("-seed_random"))
            arg.seed_random = atoi(argv[i + 1]);

        if (argv[i] == string("-training_for_gain"))
            arg.training_for_gain = atoi(argv[i + 1]);

	if (argv[i]==string("-output"))
		arg.outputFolder = argv[i+1];
    }
    ASSERT(arg.dataset != "");
    ASSERT(arg.model == "IC" || arg.model == "LT" || arg.model == "TR" || arg.model=="CONT");

    string graph_file;
	graph_file =arg.dataset;
/*commented by sigdata
    if (arg.model == "IC")
        graph_file = arg.dataset + "graph_ic.inf";
    else if (arg.model == "LT")
        graph_file = arg.dataset + "graph_lt.inf";
    else if (arg.model == "TR")
        graph_file = arg.dataset + "graph_tr.inf";
    else if (arg.model == "CONT")
        graph_file = arg.dataset + "graph_cont.inf";
    else
        ASSERT(false);
*/
    //get the dataset folder
    std::size_t found = arg.dataset.find_last_of("/");
    arg.dataset = arg.dataset.substr(0,found+1);
    InfGraph g(arg.dataset, graph_file, arg.seed_random);


    if (arg.model == "IC")
        g.setInfuModel(InfGraph::IC);
    else if (arg.model == "LT")
        g.setInfuModel(InfGraph::LT);
    else if (arg.model == "TR")
        g.setInfuModel(InfGraph::IC);
    else if (arg.model == "CONT")
        g.setInfuModel(InfGraph::CONT);
    else
        ASSERT(false);

    INFO(arg.T);

    run_with_parameter(g, arg);
}


int main(int argn, char **argv)
{
    __head_version = "v1";
    OutputInfo info(argn, argv);


    Run( argn, argv );
}


