//Alison Lui
//Header and implementation for Node struct

#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <string>
#include <vector>
using namespace std;

class Node {
	friend ostream& operator<< (ostream &, Node &);
	public:
		Node(string="", vector<Node*> = {nullptr});
		~Node();
		string value;
		vector<Node*> children;
};

#endif
