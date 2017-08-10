//Name: Yuhang lian
//Pid: A91018743
//Name: Bo Pang
//Pid:A91115104
//this file implement certain methods called on disjoint set
#include <iostream>
#include <vector>
#include <unordered_map>
#include "disjoint.hpp"
bool connected(set* in ,string name1, string name2);


//this method will union two input string
//by finding its node first then point its root
//to sentinel node
void set::Union(string name1, string name2){
    //check if these two nodes are already connected
    if(connected(this,name1,name2)){
        return;
    }
    //find the sentinel node
    find(name2)->parent = find(name1);
}

//this method test if two nodes are connected
bool connected(set* in ,string name1, string name2){
    node* cur = in->lists[name1];
    //compare the sentinel node of the two nodes
    if(in->find(name1)==in->find(name2)){
        return true;
    }
    return false;
}

//this method keep going up till reach the sentinel node
node* set::find(string name1){
    //create a pointer
    node* cur = this->lists[name1];
    while(cur->parent !=0){
        //move up till reach sentinel
        cur = cur->parent;
    }
   // cout<<cur<<endl;
    return cur;
}