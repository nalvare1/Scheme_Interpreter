#include<iostream>
#include<sstream>
#include<string>
#include<cstring>
#include<stack>
#include<cmath>
#include<cstdio>
#include<stdlib.h>
#include<vector>
#include<algorithm>
#include "node.h"
using namespace std;

bool isParenOrOperator(char);
bool isNumber(char);
string parse_token(istream&);
Node* parse_expression(istream&);
void usage(int);
void evaluate_r(const Node *n, stack<string> &s);
void mathEval(string, int, stack<string> &);
bool isApostrophe(char);
bool isMathFunc(string);
int gcd_func(int, int);
bool string_is_number(string);

int main(int argc, char* argv[]){
	bool batchMode = false; //disable prompt
	bool debugMode = false; //print tree generated after each input

	//parse command line arguments
	int argind = 1;
	while(argind < argc && strlen(argv[argind]) > 1 && argv[argind][0] == '-') {
		char *arg = argv[argind++];
		switch (arg[1]){
			case 'h':
				usage(0);
			case 'b':
				batchMode = true;
				break;
			case 'd':
				debugMode = true;
				break;
			default:
				usage(1);
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
		stringstream s(line); //make line from stdin a stream
		Node* syntaxtree = parse_expression(s); //turn line into tree
		if(syntaxtree){
			if(debugMode){
				cout << *syntaxtree << endl;
			}
			stack<string> result; //stack to store intermediary results
			evaluate_r(syntaxtree, result); //traverse syntax tree and evaluate all functions
			cout << result.top() << endl; //top of stack should now be final result
		}
	}
	cout << endl;

	return 0;
}

string parse_token(istream &is){
	string token;
	while(isspace(is.peek()) && is.peek() != '-'){ //ignore space
		is.get();
	}
	if(is.peek()=='-'){ //decide if negative or subtraction if negative sign encountered
		token = is.get();
		if(isdigit(is.peek())){
			while(isNumber(is.peek())){
				token += is.get();
			}
		}
	}else if(isParenOrOperator(is.peek())){ //get one character if parenthesis or operator
		token = is.get();
	}else if(isApostrophe(is.peek())){ //implements quote function
		is.get(); //Don't include apostrophe
		token += is.get(); //add first '(' to string
		if(token == "("){
			int openParenCount = 1, closeParenCount = 0; //count parentheses in order to grab everything within outermost parentheses (can't necessarily stop at first close paren)
			while(openParenCount-closeParenCount != 0){
				if(is.peek() == '('){
					openParenCount++;
				}else if(is.peek() == ')'){
					closeParenCount++;
				}
				token += is.get(); //token will be an unevaluated string including parentheses - functions dealing with lists will need to keep in mind the format of this argument which they will be getting from the stack - consider turning this into a stream with sstream when running these functions
			}
		}else if(token == "\""){
			while(is.peek() != '"'){
				token += is.get();
			}
			token += is.get();
		}
	}else if(isalpha(is.peek())){ //get whole word if alpha encountered
		while(isalpha(is.peek())||is.peek()=='?'){
			token += is.get();
		}
		if(token=="quote"){ //other instance of quote func - see above instance for comments
			token = ""; //reset token
			is.get(); //don't include space
			token += is.get();
			if(token == "("){
				int openParenCount = 1, closeParenCount = 0;
				while(openParenCount-closeParenCount != 0){
					if(is.peek() == '('){
						openParenCount++;
					}else if(is.peek() == ')'){
						closeParenCount++;
					}
					token += is.get();
				}
			}else if(token == "\""){
				while(is.peek() != '"'){
					token += is.get();
				}
				token += is.get();
			}
		}
	}else if(isNumber(is.peek())){ //get whole number if digit encountered
		token = is.get();
		while(isNumber(is.peek())){
			token += is.get();
		}
	}else if(is.peek() == '"'){ //get string if quote encountered, backslash not yet supported
		token += is.get();
		while(is.peek() != '"'){
			token += is.get();
		}
		token += is.get(); //quotes on string included so stack can differentiate between function and string
	}
	return token;
}

Node* parse_expression(istream &is){
	string token;
	token = parse_token(is); //get next token from stream
	if(!token[0])
		return nullptr;
	vector<Node*> children = {nullptr}; //initialize to one null child
	if(token==" " || token==")"){
		return nullptr;
	}
	if(token=="("){
		token = parse_token(is); //function name should be right after open paren
		if(token==")") return nullptr;
		int currentChild = 0; //keep track of number of children for indexing purposes below
		children[0] = (parse_expression(is)); //next expression should be first child
		while(children[currentChild] && is.peek() != ')'){ //add all children
			children.push_back(parse_expression(is));
			currentChild++;
		}
		if(children[currentChild]){
			parse_token(is); //remove ')' from stream
		}
	}
	return new Node(token, children);
}

bool isParenOrOperator(char c){
	return ( c=='('|| c==')' || c=='+' || c=='-' || c=='*' || c=='/' || c=='<' || c=='>');
}

bool isNumber(char c){ //includes period in order to include floats
	return ( (c>='0' && c<='9') || c=='.' );
}

bool isMathFunc(string func){ //Will eventually include other math functions
	return ( func == "+" || func == "-" || func == "*" || func == "/"
			|| func == "sqrt" || func == "remainder" || func=="floor" || func=="ceiling"
			|| func=="truncate" || func=="round" || func=="expt" || func == "sin" || func == "cos"
			|| func == "tan" || func == "asin" || func == "acos" || func == "atan" || func =="equal?"
			|| func == "gcd" || func == "lcm" || func == "quotient" || func == "log"
			|| func == "positive?" || func == "negative?" || func=="zero?" || func == "odd?"
			|| func == "even?" || func == "<" || func == ">");
}

bool isApostrophe(char c) {
	return (c == '\'');
}

void usage(int exit_status){
	cout << "Usage: scheme [-b -d]" << endl;
	cout << "    -b Batch mode (disable prompt)" << endl;
	cout << "    -d Debug mode (display messages)" << endl;
	if(!exit_status)
		exit(EXIT_SUCCESS);
	else
		exit(EXIT_FAILURE);
}

void evaluate_r(const Node *n, stack<string> &s){
	int nchildren = 0; //number of children on this node
	for(size_t i=0; i<n->children.size(); i++){
		if(n->children[i]){
			evaluate_r(n->children[i], s);
			nchildren++;
		}
	}
	if(nchildren==0){ //if the node has no children its value is not an operator/function
		if(n->value=="exit") exit(EXIT_SUCCESS);
		s.push(n->value);
	}else{
		string func = n->value;
		stringstream arg_error;
		arg_error << func << ": Wrong number of arguments";
		stringstream list_error;
		list_error << func << ": Non-list argument passed to list function" << endl << "\t" << s.top();
		//Check for different functions here!!
		//Each function will find its arguments (# of args = nchildren)  on top of the stack
		//and should push its return value to the stack
		//(will need to convert to and from strings when using the stack since it's of type string)
		//note: quote is not a function that belongs here because it deals with parsing so it's integrated into parse_expression
		if(isMathFunc(func)){
			mathEval(func, nchildren, s);
		}else if(func=="car"){
			if(nchildren!=1){
				s.push(arg_error.str());
			}else if(s.top()[0] != '(' || s.top()[1] == ')'){
				s.push(list_error.str());
			}else{
				stringstream ss(s.top());
				s.pop();
				parse_token(ss); //remove paren
				s.push(parse_token(ss)); //push first element
			}
		}else if(func=="cdr"){
			if(nchildren!=1){
				s.push(arg_error.str());
			}else if(s.top()[0] != '(' || s.top()[1] == ')'){
				s.push(list_error.str());
			}else{
				string list = s.top();
				s.pop();
				size_t newlistBegin = 0;
				while(list[newlistBegin] != ' '){
					newlistBegin++;
					if(newlistBegin >= list.length()){
						s.push("()");
						return;
					}
				}
				newlistBegin++;
				string newlist = list.substr(newlistBegin, BUFSIZ); 
				newlist.insert(0, "(");
				s.push(newlist);
			}
		}else{
			stringstream ss;
			ss << func << ": Unrecognized function";
			s.push(ss.str());
		}
	}
}

void mathEval(string func, int nargs, stack<string> &s){
	vector<float> args;
	for(int i=0; i<nargs; i++){
		if(!string_is_number(s.top())){
			stringstream ss;
			ss << "Error: invalid argument to function: " << func;
			ss << endl << "\t" << s.top();
			s.push(ss.str());
			return;
		}
		bool negative = false;
		if(s.top()[0] == '-'){
			s.top()[0] = '0';
			negative = true;
		}
		if(negative){
			args.push_back((-1)*stod(s.top())); //add typecasted arg to args vector from top of stack
		}else{
			args.push_back(stod(s.top())); //add typecasted arg to args vector from top of stack
		}
		s.pop(); //remove arg from stack
	}
	stringstream arg_error;
	arg_error << func << ": Wrong number of arguments";
	float result; 
	if(func=="+"){
		result = 0;
		for(size_t i=0; i<args.size(); i++){
			result += args[i];
		}
		stringstream result_stream;
		result_stream << result;
		s.push(result_stream.str());
	}else if(func=="-"){
		result = args.back();
		for(int i=args.size()-2; i>=0; i--){ //backwards will be order of input - important for non-commutative function
			result -= args[i];
		}
		stringstream result_stream;
		result_stream << result;
		s.push(result_stream.str());
	}else if(func=="*"){
		result = 1;
		for(size_t i=0; i<args.size(); i++){
			result *= args[i];
		}
		stringstream result_stream;
		result_stream << result;
		s.push(result_stream.str());
	}else if(func=="/"){
		result = args.back();
		for(int i=args.size()-2; i>=0; i--){ //backwards will be order of input - important for non-commutative function
			result /= args[i];
		}
		stringstream result_stream;
		result_stream << result;
		s.push(result_stream.str());
	}else if(func=="sqrt"){
		if(args.size()>1){
			s.push(arg_error.str());
		}else{
			result = args[0];
			result = sqrt(result);
			stringstream result_stream;
			result_stream << result;
			s.push(result_stream.str());
		}
	}else if(func =="remainder") {
		result = 0;
		if(args.size() > 2) {
			s.push(arg_error.str());
		} else {
			result = (int) args[1] % (int) args[0];
			stringstream result_stream;
			result_stream << result;
			s.push(result_stream.str());
		}
	}else if(func == "floor") {
		if(args.size() > 1) {
			s.push(arg_error.str());
		} else {
			stringstream result_stream;
			result_stream << floor(args[0]);
			s.push(result_stream.str());
		}
	}else if(func == "ceiling") {
		if(args.size() > 1) {
			s.push(arg_error.str());
		} else {
			stringstream result_stream;
			result_stream << ceil(args[0]);
			s.push(result_stream.str());
		}
	}else if(func == "truncate") {
		if(args.size() > 1) {
			s.push(arg_error.str());
		} else {
			stringstream result_stream;
			result_stream << trunc(args[0]);
			s.push(result_stream.str());
		}
	}else if(func == "round") {
		if(args.size() > 1) {
			s.push(arg_error.str());
		} else {
			stringstream result_stream;
			result_stream << round(args[0]);
			s.push(result_stream.str());
		}
	}else if(func=="expt") {
		if(args.size()>2) {
			s.push(arg_error.str());
		} else {
			stringstream result_stream;
			result_stream << pow((double) args[1], (double) args[0]);
			s.push(result_stream.str());
		}
	}else if(func=="sin") {
		if (args.size() > 1) {
			s.push(arg_error.str());
		} else {
			stringstream result_stream;
			result_stream << sin(args[0]);
			s.push(result_stream.str());
		}
	}else if(func == "cos") {
		if (args.size() > 1) {
			s.push(arg_error.str());
		} else {
			stringstream result_stream;
			result_stream << cos(args[0]);
			s.push(result_stream.str());
		}
	}else if(func == "tan") {
		if (args.size() > 1) {
			s.push(arg_error.str());
		} else {
			stringstream result_stream;
			result_stream << tan(args[0]);
			s.push(result_stream.str());
		}
	}else if(func == "asin") {
		if (args.size() > 1) {
			s.push(arg_error.str());
		} else {
			stringstream result_stream;
			result_stream << asin(args[0]);
			s.push(result_stream.str());
		}
	}else if(func == "acos") {
		if (args.size() > 1) {
			s.push(arg_error.str());
		} else {
			stringstream result_stream;
			result_stream << acos(args[0]);
			s.push(result_stream.str());
		}
	}else if(func == "atan") {
		if (args.size() > 1) {
			s.push(arg_error.str());
		} else {
			stringstream result_stream;
			result_stream << atan(args[0]);
			s.push(result_stream.str());
		}
	}else if(func == "equal?") {
		if(args.size() != 2) {
			s.push(arg_error.str());
		} else {
			if(args[0]==args[1]) s.push("#t");
			else s.push("#f");
		}
	} else if(func == "gcd") {
		if(args.size() != 2) {
			s.push(arg_error.str());
		} else {
			int gcd = gcd_func(args[0], args[1]);
			stringstream result_stream;
			result_stream << gcd;
			s.push(result_stream.str());
		}
	} else if(func == "lcm") {
		if(args.size() != 2) {
			s.push(arg_error.str());
		} else {
			int a, b, lcm, gcd;
			a = args[0];
			b = args[1];
			gcd = gcd_func(a, b);
			lcm = (a * b) / gcd;
			stringstream result_stream;
			result_stream << lcm;
			s.push(result_stream.str());
		}
	}else if(func == "quotient"){
		int res = args.back();
		for(int i=args.size()-2; i>=0; i--){
			res /= args[i];
		}
		stringstream result_stream;
		result_stream << res;
		s.push(result_stream.str());
	}else if(func == "log"){
		if(args.size() != 1) {
			s.push(arg_error.str());
		}else{
			double res = log(args[0]);
			stringstream result_stream;
			result_stream << res;
			s.push(result_stream.str());
		}
	}else if(func == "positive?") {
		if(args.size()>1) {
			s.push(arg_error.str());
		} else if(args[0]>0) {
			s.push("#t");
		} else if(args[0]==0 || args[0]<0) {
			s.push("#f");
		}
	}else if(func == "negative?") {
		if(args.size()>1) {
			s.push(arg_error.str());
		} else if(args[0]<0) {
			s.push("#t");
		} else if(args[0]==0 || args[0]>0) {
			s.push("#f");
		}

	}else if(func == "zero?") {
		if(args.size()>1) {
			s.push(arg_error.str());
		} else if(args[0]==0) {
			s.push("#t");
		} else if(args[0]!=0) {
			s.push("#f");
		}
	}else if(func == "odd?") {
		if(args.size()>1) {
			s.push(arg_error.str());
		} else if(floor(args[0])!=args[0]){
			s.push("#f");
		} else if((int)args[0] % 2 != 0) {
			s.push("#t");
		} else {
			s.push("#f");
		}
	}else if(func == "even?") {
		if(args.size()>1) {
			s.push(arg_error.str());
		} else if(floor(args[0])!=args[0]){
			s.push("#f");
		} else if((int)args[0] % 2 == 0) {
			s.push("#t");
		} else {
			s.push("#f");
		}
	} else if(func == "<") {
		if(args.size()>2) {
			s.push(arg_error.str());
		} else if(args[0]>args[1]) {
			s.push("#t");
		} else {
			s.push("#f");
		}
	} else if(func == ">") {
		if(args.size()>2) {
			s.push(arg_error.str());
		} else if(args[0]<args[1]) {
			s.push("#t");
		} else {
			s.push("#f");
		}
	}else{
		s.push("Error: unrecognized math function"); //if an error occurs push the error to the stack
	}
}

int gcd_func(int arg1, int arg2) {
	int a, d, q, r;
	arg1 = abs(arg1);
	arg2 = abs(arg2);

	if(arg1 == arg2) {
		return arg1;
	}
	if(arg1 > arg2) {
		d = arg1;
		r = arg2;
	} else if(arg1 < arg2) {
		d = arg2;
		r = arg1;
	}
	while (r != 0) {
		a = d;
		d = r;
		//a = d*q + r
		q = floor(a/d);
		r = a - d*q;
	}
	//last d is the gcd!!
	return d;
}

bool string_is_number(string s){
	if(s[0] == '-'){
		s[0] = '0';
	}
	return !s.empty() && find_if(s.begin(), s.end(), [](char c) { return !isNumber(c); }) == s.end();
}
