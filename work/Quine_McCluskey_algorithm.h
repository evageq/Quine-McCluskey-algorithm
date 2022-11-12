#pragma once
#include "BoolTable.h"
#include <string>
#include <vector>
#include <map>
#include <set>
#include <iostream>
#include <sstream>
#include <Windows.h>

using std::string;
using std::vector;
using std::set;
using std::map;
using std::pair;
using std::cout;


class Quine_McCluskey_algorithm {
	BoolTable boolTable;
	string minimizedFunction;
	set<string> implicants;
	map<string, bool> _used;


	map<string, pair<COORD, COORD>> __crd;
	const string __NUMBEROF1 = "Number of 1's  ";
	const string __IMPLEVEL = " Implicants level ";
	map<pair<int, int>, set<string> > __groupedImpl; // implicants with i ones, in j group, i <= varCnt, j <= varCnt
public:
	string convert(string implicant, const vector<string>& variables);
	string quine_McCluskey_algorithm(set<string> minterms, const vector<string>& variables, int varCnt);

	bool isCover(string minterm, string implicant);

	int countRang(set<string> impl);

	pair<int, set<string> > minimize(const vector<string>& minterms, const vector<string>& implicants, string req, int varCnt);
	set<string> getImplicants(set<string> minterms, int varCnt, int level = 1);

	Quine_McCluskey_algorithm(const BoolTable& _boolTable);
	void printPrimeImplicantTable(const BoolTable& _boolTable, const vector<string>& _implicants, const vector<string>& _minterms);
	void printPrimeImplicantTable();
	void printMergingChart(map<pair<int, int>, set<string> > _groupedImpl);
	void printMergingChart();
	string GetMinimizedFunction();

};
