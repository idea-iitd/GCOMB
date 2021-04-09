#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<map>
#include<unordered_map>
#include<ctime>
#include<cstdlib>
#include<time.h>
#include<stdio.h>
#include<algorithm>
#include<math.h>
#include<functional>
using namespace std;


unordered_map<long long int, long long int> node_map;
unordered_map<long long int, unordered_map<long long int,int> > adjlist;
unordered_map<long long int, int> degree;
unordered_map<long long int, int> in_degree;
unordered_map<long long int, int> out_degree;


void split(const string& s, char c, vector<string>& v) 
{

   string::size_type i = 0;
   string::size_type j = s.find(c);

   while (j != string::npos) 
   {
      v.push_back(s.substr(i, j-i));
      i = ++j;
      j = s.find(c, j);

      if (j == string::npos)
         v.push_back(s.substr(i, s.length()));
   }
   
   return;
}



void read_graph(string filename, int gtype)
{
	ifstream infile;
	
	long long int a,b,i=0;
	
	infile.open(filename.c_str(),ios::in);
	
	if(infile.is_open())
	{	
		
		//directed graph
		if(gtype==2)
		{
		
			while(infile>>a>>b)
			{
			
				//check if nodes exist
				if(a!=b){
					if(node_map.find(a)==node_map.end())
					{
						node_map[a] = i++;
						in_degree[a] = 0;
						out_degree[a] = 0;

					}

					if(node_map.find(b)==node_map.end())
					{
						node_map[b] = i++;
						in_degree[b] = 0;
						out_degree[b] = 0;
					}


					adjlist[a][b] = 1;

					in_degree[b]++;

					out_degree[a]++;
				}
			}
			
		}
		
		
		//undirected graph
		else if(gtype==1)
		{
			
			while(infile>>a>>b)
			{
			
				//check if nodes exist
				if(a!=b){

					if(node_map.find(a)==node_map.end())
					{
						node_map[a] = i++;
						degree[a] = 0;

					}

					if(node_map.find(b)==node_map.end())
					{
						node_map[b] = i++;
						degree[b] = 0;

					}

					//incrementing degree iff a,b or b,a dont exist
					if(!adjlist[a][b]&&!adjlist[b][a])
					{

						degree[a]++;

						degree[b]++;

					}

					adjlist[a][b] = 1;

					adjlist[b][a] = 1;

				}
	
			}
			
		}
		
	}

	
	infile.close(); 

	return;
	
}

int main(int argc,char* argv[])
{
	
	if(argc!=4)
	{
		cout<<"usage : "<<argv[0]<<" <path-to-graph-file> <graph-type: 1:undirected 2:directed> <edge probability for UN>";
		
		exit(0);
		
	}
	
	vector<string> v;
	
	string file_name, outfolder, filename1, filename2, filename3;
	
	file_name = argv[1];
	
	split(file_name,'/',v);
	
	outfolder="";

	for(int i=0;i<v.size()-1;i++)
	{
		outfolder=outfolder+v[i]+"/";
	}
	
	
	int gtype = atoi(argv[2]);
	
	if(gtype>2||gtype<1)
	{
		cout<<"wrong graph type\n";
		
		exit(0);
		
	}
	
	float prob = atof(argv[3]);
	
	if(prob<=0||prob>1)
	{	
		cout<<"wrong probability value\n";
		
		exit(0);
	}
	
	
	read_graph(file_name,gtype);
	
	
	//printing to file

	//undirected graph

	if(gtype==1)
	{
	
		ofstream myfile;
		
		string filename = outfolder+"/attribute.txt";
		
		myfile.open(filename.c_str());
		
		myfile<<"n="<<node_map.size()<<endl;
		
		int m=0;
		
		for(unordered_map<long long int,int>::iterator it3 = degree.begin();it3!=degree.end();++it3)
		{
			m+=it3->second;
		}
		
		myfile<<"m="<<m<<endl;
		
		myfile.close();
		
		
		filename1 = outfolder+"/graph_ic.inf";
		
		filename2 = outfolder+"/graph_wc.inf";
		
		filename3 = outfolder+"/graph_lt.inf";
		
		
		ofstream myfile1, myfile2, myfile3;
		
		myfile1.open(filename1.c_str());
		
		myfile2.open(filename2.c_str());
		
		myfile3.open(filename3.c_str());

	
		unordered_map<long long int, unordered_map<long long int,int> >:: iterator it;
	
		
		for(it=adjlist.begin();it!=adjlist.end();++it)
		{
			
			unordered_map<long long int,int>::iterator it1;
			
			unordered_map<long long int,int> map1 = it->second;
			
			for(it1 = map1.begin();it1!=map1.end();++it1)
			{
				
				myfile1<<node_map[(*it).first]<<" "<<node_map[(*it1).first]<<" "<<prob<<endl;
				
				myfile2<<node_map[(*it).first]<<" "<<node_map[(*it1).first]<<" "<<1.0/degree[(*it1).first]<<endl;
				
				myfile3<<node_map[(*it).first]<<" "<<node_map[(*it1).first]<<" "<<1.0/degree[(*it1).first]<<endl;
				
			}
			
		}
	
		
		myfile1.close();
		
		myfile2.close();
		
		myfile3.close();

	}
	
	
	//directed graph
	else
	{
		
		ofstream myfile;
		
		string filename = outfolder+"/attribute.txt";
		
		myfile.open(filename.c_str());
		
		myfile<<"n="<<node_map.size()<<endl;
		
		int m=0;
		
		for(unordered_map<long long int,int>::iterator it3 = out_degree.begin();it3!=out_degree.end();++it3)
		{
			m+=it3->second;
		}
		
		myfile<<"m="<<m<<endl;
		
		myfile.close();


		filename1 = outfolder+"/graph_ic.inf";
		
		filename2 = outfolder+"/graph_wc.inf";
		
		filename3 = outfolder+"/graph_lt.inf";
		
		
		ofstream myfile1, myfile2, myfile3;
		
		myfile1.open(filename1.c_str());
		
		myfile2.open(filename2.c_str());
		
		myfile3.open(filename3.c_str());

	
		unordered_map<long long int, unordered_map<long long int,int> >:: iterator it;
	
		
		for(it=adjlist.begin();it!=adjlist.end();++it)
		{
			
			unordered_map<long long int,int>::iterator it1;
			
			unordered_map<long long int,int> map1 = it->second;
			
			for(it1 = map1.begin();it1!=map1.end();++it1)
			{
				
				myfile1<<node_map[(*it).first]<<" "<<node_map[(*it1).first]<<" "<<prob<<endl;
				
				myfile2<<node_map[(*it).first]<<" "<<node_map[(*it1).first]<<" "<<1.0/in_degree[(*it1).first]<<endl;
				
				myfile3<<node_map[(*it).first]<<" "<<node_map[(*it1).first]<<" "<<1.0/in_degree[(*it1).first]<<endl;
				
			}
			
		}
	
		
		myfile1.close();
		
		myfile2.close();
		
		myfile3.close();

		
				
	}
	
	return 0;
}

 
