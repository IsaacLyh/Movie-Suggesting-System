//Name: Yuhang lian
//Pid: A91018743
//Name: Bo Pang
//Pid:A91115104
/*
* ActorGraph.cpp
* Author: Adrian Guthals
* Date: 2/24/2015
*
* This file is meant to exist as a container for starter code that you can use to read the input file format
* defined in movie_casts.tsv. Feel free to modify any/all aspects as you wish.
*/

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "ActorGraph.hpp"

using namespace std;

ActorGraph::ActorGraph() {
	this->root = new vertex();
}

bool vertex::operator<(const vertex& other) const{
	return dist > other.dist;
}

bool vertex::comp(vertex* one, vertex* other) const{
	return *one < *other;
}


//helper method, deal with the situation that if the graph is still empty yet
void newNode(ActorGraph* in,int movie_year,string movie_title,string actor_name){
    in->root->actor = actor_name;
    //pushin root;
    in->actorList[actor_name] = in->root;
    // cout<<this->actorList[actor_name]<<endl;
    //create new movie
    movie* cur = new movie();
    cur->name = movie_title;
    cur->year = movie_year;
    vector<vertex*> actors;
    actors.push_back(in->root);
    cur->actors = actors;
    in->root->movieList.push_back(cur);
    in->movies[movie_year].push_back(cur);
    pair<int,movie*> push(movie_year,cur);
    in->mby.push(push);
    //update movie to all location
    vector<edge*> edgeset;
    in->root->edgeName = edgeset;
}

//helper method used to add movie to the current actor
void actorWithNoMovie(bool use_weighted_edges,movie* itr ,ActorGraph* in , int movie_year,string movie_title,string actor_name){
    in->actorList[actor_name]->movieList.push_back(itr);
    //the movie was already created as well as the actor;
    itr->actors.push_back(in->actorList[actor_name]);
    //update edges;
    //find the cre\ated movie and update the relation
    for(vertex* a : itr->actors){
        if(a->actor.compare(actor_name)!=0){
            edge* newE = new edge();
            newE->vertexName = actor_name;
            newE->vertexloc = a;
            newE->related = itr;
            if(use_weighted_edges) newE->weights = 1+(2015-movie_year);
            in->actorList[actor_name]->edgeName.push_back(newE);
            edge* newE2 = new edge();
            newE2->vertexName = a->actor;
            newE2->vertexloc = in->actorList[actor_name];
            newE2->related = itr;
            if(use_weighted_edges) newE2->weights = 1+(2015-movie_year);
            // cout<<"weight1"<<newE2->weights<<endl;
            a->edgeName.push_back(newE2);
        }
    }
}

//if the actor already added to the graph but the movie not
void actorExist(bool use_weighted_edges,movie* itr,ActorGraph* in, string actor_name, string movie_title,int movie_year){
    //if the actor was not created
    //but the movie was created;
    vertex* newV = new vertex();
    newV->actor = actor_name;
    // cout<<"this actor dne but movie created = "<<newV->actor<<endl;
    itr->actors.push_back(newV);//update the actor to movie
    //pushin root;
    in->actorList[actor_name] = newV;
    in->actorList[actor_name]->movieList.push_back(itr);
    //find the created movie and update the relation
    for(vertex* a : itr->actors){
        // cout<<"for movie "<<itr->name<<endl;
        if(a->actor.compare(actor_name)!=0){
            edge* newE = new edge();
            newE->vertexName = actor_name;
            newE->vertexloc = a;
            newE->related = itr;
            if(use_weighted_edges) newE->weights = 1+(2015-movie_year);
            newV->edgeName.push_back(newE);
            edge* newE2 = new edge();
            newE2->vertexName = a->actor;
            newE2->vertexloc = newV;
            newE2->related = itr;
            if(use_weighted_edges) newE2->weights = 1+(2015-movie_year);
            // cout<<"weight2"<<newE2->weights<<endl;
            a->edgeName.push_back(newE2);
            // cout<<"first edge pointed to = "<<newE->vertexloc->actor<<endl;
            // cout<<"second edge created = "<<newE2->vertexloc->actor<<endl;
        }
    }
}
    
//main method that load data into the graph    
bool ActorGraph::loadFromFile(const char* in_filename, bool use_weighted_edges)
{

// Initialize the file stream
	cout<<"Data loading...."<<endl;
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

		if (record.size() != 3)
		{
// we should have exactly 3 columns
			continue;
		}

		string actor_name(record[0]);
		string movie_title(record[1]);
		int movie_year = stoi(record[2]);
		this->names.push_back(actor_name);

/* debug note :  Actor exist and movie dne justified
*                Actor dne but movie exist justified
*                Actor and movie exist justified
*                Actor and movie both dne justified
*                Algorithm correct
*/

// we have an actor/movie relationship, now what?
//TODO implement with argu true(need to change some of the code);
		if(this->root->actor.compare("") == 0){
		    newNode(this,movie_year,movie_title,actor_name);
		}
		else{
			if(this->actorList[actor_name]!=0){
			//if the actor has already being created
				movie* itr;
				bool exist = false;
				//check if the movie is also created
				for(movie* a : this->movies[movie_year]){
					if(a->name.compare(movie_title)== 0){
						exist = true;
						itr = a;
					}
				}
				if(!exist){
				//create new movie, initial its private fields
					movie* cur = new movie();
					cur->name = movie_title;
					cur->year = movie_year;
					vector<vertex*> actors;
					actors.push_back(this->actorList[actor_name]);
					cur->actors = actors;

					//push the new movie to actor's list;
					this->actorList[actor_name]->movieList.push_back(cur);
					this->movies[movie_year].push_back(cur);
		            pair<int,movie*> push(movie_year,cur);
		            this->mby.push(push);
					//update movie to all location
				}
				else{
		            actorWithNoMovie(use_weighted_edges,itr,this, movie_year, movie_title,actor_name);
		        }
		    }
			else{
				//if the actor has not been created
				movie* itr;
				bool exist = false;
				for(movie* a : this->movies[movie_year]){
					if(a->name.compare(movie_title)== 0){
						exist = true;
						itr = a;
					}
				}
				//check the movie exist ?
				if(exist){
		            actorExist(use_weighted_edges,itr,this, actor_name, movie_title,movie_year);
				}
				else{
				//if the actor was not created
				//and the movie neither;
					vertex* newV = new vertex();
					newV->actor = actor_name;
				//pushin root;
					this->actorList[actor_name] = newV;
				//create new movie
					movie* cur = new movie();
					cur->name = movie_title;
					cur->year = movie_year;
					vector<vertex*> actors;
					actors.push_back(newV);
					cur->actors = actors;
					newV->movieList.push_back(cur);
				    pair<int,movie*> push(movie_year,cur);
				    this->mby.push(push);
					this->actorList[actor_name]->movieList.push_back(cur);
					this->movies[movie_year].push_back(cur);
				//update movie to all location
					vector<edge*> edgeset;
					newV->edgeName = edgeset;
				}

			}
		}
	}
	cout<<"finish loading."<<endl;

	if (!infile.eof())
	{
		cerr << "Failed to read " << in_filename << "!\n";
		return false;
	}
	infile.close();

	return true;
}




ActorGraph::~ActorGraph(){
	//deletes edges
	for(string actorName: this->names){
		for(auto& e: this->actorList[actorName]->edgeName){	
			if(e) delete e;
			e=0;
		}
	}
	//delete movies
	for(auto p : movies) {
		for (auto m : p.second) {
			delete m;
		}

	}
	//delete vertices
	for(auto al : actorList) {
		delete al.second;

	}
	
}











