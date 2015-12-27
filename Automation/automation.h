#pragma once
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <bitset>
#include <set>
#include <iostream>
using namespace std;

class symbol {
private:
	string rep;
public:
	symbol(const string& str) {
		rep = str;
	}
	symbol(const char *str) {
		rep = str;
	}
	~symbol() {}
	inline bool isempty() { return rep == ""; }
	friend inline bool operator <(const symbol&, const symbol&);
	friend ostream& operator << (ostream&, const symbol&);
}empty("");

inline bool operator <(const symbol& a, const symbol& b) {
	return a.rep < b.rep;
}

ostream& operator << (ostream& out, const symbol& b) {
	out << b.rep;
	return out;
}

class String {
private:
	vector <symbol> str;
public:
	inline vector <symbol>::size_type size() const{
		return str.size();
	}
	
	inline bool empty() {
		return str.empty();
	}

	String() {}
	
	String(const symbol& s) {
		str.push_back(s);
	}
	
	~String() {}

	friend inline bool operator <(const String&, const String&);
	friend String operator +(String, const String&);
	friend String operator +(String, const symbol&);
	friend String operator +(const symbol&, const String&);
	String& operator += (const String& b) {
		for (auto s = b.str.begin(); s != b.str.end(); s++)
			str.push_back(*s);
		return *this;
	}

	String& operator += (const symbol& b) {
		str.push_back(b);
		return *this;
	}

	friend ostream& operator << (ostream&, const String&);
};

inline bool operator <(const String& a, const String& b) {
	return a.str < b.str;
}

String operator +(String a, const String& b) {
	for (auto s = b.str.begin(); s != b.str.end(); s++)
		a.str.push_back(*s);
	return a;
}

String operator +(String a, const symbol& b) {
	a.str.push_back(b);
	return a;
}

String operator +(const symbol& a, const String& b) {
	return String(a) + b;
}

ostream& operator << (ostream& out, const String& b) {
	for (auto s = b.str.begin(); s != b.str.end(); s++)
		out << *s << " ";
	return out;
}

class automation {
private:
	int n;
	vector <string> state;
	vector <map <symbol, set<int> > > E;
	int init;
	set <int> accepted;
	String bracket(const pair <String, bool>& x, char flag) {
		if (flag == '+') {
			if (x.second) return "(" + x.first + ")";
			return x.first;
		}
		else {
			if ((int)x.first.size() > 1) return "(" + x.first + ")";
			return x.first;
		}
	}
public:
	automation(int N) : n(N) {
		E.resize(N);
		state.resize(N);
	}
	~automation() {}

	void name(int idx, const string& s) {
		state[idx] = s;
	}
	
	void setinit(int idx) {
		init = idx;
	}

	void addac(int idx) {
		accepted.insert(idx);
	}

	void addedge(int v1, int v2, const symbol& s) {
		E[v1][s].insert(v2);
	}

	bool isaccepted(const String&);

	inline String RE() {
		String ret;
		vector<map <int, pair<String, bool> > > edge(n);
		for (auto FNL = accepted.begin(); FNL != accepted.end(); FNL++) {
			for (int i = 0; i < n; i++) edge[i].clear();
			int fnl = *FNL;
			for (int i = 0; i < (int)E.size(); i++)
				for (auto s = E[i].begin(); s != E[i].end(); s++)
					for (auto v = s->second.begin(); v != s->second.end(); v++) {
						int j = *v;
						if (edge[i].find(j) == edge[i].end()) edge[i][j] = make_pair(s->first, false); else {
							edge[i][j].first += "+", edge[i][j].second = true;
							edge[i][j].first += s->first;
						}
					}
			
			for (int i = 0; i < n; i++) {
				if (i == init || i == fnl) continue;
				String mid;
				if (edge[i].find(i) != edge[i].end()) mid = bracket(edge[i][i], '*') + "^{*}";
				for (int j = 0; j < n; j++) {
					if (j == i) continue;
					if (edge[j].find(i) != edge[j].end()) {
						for (auto e = edge[i].begin(); e != edge[i].end(); e++) 
							if (e->first != i) {
								if (edge[j].find(e->first) == edge[j].end()) {
									edge[j][e->first] = make_pair(bracket(edge[j][i], '+') + mid + bracket(e->second, '+'), false);
								}
								else {
									edge[j][e->first].first += "+", edge[j][e->first].second = true;
									edge[j][e->first].first += bracket(edge[j][i], '+') + mid + bracket(e->second, '+');
								}
							}
					}
				}
				edge[i].clear();
			}
			
			if (!ret.empty()) ret += "+";
			if (init == fnl) {
				ret += bracket(edge[init][fnl], '*') + "^{*}";
			} else {
				if (edge[fnl].find(init) != edge[fnl].end()) {
					bool ii = edge[init].find(init) != edge[init].end(),
						ff = edge[fnl].find(fnl) != edge[fnl].end(),
						fi = edge[fnl].find(init) != edge[fnl].end();
					String cir;
					if (ii) cir += bracket(edge[init][init], '*');
					cir += bracket(edge[init][fnl], '+');
					if (ff) cir += bracket(edge[fnl][fnl], '*');
					cir += bracket(edge[fnl][init], '+');
					ret = "(" + cir + ")" + "^{*}";
					if (ii) ret += bracket(edge[init][init], '*');
					ret += bracket(edge[init][fnl], '+');
					if (ff) ret += bracket(edge[fnl][fnl], '*');
				}
			}
		}
		return ret;
	}

	void dot(const string& out) {
		ofstream fout(out);
		fout << "digraph G{" << endl;
		fout << "	rankdir = LR;" << endl;
		fout << "	node [shape = none];" << endl;
		fout << "	source [label = \"\"];" << endl;
		fout << "	node [shape = doublecircle];" << endl;
		fout << "	";
		for (auto x = accepted.begin(); x != accepted.end(); x++) {
			if (x != accepted.begin()) fout << ", ";
			fout << state[*x];
		}
		fout << ";" << endl;
		fout << "	node [shape = circle];" << endl;
		fout << "	source->" + state[init] + " [label = \"start\"];" << endl;
		for (int i = 0; i < (int)E.size(); i++)
			for (auto s = E[i].begin(); s != E[i].end(); s++) 
				for (auto v = s->second.begin(); v != s->second.end(); v++) {
					int j = *v;
					fout << "	" << state[i] << "->" << state[j] << " [label=\"" << (s->first) << "\"];" << endl;
				}
		fout << "}" << endl;
		fout.close();
	}

	string& operator [](int idx) {
		return state[idx];
	}
};