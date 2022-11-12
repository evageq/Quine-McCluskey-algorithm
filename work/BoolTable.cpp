#include "BoolTable.h"
#include <sstream>
#include <iostream>
#include <algorithm>
#include <stack>
using std::stack;
using std::cout;
using std::endl;

std::string BoolTable::parseExpr(std::string expr) { // return postfix notation
	vector<std::string> out;
	std::stringstream ss; ss << expr;
	std::string tmp;
	stack<char> oper;
	while (getline(ss, tmp, ' ')) {
		if (tmp[0] == '(') {
			int i = 0;
			for (; i < tmp.size() && tmp[i] == '('; ++i) {
				oper.push('(');
			}
			tmp.erase(tmp.begin(), tmp.begin() + i);
		}
		if (tmp.back() == ')') {
			int i = tmp.find(')');
			std::string var;
			var.assign(tmp.begin(), tmp.begin() + i);
			out.push_back(var);
			for (; i < tmp.size(); ++i) {
				while (oper.top() != '(') {
					out.push_back(std::string());
					out.back().push_back(oper.top());
					oper.pop();
				}
				oper.pop();
			}
			tmp = "";
		}
		if (operators.find(tmp) != std::string::npos) {
			char o1 = tmp[0];
			while (oper.empty() == false && oper.top() != '(' &&
				(precedence(oper.top()) > precedence(o1)))
			{
				char o2 = oper.top();
				oper.pop();
				out.push_back(std::string());
				out.back().push_back(o2);
			}
			oper.push(o1);
		}
		else if (tmp.size() != 0) {
			out.push_back(tmp);
		}
	}
	while (oper.empty() == false) {
		out.push_back(std::string());
		out.back().push_back(oper.top());
		oper.pop();
	}
	std::string ans;
	for (auto p : out) {
		ans += p;
		ans.push_back(' ');
	}
	return ans;
}
// calculate only parsed expr in postfix 
bool BoolTable::calculateExpr(vector<bool> vec, std::string _expr) {
	std::map<std::string, bool> _value;
	for (int i = 0; i < vec.size(); ++i)
		_value[variables[i]] = vec[i];

	vector<std::string> _exprVec;
	std::stringstream ss; ss << _expr;
	std::string tmp;
	while (getline(ss, tmp, ' ')) {
		if (tmp.size() > 0 && (tmp[0] == '!' || isalpha(tmp[0]) || isdigit(tmp[0]) || operators.find(tmp[0]) != std::string::npos))
			_exprVec.push_back(tmp);
	}

	for (int i = 0; i < _exprVec.size(); ++i) {
		if (operators.find(_exprVec[i]) != std::string::npos) continue;
		bool flag = false;
		if (_exprVec[i][0] == '!') {
			flag = true;
			_exprVec[i].erase(_exprVec[i].begin());
		}
		if (_value[_exprVec[i]]) {
			_exprVec[i] = std::string();

			if (flag)
				_exprVec[i].push_back(ZERO);
			else
				_exprVec[i].push_back(ONE);
		}
		else {
			_exprVec[i] = std::string();

			if (flag)
				_exprVec[i].push_back(ONE);
			else
				_exprVec[i].push_back(ZERO);
		}
	}

	stack<bool> calc;
	for (int i = 0; i < _exprVec.size(); ++i) {
		if (_exprVec[i].find(ONE) != std::string::npos || _exprVec[i].find(ZERO) != std::string::npos) {
			if (_exprVec[i].find(ONE) != std::string::npos)
				calc.push(true);
			else
				calc.push(false);
		}
		else {
			bool a, b;
			a = calc.top(); calc.pop();
			b = calc.top(); calc.pop();
			bool F;
			if (_exprVec[i].find("+") != std::string::npos)
				F = a || b;
			else
				F = a && b;
			calc.push(F);
		}
	}
	return calc.top();
}
vector<bool> BoolTable::calculateBinary(int a) {
	vector<bool> v(varCnt);
	for (int i = 0; i < varCnt; ++i) {
		v[i] = (1 << i) & (a);
	}
	reverse(v.begin(), v.end());
	return v;
}
void BoolTable::parseVariables(std::string expr) { // expr - postfix notation std::string
	int _varCnt = 0;
	vector<std::string> _expr;
	std::stringstream ss; ss << expr;
	std::string tmp;
	set<std::string> _variables;
	while (getline(ss, tmp, ' ')) {
		_expr.push_back(tmp);
	}
	for (auto str : _expr) {
		if (str.size() > 0 && (isalpha(str[0]) || isdigit(str[0]) || str[0] == '!')) {
			if (str.front() == '!') str.erase(str.begin());
			if (_variables.find(str) == _variables.end())
				++_varCnt;
			_variables.insert(str);
		}
	}
	varCnt = _varCnt;
	for (auto p : _variables)
		variables.push_back(p);
	sort(variables.begin(), variables.end());
}

vector<pair<vector<bool>, bool> > BoolTable::createBoolTable() {
	vector<pair<vector<bool>, bool> > _boolTable;
	_boolTable.resize(1 << varCnt);

	for (int i = 0; i < _boolTable.size(); ++i)
		_boolTable[i].first = calculateBinary(i);

	for (int i = 0; i < _boolTable.size(); ++i)
		_boolTable[i].second = calculateExpr(_boolTable[i].first, parsedExpr);
	return _boolTable;
}

vector<pair<vector<bool>, bool> > BoolTable::createBoolTable(std::string _vec) { //bianry vec: 000111010...
	vector<pair<vector<bool>, bool> > _boolTable;
	_boolTable.resize(1 << varCnt);

	for (int i = 0; i < _boolTable.size(); ++i)
		_boolTable[i].first = calculateBinary(i);

	for (int i = 0; i < _boolTable.size(); ++i) {
		_boolTable[i].second = _vec[i] == '1' ? true : false;
	}
	return _boolTable;
}

BoolTable::BoolTable() {

}
BoolTable::BoolTable(std::string _expr) {
	expr = _expr;
	parsedExpr = parseExpr(_expr);
	parseVariables(parsedExpr);
	boolTable = createBoolTable();

}
BoolTable::BoolTable(std::string _vec, bool _) {
	if (((_vec.size() * 4) & (_vec.size() * 4 - 1)) != 0) abort();
	varCnt = log2(_vec.size() * 4);
	std::string tmpVariables = "";
	char ch = 'a';
	for (int i = 0; i < varCnt; ++i) {
		tmpVariables.push_back(ch);
		tmpVariables.push_back(' ');
		++ch;
	}
	parseVariables(tmpVariables);
	std::string binaryVec = "";
	for (int i = 0; i < _vec.size(); ++i) {
		int tmp;
		if ('0' <= _vec[i] && _vec[i] <= '9') {
			tmp = _vec[i] - '0';
		}
		else {
			tmp = 10 + _vec[i] - 'A';
		}
		for (int j = 3; j >= 0; --j) {
			bool flag = (1 << j) & tmp;
			binaryVec.push_back(flag ? '1' : '0');
		}
	}
	boolTable = createBoolTable(binaryVec);
}
const BoolTable& BoolTable::operator = (const BoolTable& M) {
	if (&M == this) return *this;

	this->expr = M.expr;
	this->varCnt = M.varCnt;
	this->parsedExpr = M.parsedExpr;
	this->variables = M.variables;
	this->boolTable = M.boolTable;
	return *this;

}

vector<std::string> BoolTable::getVariables() {
	return variables;
}
void BoolTable::printTable() {
	cout << expr << endl;
	int width = 0;
	for (int i = 0; i < variables.size(); ++i)
		width = std::max(width, (int)variables[i].size());
	++width;
	for (int i = 0; i < variables.size(); ++i) {
		//cout << variables[i] << ' ';
		printf("%*s%c", width, variables[i].c_str(), i == variables.size() - 1 ? '|' : ' ');
	}
	printf("%*c|\n", width, 'F');
	for (int i = 0; i < (width + 1) * (variables.size() + 1); ++i)
		printf("%c", '-');
	printf("\n");
	//cout << 'F' << endl;
	for (int i = 0; i < boolTable.size(); ++i) {
		for (int j = 0; j < boolTable[i].first.size(); ++j) {
			printf("%*c%c", width, boolTable[i].first[j] ? '1' : '0', j == boolTable[i].first.size() - 1 ? '|' : ' ');
			//cout << boolTable[i].first[j] << ' ';
		}
		printf("%*c|\n", width, boolTable[i].second ? '1' : '0');
		for (int i = 0; i < (width + 1) * (variables.size() + 1); ++i)
			printf("%c", '-');
		printf("\n");
		//cout << boolTable[i].second;
		//cout << endl;
	}
}
int BoolTable::getVariablesCnt() {
	return varCnt;
}
bool BoolTable::getFuncValue(int i) {
	return boolTable[i].second;
}
set<std::string> BoolTable::getMinterms() {
	set<std::string> minterms;
	int n = this->getVariablesCnt();
	for (int i = 0; i < (1 << n); ++i) {
		if (this->getFuncValue(i)) {
			std::string tmp;
			tmp.resize(n);
			for (int j = 0; j < n; ++j) {
				bool flag = (1 << j) & (i);
				tmp[j] = (flag ? '1' : '0');
			}
			reverse(tmp.begin(), tmp.end());
			minterms.insert(tmp);
		}
	}
	return minterms;
}
