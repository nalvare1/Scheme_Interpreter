//Alison Lui

#include<iostream>
#include<sstream>
#include<string>
#include<cstring>
#include "node.h"
using namespace std;

bool isParenOrOperator(char);
bool isDigit(char);
string parse_token(istream&);
Node* parse_expression(istream&);
void usage();

int main(int argc, char* argv[]){
	bool batchMode = false;
	bool debugMode = false;

	//parse command line arguments
	if(argc > 1){
		if(!strcmp(argv[1],"-b")){
			batchMode = true;
		}else if(!strcmp(argv[1],"-d")){
			debugMode = true;
		}else if(!strcmp(argv[1],"-h")){
			usage();
			return 0;
		}else{
			usage();
			return 1;
		}
	}
	if(argc > 2){
		if(!strcmp(argv[2],"-b")){
			batchMode = true;
		}else if(!strcmp(argv[2],"-d")){
			debugMode = true;
		}else if(!strcmp(argv[2],"-h")){
			usage();
			return 0;
		}else{
			usage();
			return 1;
		}
	}

	//Main execution
	while(!cin.eof()){
		if(!batchMode){
			cout << ">>> ";
		}
		string line;
		if (!getline(cin, line)){
			break;
		}
		stringstream s(line);
		Node* syntaxtree = parse_expression(s);
		if(debugMode){
			cout << *syntaxtree;
		}
	}

	return 0;
}

string parse_token(istream &is){
	string token;
	while(isspace(is.peek())){
		is.get();
	}
	if(isParenOrOperator(is.peek())){
		token = is.get();
	}else if(isDigit(is.peek())){
		token = is.get();
		while(isDigit(is.peek())){
			token += is.get();
		}
	}
	return token;
}

Node* parse_expression(istream &is){
	string token;
    token = parse_token(is);
	Node* left = nullptr;
    Node* right = nullptr;
	if(token==" " || token==")"){
		return nullptr;
	}
	if(token=="("){
		token = parse_token(is);
		left = parse_expression(is);
		if(left){
			right = parse_expression(is);
		}
		if(right){
			parse_token(is);
		}
	}
	return new Node(token, left, right);
}

bool isParenOrOperator(char c){
	return ( c=='('|| c==')' || c=='+' || c=='-' || c=='*' || c=='/' );
}

bool isDigit(char c){
	return ( c>='0' && c<='9' );
}

void usage(){
	cout << "Usage: uscheme [-b -d]" << endl;
	cout << "    -b Batch mode (disable prompt)" << endl;
	cout << "    -d Debug mode (display messages)" << endl;
}


