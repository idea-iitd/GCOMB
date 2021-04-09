#include "LTTest.h"

LTTest::LTTest(AnyOption *opt1) {

   	opt = opt1;
    cout << "In testing phase" << endl;

    outdir = opt->getValue("outdir");
	probGraphFile = opt->getValue("probGraphFile");
	graphType = DIRECTED;
	
	setModel(opt);
	
	cout << "User specified options : " << endl;
	cout << "model : " << m << " or " << model << endl;
	cout << "outdir : " << outdir << endl;
	cout << "Input graph file : " << probGraphFile << endl;
	
	//srand(time(NULL));
}


LTTest::~LTTest() {
    cout << "Total time taken : " << getTime() << endl;
}


void LTTest::setModel(AnyOption *opt) {
    m = opt->getValue("propModel");
    model = LT;
    if (m.compare("LT") == 0) {
		model = LT;	
	} else if (m.compare("IC") == 0) {
		model = IC;
	} /*else if (m.compare("PC") == 0) {
		model = PC;
	}*/
}


void LTTest::doAll() {

    xNode *pX = new xNode();

    MC *mc = new MC(this->opt);
    mc->readInputData();

    AM = mc->getAM();
    users = mc->getUsers();
    numEdges = mc->getNumEdges();  
    
    cout << "Number of users in graph : " << users->size() << endl;

    int phase = strToInt(opt->getValue("phase"));

    clearAll();
    if (phase == 20) {
      // baseline algos
        Baseline *base = new Baseline(this->opt);
        base->setAM(this->AM);
        base->setUsers(this->users);

        if (base->checkDS() != false) {
            base->doAll();
        }
    }

}


void LTTest::clearAll() {
    curSeedSet.clear();
}


float LTTest::getTime() const {	
	time_t curTime;
	time(&curTime);

	float min = ((float)(curTime - startTime))/60;
	return min;
}


void LTTest::openOutputFile() {
	if (outFile.is_open()) {
		outFile.close(); 
	}
	string algorithm = "lttest";
	cout << "problem : " << problem << endl;
	string filename = outdir + "/lttest" + ".txt";
	
//	string filename = outdir + "/" + problem + "_" + m + "_" + algorithm + "_0" + ".txt";
	outFile.open (filename.c_str());

	if (outFile.is_open() == false) {
		cout << "Can't open file " << filename << " for writing" << endl;
		exit(1);
	}

}


void LTTest::writeOutputFile(UID v, float cov, float marginal_gain, int curTimeStep) {
	cout << endl << "Picked a seed node: " << v << ", total: " << curSeedSet.size() << endl;
	outFile << v << " " << cov << " " << marginal_gain << " " << curTimeStep << " " << getCurrentMemoryUsage() << " " << getTime() <<  " " << " " <<  endl;
	cout << v << " " << cov << " " << marginal_gain << " " << curTimeStep << " " << getCurrentMemoryUsage() << " " << getTime() << " " <<  endl;
	cout << endl << endl;

}


////////////////////////////// 
// end of namespace is here //
// ///////////////////////////
//}


