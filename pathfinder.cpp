//Name: Yuhang lian
//Pid: A91018743
//Name: Bo Pang
//Pid:A91115104
//this program will read in pairs of actors from inputfile and output their unweighted/weighted 
//path between each other

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <queue>
#include "ActorGraph.hpp"


using namespace std;

void findpath(ActorGraph& main , const char* in_filename, ofstream& outF,bool use_weighted_edges);

void algorithmFind(ActorGraph& main, string actorName1,string actorName2,ofstream& outX);

int main(int argc, char* argv[]){

	char* dicFileName = argv[1];
	string option = argv[2];
	char* testFile = argv[3];
	char* output = argv[4];
	ofstream out;
    	out.open(output);
	ActorGraph* main = new ActorGraph();
  	out<<"(actor)--[movie#@year]-->(actor)--..."<<endl;
	if(option.compare("w")==0){
		main->loadFromFile(dicFileName, true);
	    findpath(*main,testFile,out,true);
	}
	else if(option.compare("u")==0){
		main->loadFromFile(dicFileName, false);
	    findpath(*main,testFile,out,false);

	}
	else{
		cout<<"Wrong second argument!Must be either u or w"<<endl;
	}
	delete main;
    	out.close();
}

//method that process the input file and take the pair of actors into the next method
void findpath(ActorGraph& main , const char* in_filename, ofstream& outF,bool use_weighted_edges){
	ActorGraph& newG = main;
	ifstream infile(in_filename);
	bool have_header = false;
	// keep reading lines until the end of file is reached
	while (infile)
	{
		string s;
		// get the next line
		if (!getline( infile, s )) break;
		if (!have_header)
		{
			// skip the header
			have_header = true;
			continue;
		}
		istringstream ss( s );
		vector <string> record;
		while (ss)
		{
			string next;
			// get the next string before hitting a tab character and put it in 'next'
			if (!getline( ss, next, '\t' )) break;
			record.push_back( next );
		}
		if (record.size() != 2)
		{
			// we should have exactly 2 columns
			continue;
		}
		string actorName1(record[0]);
		string actorName2(record[1]);
		algorithmFind(newG,actorName1,actorName2,outF);
        //this for loop restore graph to origin
		for(string a : newG.names){
			newG.actorList[a]->done = false;
			newG.actorList[a]->dist = 4294967295;
			newG.actorList[a]->pervious = 0;
            newG.actorList[a]->previousMovie = "";
		}
	}
	infile.close();
}


//method that contains the algorithm that find the path between the passed two actors
//and output their path into the outputfile
void algorithmFind(ActorGraph& main,string actorName1,string actorName2,ofstream& outX){
    //if the actor dne-- return;
	if(main.actorList[actorName1] == 0){
		outX<<actorName1<<"not found"<<endl;
		return;
	}
    //create vectors to store
	vector<pair<string,string> > output;
	vertex* itr = main.actorList[actorName1];
	priority_queue<pair<int,vertex*> ,vector<pair<int,vertex*> >,greater<pair<int,vertex*> > > q;
    //initialize corresponding fields for findpath;
	itr->dist = 0;
	itr->pervious = 0;
    itr->previousMovie = "";
	pair<int,vertex*> enqueue(0,itr);
	q.push(enqueue);
	priority_queue<pair<int,vector<string> >,vector<pair<int,vector<string>> >, greater<pair<int,vector<string>> > > out;
    //while the queue is not empty
	while(!q.empty()){
        //get the top item then go down
		pair<int,vertex*> top = q.top();
		q.pop();
		// cout<<top.second->actor<<endl;
		if(!(top.second->done)){
			top.second->done = true;
            //go though all the edges in one node
			for(edge* a : top.second->edgeName){
				//cout<<a->weights<<endl;
				int distc = top.first+ a->weights;
				if(distc < a->vertexloc->dist){
					//cout<<distc<< " = distc"<<endl;
					//cout<<a->vertexloc->dist<<" = vertex dist"<<endl;
					a->vertexloc->pervious = top.second;
                    			stringstream ss;
                    			ss<<a->related->year;
                    			string str = ss.str();
                    			string ax =a->related->name+"#@"+str;
                    			a->vertexloc->previousMovie = ax;
					//cout<<a->vertexloc->actor<<" = actor"<<endl;
					a->vertexloc->dist = distc;
					//cout<<a->vertexloc->actor<<endl;
					//cout<<a->vertexloc->actor<<endl;
					if(a->vertexloc->actor.compare(actorName2) == 0){
						vertex* itr = a->vertexloc;
						int count  =0 ;
						string str;
               			                vector<string> reverse;
                        //push all its pervious in to the set;
						while(itr->pervious!=0){
                            			reverse.push_back(itr->actor);
                            			reverse.push_back(itr->previousMovie);
						//cout<<itr->pervious->actor<<endl;
						itr = itr->pervious;
						count++;
						}
						pair<int,vector<string> > store(distc,reverse);
						// cout<<distc<<endl;
						out.push(store);
					}
					pair<int,vertex*> enq(distc,a->vertexloc);
					// cout<<a->vertexloc->actor<<endl;
					q.push(enq);
				}
			}
		}
	}
    bool mode = 1;
    //this part output the required format
    outX<<"("<<actorName1<<")";
    for(int pos = out.top().second.size()-1;pos>=0;pos--){
        string a  = out.top().second[pos];
        if(mode){
            outX<<"--["<<a<<"]-->";
            mode = 0;
        }
        else{
            //actor name;
            outX<<"("<<a<<")";
            mode = 1;
        }
    }
    outX<<endl;
}































