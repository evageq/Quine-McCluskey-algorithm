/*
	Quine McCluskey algorithm
	input constraints
		- there are two binary operators: "+, *"
		- binary operators should be surrounded with spaces
		- Valid Parentheses order
		- unary operator "!" precede variable name without spaces
	Valid Inputs:
		!a * !b * !c + !a * !b * c + a * !b * !c + a * !b * c + a * b * !c
		(a + b) * !c
		...
*/



#include <Windows.h>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <stack>
#include <sstream>
#include <set>
#include <algorithm>
#include <map>
#include <cassert>
#include <conio.h>

#include "BoolTable.h"
#include "MoveCoord.h"
#include "Quine_McCluskey_algorithm.h"

using namespace std;
bool TESTVALUE = true;
const int INF = 2e9;
map<char, int> preced;
int precedence(char ch) {
	return preced[ch];
}


pair<int, int> difference(string a, string b) { // count last id of differnet values and amount of differneces (dif, id)
	pair<int, int> dif(0, 0);
	for (int i = 0; i < a.size(); ++i) {
		//if (a[i] == '*' || b[i] == '*') continue;
		if (a[i] != b[i]) {
			dif.first++;
			dif.second = i;
		}
	}
	return dif;
}
int countOnes(string a) {
	int tmp = 0;
	for (int i = 0; i < a.size(); ++i)
		if (a[i] == '1') ++tmp;
	return tmp;
}


COORD maxCoord(COORD a, COORD b) {
	if (a.Y > b.Y)
		return a;
	else if (a.Y == b.Y) {
		if (a.X > b.X)
			return a;
		return b;
	}
	return b;
}

string _parse(string a) {

	string tmp;
	for (int i = 0; i < a.size(); ++i) {
		if (a[i] == '1')
			tmp.push_back('a' + i);
		else {
			tmp.push_back('!');
			tmp.push_back('a' + i);
		}
	}
	return tmp;
}

int32_t main() {
	preced['+'] = 1;
	preced['*'] = 2;
	
	cout << "Choose a variant of input\n";
	cout << "1. Vector of function, using hex. Example: 23577999\n";
	cout << "2. An expresion. Example: ad + de + ab!e + ac!e + b!cd + a!b!ce + !abce\n";

	string expr;
	int cmd;
	while ((scanf_s("%d", &cmd, 1000)) != 1 || (cmd != 1 && cmd != 2)) {
		cout << "No such command. Try again\n";
	}
	cin.ignore(10000, '\n');
	getline(cin, expr);

	BoolTable table;
	
	if (cmd == 1) {
		table = BoolTable(expr, true);
	}
	else if (cmd == 2) {
		table = BoolTable(expr);
	}
	
	cout << "\n\n" << "Function table" << '\n';
	table.printTable();
	cout << "\n\n";
	
	Quine_McCluskey_algorithm quine(table);
	cout << "Merging chart\n";
	quine.printMergingChart();
	cout << "\n\n";
	
	cout << "Prime Implicants Table\n";
	quine.printPrimeImplicantTable();

	cout << "\n\n";
	cout << "Minimized Function\n";
	string ans = quine.GetMinimizedFunction();
	cout << endl <<"   "<<ans << endl << endl;
	
	
	getchar();
	return 0;
}