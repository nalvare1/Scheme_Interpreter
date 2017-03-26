//Alison Lui
//Header and implementation for Node struct

#include <iostream>
#include <string>
#include <vector>
#include "node.h"
using namespace std;

Node::Node(string val, vector<Node*> chldrn){
	value = val;
	children = chldrn;
}

Node::~Node(){
	for(size_t i=0; i<children.size(); i++){
		if(children[i]){
			delete children[i];
		}
	}
}

ostream& operator<<(ostream &os, Node &n){
	os << "(Node: ";
	os << "value=" << n.value;
	for(size_t i=0; i<n.children.size(); i++){
		if(n.children[i]){
			os << ", child=" << *n.children[i];
		}
	}
	os << ")";
	return os;
}

