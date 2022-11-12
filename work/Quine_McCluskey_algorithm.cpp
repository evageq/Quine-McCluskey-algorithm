#include "Quine_McCluskey_algorithm.h"
#include "MoveCoord.h"
#include <assert.h>

const int INF = 2e9;
using std::make_pair;
using std::to_string;

int countOnes(string a);
pair<int, int> difference(string a, string b);
COORD maxCoord(COORD a, COORD b);

string Quine_McCluskey_algorithm::convert(string implicant, const vector<string>& variables) {
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
string Quine_McCluskey_algorithm::quine_McCluskey_algorithm(set<string> minterms, const vector<string>& variables, int varCnt) {
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

bool Quine_McCluskey_algorithm::isCover(string minterm, string implicant) {
	bool ans = true;
	for (int i = 0; i < minterm.size(); ++i) {
		if (implicant[i] != '-') {
			ans &= minterm[i] == implicant[i];
		}
	}
	return ans;
}
int Quine_McCluskey_algorithm::countRang(set<string> impl) {
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
pair<int, set<string> > Quine_McCluskey_algorithm::minimize(const vector<string>& minterms, const vector<string>& implicants, string req, int varCnt) {

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
set<string> Quine_McCluskey_algorithm::getImplicants(set<string> minterms, int varCnt, int level) {
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



	Quine_McCluskey_algorithm::Quine_McCluskey_algorithm(const BoolTable& _boolTable) {
		boolTable = _boolTable;
		this->minimizedFunction =
			quine_McCluskey_algorithm(
				this->boolTable.getMinterms(),
				this->boolTable.getVariables(),
				this->boolTable.getVariablesCnt()
			);

	}
	void Quine_McCluskey_algorithm::printPrimeImplicantTable(const BoolTable& _boolTable, const vector<string>& _implicants, const vector<string>& _minterms) {
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
			else {

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
		for (int i = 0; i < _minterms.size() + 1; ++i) {
			if (i == 0) {
				gotoxy(coordinates[_minterms[0]].first.X, coordinates[_minterms[0]].second.Y - 1);
			}
			else {
				gotoxy(coordinates[_minterms[i - 1]].first.X, coordinates[_minterms[i - 1]].second.Y + 1);
			}
			for (int j = 0; j < coordinates[_implicants.back()].second.X; ++j) {
				cout << '-';
			}
		}
		// print horizontal '|'
		for (int i = 0; i < _implicants.size() + 1; ++i) {
			if (i == 0) {
				gotoxy(coordinates[_implicants[0]].first.X, coordinates[_implicants[0]].first.Y);
			}
			else {
				gotoxy(coordinates[_implicants[i - 1]].second.X - 1, coordinates[_implicants[i - 1]].second.Y);
			}
			for (int j = 0; j < _minterms.size() + 1; ++j) {
				cout << '|';
				gotoxy(wherex() - 1, wherey() + 2);
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
	void Quine_McCluskey_algorithm::printPrimeImplicantTable() {
		vector<string> _minterms;
		for (auto p : this->boolTable.getMinterms())
			_minterms.push_back(p);
		vector<string> _implicants;
		for (auto p : this->implicants)
			_implicants.push_back(p);
		printPrimeImplicantTable(this->boolTable, _implicants, _minterms);
		//gotoxy(wherex(), wherey() + 1);
	}
	void Quine_McCluskey_algorithm::printMergingChart(map<pair<int, int>, set<string> > _groupedImpl) {
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

				gotoxy(__crd[tmpImpLvl].first.X + 9 - _s / 2, __crd[tmpImpLvl].second.Y + 2);
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

		gotoxy(__crd[__NUMBEROF1].first.X, mx.Y + 1);
		for (int k = __crd[__NUMBEROF1].first.X; k <= __crd[__IMPLEVEL + to_string(*(_levelsImp.rbegin()))].second.X; ++k)
			cout << '-';
		gotoxy(__crd[__IMPLEVEL + to_string(*(_levelsImp.rbegin()))].second.X, __crd[__IMPLEVEL + to_string(*(_levelsImp.rbegin()))].second.Y);
		for (int k = __crd[__IMPLEVEL + to_string(*(_levelsImp.rbegin()))].second.Y; k <= mx.Y; ++k) {
			cout << '|';
			gotoxy(wherex() - 1, wherey() + 1);
		}
	}
	void Quine_McCluskey_algorithm::printMergingChart() {
		printMergingChart(this->__groupedImpl);
	}
	string Quine_McCluskey_algorithm::GetMinimizedFunction() {
		return minimizedFunction;
	}