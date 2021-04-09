//#define HEAD_TRACE
#define HEAD_INFO

#define HEAD_INFO
//#define HEAD_TRACE
#include "sfmt/SFMT.h"
#include "head.h"
#include "memoryusage.h"
#include "graph.h"
#include <string>
void run(TimGraph & m, string dataset, string outdir, int k, double epsilon, string model ){
       
    ofstream seedFile, outputFile, statFile;
    string outputFilename, statFilename, seedFilename;

    //double epsilonPrint = floor(epsilon *100.)/100.;

    char epsilonConvert[64];
    double epsilonPrint;
    sprintf(epsilonConvert, "%.*g", 2, epsilon);
    epsilonPrint = strtod(epsilonConvert, 0);

    outputFilename = outdir + "/" + "output_TIM_" + model + "_" + to_string(k)+"_"+epsilonConvert + ".txt";

    outputFile.open (outputFilename.c_str());
    outputFile << "dataste:" << dataset << " k:" << k << " epsilon:"<< epsilonPrint <<   " model:" << model << endl;
    m.k=k;
    if(model=="IC")
        m.setInfuModel(InfGraph::IC);
    else if(model=="LT")
        m.setInfuModel(InfGraph::LT);
    else
        ASSERT(false);

    outputFile<<"Finish Read Graph, Start Influecne Maximization"<<endl;
    clock_t time_start = clock();
    m.EstimateOPT(epsilon);
    clock_t time_end = clock();
    outputFile<<"Time used: " << Timer::timeUsed[100]/TIMES_PER_SEC << "s" <<endl;

    outputFile<<"Selected k SeedSet: ";
    for(auto item:m.seedSet)
        outputFile<< item << " ";
    cout<<endl;
    outputFile<<"Estimated Influence: " << m.InfluenceHyperGraph() << endl;
    Counter::show();
    //Added by Sigdata
       statFilename = outdir + "/" + "stat_TIM_" + model + "_" + to_string(k)+"_"+epsilonConvert + ".txt";
       seedFilename = outdir + "/" + "seeds_TIM_" + model + "_" + to_string(k)+"_"+epsilonConvert + ".txt";
       statFile.open (statFilename.c_str());
       seedFile.open (seedFilename.c_str());
       for(auto item:m.seedSet)
       		seedFile<< item << endl;
	//statFile<<m.InfluenceHyperGraph()<<" "<<Timer::timeUsed[100]/TIMES_PER_SEC<<" "<< disp_mem_usage("")<<endl;//Add memory usage and check time
        statFile<<m.InfluenceHyperGraph()<<" "<<(float)(time_end - time_start) / CLOCKS_PER_SEC<<" "<< disp_mem_usage("")<<endl;//Add memory usage and check time
	statFile.close();
	seedFile.close();
	outputFile.close();
}
void parseArg(int argn, char ** argv)
{
    string dataset="";

    double epsilon=0;
    string model="";
    int k=0;
    string outdir="";
    for(int i=0; i<argn; i++)
    {
        if(argv[i]==string("-dataset")) dataset=string(argv[i+1]);//Modified by Sigdata +"/";
        if(argv[i]==string("-outdir")) outdir=string(argv[i+1]);//Added by Sigdata +"/";
        if(argv[i]==string("-epsilon")) epsilon=atof(argv[i+1]);
        if(argv[i]==string("-k")) k=atoi(argv[i+1]);
        if(argv[i]==string("-model")) {
            if(argv[i+1]==string("LT"))
            {
                model=argv[i+1];
            }
            else if(argv[i+1]==string("IC"))
            {
                model=argv[i+1];
            }
            else
                ExitMessage("model should be IC or LT");
        }
    }
    if (dataset=="")
        ExitMessage("argument dataset missing");
    if (k==0)
        ExitMessage("argument k missing");
    if (epsilon==0)
        ExitMessage("argument epsilon missing");
    if (model=="")
        ExitMessage("argument model missing");


    string graph_file;
    /*if(model=="IC")
        graph_file=dataset + "graph_ic.inf";
    else if(model=="LT")
        graph_file=dataset + "graph_lt.inf";
	*/
    graph_file =dataset;
	//Extract folder from dataset to get the folder
    std::size_t found = dataset.find_last_of("/");
    dataset = dataset.substr(0,found+1);
    //cout<<"folder is "<<dataset<<" "<<found<<endl;
    TimGraph m(dataset, graph_file);
    run(m, dataset, outdir, k ,  epsilon, model );
}





int main(int argn, char ** argv)
{
    OutputInfo info(argn, argv);
    parseArg( argn, argv );
}






