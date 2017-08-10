//Name: Yuhang lian
//Pid: A91018743
//Name: Bo Pang
//Pid:A91115104
#include <iostream>
#include <vector>
#include <unordered_map>
#include <chrono>
using namespace std;

class Timer{
private:
    std::chrono::time_point<std::chrono::high_resolution_clock> start;
    
public:
    
    /*
     * Function called when starting the timer.
     */
    void begin_timer();
    
    /*
     * Function called when ending the timer. Returns duration in nanoseconds
     * PRECONDITION: begin_timer() must be called before this function
     */
    long long end_timer();
    
    
};


class node{
public:
    string name;
    node* parent;
    unordered_map<string,bool> connected;
    node(){
        parent = 0;
    }
};

class set{
public:
    unordered_map<string, node*> sentinals;
    unordered_map<string, node*> lists;
    set(){
        
    }
    //this is the union method
    void Union(string name1, string name2);
    //this is the find method
    node* find(string name1);
};





