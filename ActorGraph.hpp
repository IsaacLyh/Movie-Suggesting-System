//Name: Yuhang lian
//Pid: A91018743
//Name: Bo Pang
//Pid:A91115104
/*
 * ActorGraph.hpp
 * Author: Adrian Guthals
 * Date: 2/24/2015
 *
 * This file is meant to exist as a container for starter code that you can use to read the input file format
 * defined in movie_casts.tsv. Feel free to modify any/all aspects as you wish.
 */

#ifndef ACTORGRAPH_HPP
#define ACTORGRAPH_HPP
#include <queue>
#include <iostream>
#include <vector>
#include <unordered_map>
// Maybe include some data structures here

using namespace std;
class movie;
class vertex;

//this is the edge class
/* it contain the name of vertex
 * and the vertex point to
 * weight is initialize to 1
 * movie is the movie that link 2 actors;
 */
class edge{
public:
    vertex* vertexloc;
    string vertexName;
    int weights;
    movie* related;
    edge(){
        weights = 1;
    }
    ~edge(){}
};

/*this is the vertex class
 *store movies the actor acted
 *connections he or she have
 * and fields for findpaths
 */
class vertex{
public:
    string actor;
    vector<movie*> movieList;
    vector<edge*> edgeName;
    bool done;
    unsigned int dist;
    vertex* pervious;
    string previousMovie;
    vertex(){
        previousMovie = "";
        done = false;
        dist = 4294967295;//maximum int
    }
~vertex(){}
    bool operator<(const vertex& other) const;
    bool comp(vertex* one, vertex* other) const;
};

/*this is the movie
 *it has its actors
 *and year name
 */
class movie{
public:
    string name;
    int year;
    vector<vertex*> actors;
    int pr;
    movie(){
      pr=0;
    }
    bool done;
~movie(){}

};



class ActorGraph {
protected:
  // Maybe add class data structure(s) here

public:
  ActorGraph(void);
    vertex* root;   //checked
    vector<string> names;  //checked
    unordered_map<string,vertex*> actorList;
    unordered_map<int,vector<movie*> > movies;
    priority_queue<pair<int,movie* >,vector<pair<int,movie*> >, greater<pair<int,movie*> > > mby;
  // Maybe add some more methods here
  
  /** You can modify this method definition as you wish
   *
   * Load the graph from a tab-delimited file of actor->movie relationships.
   *
   * in_filename - input filename
   * use_weighted_edges - if true, compute edge weights as 1 + (2015 - movie_year), otherwise all edge weights will be 1
   *
   * return true if file was loaded sucessfully, false otherwise
   */
  bool loadFromFile(const char* in_filename, bool use_weighted_edges);
    ~ActorGraph();
};


#endif // ACTORGRAPH_HPP
