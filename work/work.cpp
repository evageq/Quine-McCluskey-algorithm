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
using namespace std;
bool TESTVALUE = true;
const int INF = 2e9;
map<char, int> preced;
int precedence(char ch) {
	return preced[ch];
}

void gotoxy(short x, short y)
{
	HANDLE    StdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD    coord = { x, y };
	SetConsoleCursorPosition(StdOut, coord);
}
short wherex()
{
	HANDLE                        StdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO    csbi;
	GetConsoleScreenBufferInfo(StdOut, &csbi);
	return int(csbi.dwCursorPosition.X);
}
short wherey()
{
	HANDLE                        StdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO    csbi;
	GetConsoleScreenBufferInfo(StdOut, &csbi);
	return int(csbi.dwCursorPosition.Y);
}

class BoolTable {
	string expr;
	string parsedExpr;
	int varCnt;
	vector<string> variables;
	vector<pair<vector<bool>, bool> > boolTable; // (x1, ..., xn), F
	static const char ONE = '1', ZERO = '0';

	const string operators = "+*";


	string parseExpr(string expr) { // return postfix notation
		vector<string> out;
		stringstream ss; ss << expr;
		string tmp;
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
				string var;
				var.assign(tmp.begin(), tmp.begin() + i);
				out.push_back(var);
				for (; i < tmp.size(); ++i) {
					while (oper.top() != '(') {
						out.push_back(string());
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
					out.push_back(string());
					out.back().push_back(o2);
				}
				oper.push(o1);
			}
			else if (tmp.size() != 0) {
				out.push_back(tmp);
			}
		}
		while (oper.empty() == false) {
			out.push_back(string());
			out.back().push_back(oper.top());
			oper.pop();
		}
		string ans;
		for (auto p : out) {
			ans += p;
			ans.push_back(' ');
		}
		return ans;
	}

	// calculate only parsed expr in postfix 
	bool calculateExpr(vector<bool> vec, string _expr) {
		map<string, bool> _value;
		for (int i = 0; i < vec.size(); ++i)
			_value[variables[i]] = vec[i];

		vector<string> _exprVec;
		stringstream ss; ss << _expr;
		string tmp;
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
				_exprVec[i] = string();

				if (flag)
					_exprVec[i].push_back(ZERO);
				else
					_exprVec[i].push_back(ONE);
			}
			else {
				_exprVec[i] = string();

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

	vector<bool> calculateBinary(int a) {
		vector<bool> v(varCnt);
		for (int i = 0; i < varCnt; ++i) {
			v[i] = (1 << i) & (a);
		}
		reverse(v.begin(), v.end());
		return v;
	}

	// count variables, type them in lexic order in variables array
	void parseVariables(string expr) { // expr - postfix notation string
		int _varCnt = 0;
		vector<string> _expr;
		stringstream ss; ss << expr;
		string tmp;
		set<string> _variables;
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

	vector<pair<vector<bool>, bool> > createBoolTable() {
		vector<pair<vector<bool>, bool> > _boolTable;
		_boolTable.resize(1 << varCnt);

		for (int i = 0; i < _boolTable.size(); ++i)
			_boolTable[i].first = calculateBinary(i);

		for (int i = 0; i < _boolTable.size(); ++i)
			_boolTable[i].second = calculateExpr(_boolTable[i].first, parsedExpr);
		return _boolTable;
	}
	vector<pair<vector<bool>, bool> > createBoolTable(string _vec) { //bianry vec: 000111010...
		vector<pair<vector<bool>, bool> > _boolTable;
		_boolTable.resize(1 << varCnt);

		for (int i = 0; i < _boolTable.size(); ++i)
			_boolTable[i].first = calculateBinary(i);

		for (int i = 0; i < _boolTable.size(); ++i) {
			_boolTable[i].second = _vec[i] == '1' ? true : false;
		}
		return _boolTable;
	}
public:
	BoolTable() {

	}
	BoolTable(string _expr) {
		expr = _expr;
		parsedExpr = parseExpr(_expr);
		parseVariables(parsedExpr);
		boolTable = createBoolTable();

	}
	BoolTable(string _vec, bool _) {
		if (((_vec.size() * 4) & (_vec.size() * 4 - 1)) != 0) abort();
		varCnt = log2(_vec.size() * 4);
		string tmpVariables = "";
		char ch = 'a';
		for (int i = 0; i < varCnt; ++i) {
			tmpVariables.push_back(ch);
			tmpVariables.push_back(' ');
			++ch;
		}
		parseVariables(tmpVariables);
		string binaryVec = "";
		for (int i = 0; i < _vec.size(); ++i){
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
	const BoolTable& operator = (const BoolTable& M) {
		if (&M == this) return *this;

		this->expr = M.expr;
		this->varCnt = M.varCnt;
		this->parsedExpr = M.parsedExpr;
		this->variables = M.variables;
		this->boolTable = M.boolTable;
		return *this;

	}
	
	vector<string> getVariables() {
		return variables;
	}
	void printTable() {
		cout << expr << endl;
		int width = 0;
		for (int i = 0; i < variables.size(); ++i)
			width = max(width, (int)variables[i].size());
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
			printf("%*c|\n", width, boolTable[i].second?'1':'0');
			for (int i = 0; i < (width + 1) * (variables.size() + 1); ++i)
				printf("%c", '-');
			printf("\n");
			//cout << boolTable[i].second;
			//cout << endl;
		}
	}
	int getVariablesCnt() {
		return varCnt;
	}
	bool getFuncValue(int i) {
		return boolTable[i].second;
	}
	set<string> getMinterms() {
		set<string> minterms;
		int n = this->getVariablesCnt();
		for (int i = 0; i < (1 << n); ++i) {
			if (this->getFuncValue(i)) {
				string tmp;
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
	
};

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
class Quine_McCluskey_algorithm {
	BoolTable boolTable;
	string minimizedFunction;
	set<string> implicants;
	map<string, bool> _used;


	map<string, pair<COORD, COORD>> __crd;
	const string __NUMBEROF1 = "Number of 1's  ";
	const string __IMPLEVEL = " Implicants level ";
	map<pair<int, int>, set<string> > __groupedImpl; // implicants with i ones, in j group, i <= varCnt, j <= varCnt

	string convert(string implicant, const vector<string>& variables) {
		string ret;
		for (int i = 0; i < implicant.size(); ++i) {
			if (implicant[i] == '0') {
				ret.push_back('!');
				ret += variables[i];
			}
			if (implicant[i] == '1') {
				ret += variables[i];
			}
		}
		return ret;
	}
	string quine_McCluskey_algorithm(set<string> minterms, const vector<string>& variables, int varCnt) {
		implicants = getImplicants(minterms, varCnt);
		vector<string> implicantsVec;
		vector<string> mintermsVec;
		for (auto p : implicants) {
			implicantsVec.push_back(p);
		}
		for (auto p : minterms) {
			mintermsVec.push_back(p);
		}
		string req = "";
		set<string> minimized = minimize(mintermsVec, implicantsVec, req, varCnt).second;
		vector<string> tmpImplicants;
		for (auto p : minimized) {
			tmpImplicants.push_back(convert(p, variables));
		}
		string ans = "";
		for (int i = 0; i < tmpImplicants.size(); ++i) {
			ans += tmpImplicants[i];
			if (i != tmpImplicants.size() - 1) {
				string oper = " + ";
				ans += oper;
			}
		}
		for (auto& p : __groupedImpl) {
			set<string> newSt;
			for (auto _mint : p.second) {
				if (_used[_mint]) {
					string tmp = "+";
					tmp += _mint;
					newSt.insert(tmp);
				}
				else
					newSt.insert(_mint);
			}
			p.second = newSt;
		}
		return ans;
	}

	bool isCover(string minterm, string implicant) {
		bool ans = true;
		for (int i = 0; i < minterm.size(); ++i) {
			if (implicant[i] != '-') {
				ans &= minterm[i] == implicant[i];
			}
		}
		return ans;
	}
	int countRang(set<string> impl) {
		int r = 0;
		for (auto p : impl) {
			for (int i = 0; i < p.size(); ++i) {
				if (p[i] == '1' || p[i] == '0') {
					++r;
				}
			}
		}
		return r;
	}
	pair<int, set<string> > minimize(const vector<string>& minterms, const vector<string>& implicants, string req, int varCnt) {

		map<string, bool> used;
		set<string> usingImplicants;
		for (int i = 0; i < req.size(); ++i) {
			if (req[i] == '1') {
				usingImplicants.insert(implicants[i]);
			}
		}
		for (int i = 0; i < minterms.size(); ++i) {
			bool flag = false;
			for (auto imp : usingImplicants) {
				flag |= isCover(minterms[i], imp);
			}
			used[minterms[i]] = flag;
		}
		bool isAllCover = true;
		for (auto p : minterms) {
			isAllCover &= used[p];
		}
		if (isAllCover) {
			int rang = countRang(usingImplicants);
			return make_pair(rang, usingImplicants);
		}
		else {
			if (req.size() >= implicants.size()) {
				return make_pair(INF, set<string>());
			}
			string tmpLeft = req;
			tmpLeft.push_back('0');
			auto left = minimize(minterms, implicants, tmpLeft, varCnt);

			string tmpRight = req;
			tmpRight.push_back('1');
			auto right = minimize(minterms, implicants, tmpRight, varCnt);
			if (left.first < right.first) {
				return left;
			}
			else {
				return right;
			}
		}
	}
	set<string> getImplicants(set<string> minterms, int varCnt, int level = 1) {
		if (level == 1) {
			map<int, set<string>> _OnesMint;
			for (auto _mint : minterms) {
				_OnesMint[countOnes(_mint)].insert(_mint);
			}
			for (auto p : _OnesMint) {
				__groupedImpl[make_pair(p.first, 0)] = p.second;
			}
			//TESTVALUE = false;
		}
		bool is_end = true;

		//map<string, bool> used;
		set<string> newMinterms;
		vector<vector<string> > groups(varCnt + 1);
		for (auto p : minterms) {
			groups[countOnes(p)].push_back(p);
		}
		bool flag;
		for (int group = 0; group < varCnt; ++group) {
			for (int i = 0; i < groups[group].size(); ++i) {
				string a = groups[group][i];

				if (a.back() == '*') {
					newMinterms.insert(a);
					continue;
				}

				flag = true;
				for (int j = 0; j < groups[group + 1].size(); ++j) {
					string b = groups[group + 1][j];
					string tmpa = a;
					pair<int, int> dif = difference(tmpa, b);
					if (dif.first == 1) {
						_used[a] = _used[b] = true;
						tmpa[dif.second] = b[dif.second] = '-';
						newMinterms.insert(b);
						flag = false;

					}
				}
				is_end &= flag;

				if (flag && !_used[a]) {
					a.push_back('*');
					newMinterms.insert(a);
				}
			}
		}
		if (is_end) {
			set<string> st;
			for (auto p : minterms) {
				if (p.back() == '*') {
					string tmp;
					tmp.assign(p.begin(), p.end() - 1);
					//minterms.erase(p);
					st.insert(tmp);
				}
				else {
					st.insert(p);
				}
			}

			return st;
		}
		else { // add implicants after mering

			for (auto _mint : newMinterms) {
				__groupedImpl[make_pair(countOnes(_mint), level)].insert(_mint);
			}
			
		}
		// Fill the group
			
		
		//
		return getImplicants(newMinterms, varCnt, level + 1);

	}

public:

	Quine_McCluskey_algorithm(const BoolTable& _boolTable) {
		boolTable = _boolTable;
		this->minimizedFunction = 
			quine_McCluskey_algorithm(
				this->boolTable.getMinterms(),
				this->boolTable.getVariables(),
				this->boolTable.getVariablesCnt()
			);
		
	}
	void printPrimeImplicantTable(const BoolTable& _boolTable, const vector<string>& _implicants, const vector<string>& _minterms) {
		short startx = 0;
		short starty = wherey() + 2;
		string _MINTERMS = "| Minterms |";
		map<string, pair<COORD, COORD>> coordinates; // [start;end) of the printed str respectively

/*		
		gotoxy(startx, starty);
		for (int i = 0; i < _MINTERMS.size() - 1; ++i) cout << '-';
		gotoxy(startx, starty + 1);

		

		coordinates[_MINTERMS].first = { wherex(), wherey() };
		cout << _MINTERMS;
		coordinates[_MINTERMS].second = { wherex(), wherey() };
		gotoxy(coordinates[_MINTERMS].first.X, coordinates[_MINTERMS].second.Y + 1);
		//for (int i = 0; i < (int)_MINTERMS.size()-1; ++i) cout << '-';

		gotoxy(coordinates[_MINTERMS].second.X - 1, coordinates[_MINTERMS].second.Y - 2); //_MINTERMS.size() - 1
		string _IMPLICANTS = "| Implicants |";
		coordinates[_IMPLICANTS].first = { wherex(), wherey() };
		cout << _IMPLICANTS;
		coordinates[_IMPLICANTS].second = { wherex(), wherey() };
*/

		//print minterms
		/*
			...
			---
			minterms
			---
			...
		*/
		for (int i = 0; i < _minterms.size(); ++i) {
			if (i != 0) {
				gotoxy(coordinates[_minterms[i - 1]].first.X, coordinates[_minterms[i - 1]].second.Y + 2);
			}
			else{
				
				gotoxy(startx, starty);
				for (int j = 0; j < _minterms[i].size() + 4; ++j)
					cout << '-';
				gotoxy(startx, starty + 1);

			}
			string tmp = "| ";
			tmp += _minterms[i];
			tmp += string(" |");
			coordinates[_minterms[i]].first = { wherex(), wherey() };
		
			cout << tmp;
			coordinates[_minterms[i]].second = { wherex(),wherey() };
			
			gotoxy(coordinates[_minterms[i]].first.X, coordinates[_minterms[i]].second.Y + 1);
			for (int j = 0; j < tmp.size(); ++j)
				cout << '-';
		}
		// print Implicants
		assert(implicants.size() != 0);
		for (int i = 0; i < _implicants.size(); ++i) {
			if (i != 0) {
				gotoxy(coordinates[_implicants[i - 1]].second.X - 1, coordinates[_implicants[i - 1]].second.Y);
			}
			else {
				gotoxy(coordinates[_minterms[0]].second.X - 1, coordinates[_minterms[0]].second.Y - 2);
			}
			string tmp = "| ";
			tmp += _implicants[i];
			tmp += string(" |");
			coordinates[_implicants[i]].first = { wherex(), wherey() };
			cout << tmp;
			coordinates[_implicants[i]].second = { wherex(), wherey() };
		}
		// print vert '-'
		for (int i = 0; i < _minterms.size()+1; ++i) {
			if (i == 0) {
				gotoxy(coordinates[_minterms[0]].first.X, coordinates[_minterms[0]].second.Y - 1);
			}
			else {
				gotoxy(coordinates[_minterms[i - 1]].first.X, coordinates[_minterms[i - 1]].second.Y + 1);
			}
			for (int j = 0; j < coordinates[_implicants.back()].second.X; ++j){
				cout << '-';
			}
		}
		// print horizontal '|'
		for (int i = 0; i < _implicants.size() + 1; ++i) {
			if (i == 0) {
				gotoxy(coordinates[_implicants[0]].first.X, coordinates[_implicants[0]].first.Y);
			}
			else {
				gotoxy(coordinates[_implicants[i - 1]].second.X-1, coordinates[_implicants[i - 1]].second.Y);
			}
			for (int j = 0; j < _minterms.size() + 1; ++j) {
				cout << '|';
				gotoxy(wherex()-1, wherey() + 2);
			}
		}
		// print matches implicants with mintems
		int midPoint = (_implicants[0].size() + 4) / 2;
		for (int i = 0; i < _implicants.size(); ++i) {
			for (int j = 0; j < _minterms.size(); ++j) {
				gotoxy(coordinates[_implicants[i]].first.X + midPoint, coordinates[_minterms[j]].second.Y);
				if (isCover(_minterms[j], _implicants[i])) {
					cout << 'X';
				}
			}
		}

	}
	void printPrimeImplicantTable() {
		vector<string> _minterms;
		for (auto p : this->boolTable.getMinterms())
			_minterms.push_back(p);
		vector<string> _implicants;
		for (auto p : this->implicants)
			_implicants.push_back(p);
		printPrimeImplicantTable(this->boolTable, _implicants, _minterms);
		//gotoxy(wherex(), wherey() + 1);
	}
	void printMergingChart(map<pair<int, int>, set<string> > _groupedImpl) {
		set<int> _cnt; // number of ones column
		set<int> _levelsOnes;
		set<int> _levelsImp;
		COORD mx = { 0,0 };
		int _s = _groupedImpl.begin()->second.begin()->size() - 1;

		for (auto p : _groupedImpl) {
			_levelsOnes.insert(p.first.first);
			_levelsImp.insert(p.first.second);
		}
		bool _first = true;
		for (auto level : _levelsImp) {
			pair<COORD, COORD> prevPrinted;
			if (_first) {
				_first = false;
				gotoxy(0, wherey() + 2);
				cout << "Implicants using to merge are marked with \'+\'\n";
				cout << "Implicants incapable merging are marked with \'*\'\n\n";
				__crd[__NUMBEROF1].first = { wherex(), wherey() };
				cout << __NUMBEROF1;
				__crd[__NUMBEROF1].second = { wherex(), wherey() };
				string tmp = __IMPLEVEL;
				tmp.push_back('0');
				__crd[tmp].first = { wherex(), wherey() };
				cout << tmp;
				__crd[tmp].second = { wherex(), wherey() };


				for (auto _m : this->boolTable.getMinterms()) {
					_cnt.insert(countOnes(_m));
				}
				gotoxy(__crd[tmp].first.X + 9 - _s / 2, __crd[tmp].second.Y + 2);
				// 8 - s/2
				string Brdi = "Brd";
				for (auto i : _cnt) {
					string _cur = Brdi + to_string(i);

					__crd[_cur].first = { __crd[__NUMBEROF1].first.X, wherey() };
					for (auto _mint : _groupedImpl[make_pair(i, 0)]) {
						cout << _mint;
						COORD crdMint = { wherex(), wherey() };
						mx = maxCoord(mx, crdMint);
						gotoxy(__crd[tmp].first.X + 9 - _s / 2, wherey() + 1);
						
					}
					gotoxy(__crd[tmp].first.X, wherey());
					for (int k = __crd[tmp].first.X; k < __crd[tmp].second.X + 2; ++k) cout << '-';
					__crd[_cur].second = { __crd[__NUMBEROF1].first.X, short(wherey() - 1) };
					gotoxy(__crd[tmp].first.X + 9 - _s / 2, wherey() + 1);
				}
				// print ---
				for (auto i : _cnt) {
					string tmpBrd = string("Brd") + to_string(i);
					gotoxy(__crd[tmpBrd].first.X, __crd[tmpBrd].first.Y);
					cout << i;
					// fill up
					gotoxy(__crd[tmpBrd].first.X, __crd[tmpBrd].first.Y - 1);
					for (int k = 0; k < __crd[__NUMBEROF1].second.X; ++k) cout << '-';
					// fill down
					//gotoxy(__crd[tmpBrd].second.X, __crd[tmpBrd].second.Y + 1);
					//for (int k = 0; k < 25; ++k) cout << '-';
				}
				// Print
				// Nimbers of 1s |
				//               |
				//               | ...
				gotoxy(__crd[__NUMBEROF1].second.X - 1, __crd[__NUMBEROF1].second.Y);
				/*for (int k = __crd[__NUMBEROF1].first.Y; k <= __crd[string("Brd") + to_string(*(_cnt.rbegin()))].second.Y; ++k) {
					cout << '|';
					gotoxy(wherex() - 1, wherey() + 1);
				}*/



			}
			else {
				string tmpImpLvl = __IMPLEVEL + to_string(level);
				string prevImpLvl = __IMPLEVEL + to_string(level - 1);
				gotoxy(__crd[prevImpLvl].second.X + 2, __crd[prevImpLvl].second.Y);
				__crd[tmpImpLvl].first = { wherex(), wherey() };
				cout << tmpImpLvl;
				__crd[tmpImpLvl].second = { wherex(), wherey() };

				gotoxy(__crd[tmpImpLvl].first.X - 1, __crd[tmpImpLvl].second.Y);
				//for (int k = __crd[__NUMBEROF1].first.Y; k <= __crd[string("Brd") + to_string(*(_cnt.rbegin()))].second.Y; ++k) {
				
				gotoxy(__crd[tmpImpLvl].first.X + 9 - _s/2, __crd[tmpImpLvl].second.Y + 2);
				for (auto levelOnes : _levelsOnes) {
					bool _isExist = false;
					for (auto _mint : __groupedImpl[make_pair(levelOnes, level)]) {
						cout << _mint;
						COORD crdMint = { wherex(), wherey() };
						mx = maxCoord(mx, crdMint);
						gotoxy(__crd[tmpImpLvl].first.X + 9 - _s / 2, wherey() + 1);
						_isExist = true;
					}
					if (_isExist) {
						gotoxy(__crd[tmpImpLvl].first.X, wherey());
						for (int k = __crd[tmpImpLvl].first.X; k <= __crd[tmpImpLvl].second.X; ++k) cout << '-';
						gotoxy(__crd[tmpImpLvl].first.X + 9 - _s / 2, wherey() + 1);
					}
				}
			}

		}
		
		_first = true;
		for (auto level : _levelsImp) {
			if (_first) {
				_first = false;
				gotoxy(__crd[__NUMBEROF1].second.X, __crd[__NUMBEROF1].second.Y);
				for (int k = __crd[__NUMBEROF1].first.Y; k <= mx.Y; ++k) {
					cout << '|';
					gotoxy(wherex() - 1, wherey() + 1);

				}
			}
			else {
				string _tmp = __IMPLEVEL + to_string(level);
				gotoxy(__crd[_tmp].first.X, __crd[_tmp].second.Y);
				for (int k = __crd[_tmp].second.Y; k <= mx.Y; ++k) {
					cout << '|';
					gotoxy(wherex() - 1, wherey() + 1);
				}
			}
		}
		
		gotoxy(__crd[__NUMBEROF1].first.X, mx.Y+1);
		for (int k = __crd[__NUMBEROF1].first.X; k <= __crd[__IMPLEVEL + to_string(*(_levelsImp.rbegin()))].second.X; ++k)
			cout << '-';
		gotoxy(__crd[__IMPLEVEL + to_string(*(_levelsImp.rbegin()))].second.X, __crd[__IMPLEVEL + to_string(*(_levelsImp.rbegin()))].second.Y);
		for (int k = __crd[__IMPLEVEL + to_string(*(_levelsImp.rbegin()))].second.Y; k <= mx.Y ; ++k) {
			cout << '|';
			gotoxy(wherex() - 1, wherey() + 1);
		}
	}
	void printMergingChart() {
		printMergingChart(this->__groupedImpl);
	}
	string GetMinimizedFunction() {
		return minimizedFunction;
	}
};
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
	
	string expr;
	expr = "3333BBBB";
	//getline(cin, expr);
	BoolTable table(expr, true);
	cout << "\t\t\t\tFunction table\n\n";
	table.printTable();
	cout << "\n\n";
	
	Quine_McCluskey_algorithm quine(table);
	cout << "\t\t\t\t\tMerging chart\n\n";
	quine.printMergingChart();
	cout << "\n\n";
	
	cout << quine.GetMinimizedFunction();
	cout << "\t\t\t\t\t\tPrime Implicants Table\n\n";
	quine.printPrimeImplicantTable();
	//table.printTable();
	cout << "\n\n\n";
	cout << "\t\t\t\t\tMinimized Function\n\n";
	string ans = quine.GetMinimizedFunction();
	cout << endl <<"   "<<ans << endl << endl;
	
	
	getchar();
}