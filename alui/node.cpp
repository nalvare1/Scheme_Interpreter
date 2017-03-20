//Alison Lui
//Header and implementation for Node struct

#include <iostream>
#include <string>
#include "node.h"
using namespace std;

Node::Node(string val, Node * leftptr, Node * rightptr){
	value = val;
	left = leftptr;
	right = rightptr;
}

Node::~Node(){
	if(left){
		delete left;
	}
	if(right){
		delete right;
	}
}

ostream& operator<<(ostream &os, Node &n){
	os << "(Node: ";
	os << "value=" << n.value;
	if(n.left){
		os << ", left=" << *n.left;
	}
	if(n.right){
		os << ", right=" << *n.right;
	}
	os << ")";
	return os;
}

