#include "common.h"

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

double getCurrentMemoryUsage(){
    double vm, rss;

    process_mem_usage(vm, rss);
    vm/=1024;
    rss/=1024;
    //cout<< "Memory Usage:" << msg << " vm:" << vm << " MB  rss:" << rss << " MB"<<endl;
    //cout<< "Memory Usage:" << msg << " " << rss << " MB"<<endl;
    return rss;
}
/*
float getCurrentMemoryUsage() {
//	const char* outdir = "/tmp";
//	char command[strlen(outdir) + 20];
//	char tmpfile[strlen(outdir) + 40];

//	sprintf(tmpfile, "%s/tmp.out", outdir);
//	sprintf(command, "pmap %d | grep Total | awk '{print $3}' > %s", (unsigned)getpid(), tmpfile);

	string pid = intToStr(unsigned(getpid()));
	string outfile = "temp/tmp_" + pid + ".txt";
	string command = "pmap " + pid + " | grep -i Total | awk '{print $2}' > " + outfile;
	//	cout << "Command to be executed: " << command << endl;
	system(command.c_str());

	string mem_str;
	ifstream ifs(outfile.c_str());
	std::getline(ifs, mem_str);
	ifs.close();

	mem_str = mem_str.substr(0, mem_str.size()-1);
	float mem = (float)strToInt(mem_str);

//	cout << "Memory used at this point : " << mem/1024 << " MB" << endl;
	return mem/1024;
	
	return 0;
}
*/
/**** 
  Function for deleting pointers in map.
	example to delete a map : 
	for_each( StringMap.begin(),
              StringMap.end(),
              DeleteMap<int,CString*>());
  ****/
template<class A, class B>
struct DeleteMap
{
    // Overloaded () operator.
    // This will be called by for_each() function.
    bool operator()(pair<A,B> x) const
    {
        // Assuming the second item of map is to be
        // deleted. Change as you wish.
        delete x.second;
        return true;
    }
};


