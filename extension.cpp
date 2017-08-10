//Name: Yuhang lian
//Pid: A91018743
//Name: Bo Pang
//Pid:A91115104
//my extension will read in the favorite actors of the user and output some suggested movies
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <queue>
#include <chrono>
#include <thread>
#include "ActorGraph.hpp"

using namespace std;
using namespace std::this_thread; // sleep_for, sleep_until
using namespace std::chrono; // nanoseconds, system_clock, seconds

void suggestByActor(ActorGraph& main, vector<string> actors);


int main(int argc, char* argv[]){
	char* dicFileName = argv[1];
	ActorGraph* main = new ActorGraph();
	main->loadFromFile(dicFileName,false);
	cout<<endl<<"***************************************"<<endl;
	cout<<"Welcome to the Movie Suggestion System!"<<endl;
	cout<<"***************************************"<<endl;
    sleep_until(system_clock::now() + seconds(2));
	cout<<"Please enter the names of your favorite actors!"<<endl;
	cout<<"We will give your some suggested movies based on them!"<<endl;
    sleep_until(system_clock::now() + seconds(1));
    cout<<"   *Notice: please type in each name followed by a return key!"<<endl;
    cout<<"   *UpperCase Only! Firstname followed by comma, a space, and then last name"<<endl;
    cout<<"   *when you finish, please type in an END"<<endl;
    vector<string> actors;
    while(true){
    	string input = "";
    	getline(cin,input);
    	if(input.compare("END")==0) break;
		if(main->actorList[input]==0) {
			cout<<"   *Actor Not Found"<<endl;
			cout<<"   *Sorry for the limitation of our database"<<endl;
			cout<<"   *Please try again!"<<endl;
			continue;
		}
		actors.push_back(input);    
    }
    cout<<"Data processing...."<<endl;
    suggestByActor(*main,actors);
    cout<<"Finished! Please check suggestion.tsv file in the current directory"<<endl;



}

void suggestByActor(ActorGraph& main, vector<string> actors){
	ofstream out;
	out.open("suggestion.tsv");
	out<<"*********Suggested Movies********"<<endl;
	out<<"Movie name"<<"-----"<<"Year"<<"-----"<<"Actors"<<endl;
	int priority = actors.size();
	priority_queue<pair<int,string>> q;
	// vector<pair<int,string> > actorPriority;

	for(string actor: actors){
		// pair<int,string> temp (priority,actor);
		// actorPriority.push_back(temp);
		for(movie* m: main.actorList[actor]->movieList){
			if(m->pr==0) m->pr = priority;
			else m->pr +=priority;
		}
		priority--;

	}


	priority_queue<pair<int,movie*>> moviePriority;
	for(string actor: actors){
		for(movie* m: main.actorList[actor]->movieList){
			if(m->done) {
				continue;
			}
			m->done = true;
			pair<int,movie*> moviepair (m->pr,m);
			moviePriority.push(moviepair);
		}

	}
	int count = 10;
	while(count>0&&!moviePriority.empty()){
		int y = moviePriority.top().second->year;
		string movieName = moviePriority.top().second->name;
		out<<movieName<<"-----"<<y<<"-----";
		for(vertex* ver: moviePriority.top().second->actors){
			for(string actor: actors){
				if(actor.compare(ver->actor)==0){
					out<<" <"<<actor<<"> ";
				}
			}
		}
		moviePriority.pop();
		out<<endl;
		count--;
	}

	out.close();


}



















