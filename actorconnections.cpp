//Name: Yuhang lian
//Pid: A91018743
//Name: Bo Pang
//Pid:A91115104
//program that come up with the year that connects two actor together, using bfs or union-find 
//which is determined by the user
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <queue>
#include "ActorGraph.hpp"
#include "disjoint.hpp"
using namespace std;

void findYear(ActorGraph& main, const char* in_filename, const char* out_filename, string option);

void bfsFind(ActorGraph& main, string actorName1,string actorName2, ofstream& out);

void unionFind(ActorGraph& main, string actorName1,string actorName2, ofstream& out);


void Timer::begin_timer() {
    
    start = std::chrono::high_resolution_clock::now();
    
}

/*
 * Ends the timer. Compares end time with the start time and returns number of nanoseconds
 */
long long Timer::end_timer() {
    
    std::chrono::time_point<std::chrono::high_resolution_clock> end;
    end = std::chrono::high_resolution_clock::now();
    
    return (long long)std::chrono::duration_cast<std::chrono::nanoseconds>(end-start).count();
    
}

//main method processes the passed in argument user typed
int main(int argc, char* argv[]){
	char* dicFileName = argv[1];
	char* testFile = argv[2];
	char* output = argv[3];
	string option = argv[4];
	ActorGraph* main = new ActorGraph();
	main->loadFromFile(dicFileName,true);
						cout<<"test"<<endl;

	if(option.compare("bfs")==0){
		findYear(*main, testFile,output, "bfs");

	}
	else if(option.compare("ufind")==0){
		findYear(*main, testFile,output, "ufind");
	}
	else{
		cout<<"Wrong last argument!Must be either bfs or ufind"<<endl;		
	}
}

//based on the constructed graph, read the pair file and choose a option, bfs or ufind, to come up with
//the connecting year
void findYear(ActorGraph& main, const char* in_filename, const char* out_filename, string option){
	ifstream infile(in_filename);
	ofstream outfile(out_filename);
	bool have_header = false;
	// keep reading lines until the end of file is reached
	outfile<<"Actor1"<<"\t"<<"Actor2"<<"\t"<<"Year"<<endl;
    Timer tX;
    cout<<"RUNNING"<<endl;
    tX.begin_timer();
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
        auto ax = main.mby;
		string actorName1(record[0]);
		string actorName2(record[1]);
		if(option.compare("bfs")==0){
			bfsFind(main, actorName1,actorName2,outfile);
		}
		else if(option.compare("ufind")==0){
			unionFind(main, actorName1,actorName2,outfile);
		}	
        //this for loop restore graph to oigin
        main.mby = ax;
		for(string a : main.names){
			main.actorList[a]->done = false;
			main.actorList[a]->dist = 4294967295;
			main.actorList[a]->pervious = 0;
           	main.actorList[a]->previousMovie = "";
		}
	}
    long result = tX.end_timer();
    cout<<"TIME TAKEN = "<<result<<endl;
	infile.close();
	outfile.close();	
}

//using bfs searching algorithm to find the year from which two passed in actors connected 
void bfsFind(ActorGraph& main, string actorName1,string actorName2, ofstream& out){
    //if not found any of the actor, print 9999
    if(main.actorList[actorName1] == 0 ||main.actorList[actorName2] ==0){
        out<<actorName1<<"\t"<<actorName2<<"\t"<<9999<<endl;
        return;
    }
    out<<actorName1<<"\t"<<actorName2<<"\t";
    int connectedYear;
    for(connectedYear=1942;connectedYear<2016;connectedYear++){
        std::queue<vertex*> storage;
        //enqueue the actor1
        vertex* itr = main.actorList[actorName1];
        storage.push(itr);
        
        //create a vector to store the vertices been reached and
        //set their done back to false
        vector<vertex*> recover;
        while(storage.size()>0){
            vertex* ver = storage.front();
            storage.pop();
            recover.push_back(ver);
            if(ver->actor.compare(actorName2)==0){
                out<<connectedYear<<endl;
                return;
            }
            
            if(!ver->done){
                ver->done = true;
                for(edge* e : ver->edgeName){
                    //reversely get the year from weight
                    int year = 2016- e->weights;
                    if(year>connectedYear) continue;
                    storage.push(e->vertexloc);
                }
            }
        }
        for(vertex* ver : recover){
            ver->done = false;
        }
    }
    //if did not find a path to actor2, print 9999
    out<<9999<<endl;
}



//this is the union find method which takes in 2 names and find the year they being connected
void unionFind(ActorGraph& main, string actorName1,string actorName2, ofstream& out){
    //if any of the names dne
    if(main.actorList[actorName1] == 0 || main.actorList[actorName2] == 0){
        out<<actorName1<<"\t"<<actorName2<<"\t"<<"9999";
        return;
    }
    //the following part insert all the node in to the tree
    set* std = new set();
    //go though all the name;
    for(string a : main.names){
        node* newA = new node();
        newA->name = a;
        std->sentinals[a] = newA;
        std->lists[a] = newA;
    }
    //use the priority queue to restore
    while(!main.mby.empty()){
        movie* cur = main.mby.top().second;
        //loop though all the actors in each movie
        for(vertex* a : cur->actors){
            if(cur->actors[0]->actor.compare(a->actor)!=0){
                std->Union(cur->actors[0]->actor, a->actor);
            }
            if(std->find(actorName1) == std->find(actorName2)){
                out<<actorName1<<"\t"<<actorName2<<"\t"<<main.mby.top().first<<endl;
                return;
            }
        }
        main.mby.pop();
    }
}














