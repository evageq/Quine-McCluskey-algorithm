#pragma once

#include <vector>
#include <set>
#include <string>
#include <map>

using std::vector;
using std::set;
using std::pair;
int precedence(char ch);

class BoolTable {
	std::string expr;
	std::string parsedExpr;
	int varCnt;
	vector<std::string> variables;
	vector<pair<vector<bool>, bool> > boolTable; // (x1, ..., xn), F
	static const char ONE = '1', ZERO = '0';

	const std::string operators = "+*";

	// return postfix notation
	std::string parseExpr(std::string expr); 

	// calculate only parsed expr in postfix 
	bool calculateExpr(vector<bool> vec, std::string _expr);

	vector<bool> calculateBinary(int a);

	// count variables, type them in lexic order in variables array
	void parseVariables(std::string expr);

	vector<pair<vector<bool>, bool> > createBoolTable();

	vector<pair<vector<bool>, bool> > createBoolTable(std::string _vec);

public:
	BoolTable();
	BoolTable(std::string _expr);
	BoolTable(std::string _vec, bool _);
	const BoolTable& operator = (const BoolTable& M);

	vector<std::string> getVariables();
	void printTable();
	int getVariablesCnt();
	bool getFuncValue(int i);
	set<std::string> getMinterms();
	
};

