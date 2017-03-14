//Natalie Alvarez (nalvare1)
//Scheme Interpreter
//My Commit for Lab 6

#include <iostream>
#include <cctype>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include<string.h>
using namespace std;

#ifndef SCHEME_H
#define SCHEME_H

class Scheme {
	public:
		Scheme();
		~Scheme();
		void setCommand(char);
		char getCommand(int);
		void increaseSize();
		int getSize();
		void runCommand();
		void quote();
	private:
		vector <char> command;
		int size;

};

Scheme::Scheme() {
	size = 0;
}

Scheme::~Scheme() {}

void Scheme::setCommand(char a) {
	command.push_back(a);
	increaseSize();
}

char Scheme::getCommand(int n) { return command[n]; }
void Scheme::increaseSize() { size = size + 1; }
int Scheme::getSize() { return size; }

void Scheme::runCommand() {
	if(getCommand(0) != '(') { //|| getCmmand(1) == "q"???(does it store "qute" or q?
		quote();
	}
}

void Scheme::quote() {
	for(int i = 1; i < getSize(); i++) {
										
		cout << getCommand(i);//!!!FIX SPACING!!!!!!
											
	}
	cout << "\n";
}

#endif

int main(int argc, char *argv[]) {
	Scheme scheme;
	char stream;
	cout << ">>> ";
	while (cin >> stream && stream != '\n') {
		scheme.setCommand(stream);
		//cout << stream; //for testing ONLY!!
	}
	scheme.runCommand();
	return 0;
}
