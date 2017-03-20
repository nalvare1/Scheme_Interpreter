//Alison Lui
//Header and implementation for Node struct

#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <string>
using namespace std;

class Node {
	friend ostream& operator<< (ostream &, Node &);
	public:
		Node(string="", Node* = nullptr, Node* = nullptr);
		~Node();
		string value;
		Node * left;
		Node * right;
};

#endif
