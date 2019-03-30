#pragma once
#include <vector>
#include <map>
#include <queue>
#include <utility>
#include "Queen.h"
typedef std::pair<int, int> pii;
class State
{
public:
	State();
	~State();
	std::vector<Queen> queens;
	int hn = 0, step = 0, fn = 0;
	std::vector<int> row, col, rd;
	std::map<int, int> ld;
	std::queue<pii> solution;
	friend bool operator > (const State& l, const State& r) {
		return l.fn > r.fn;
	};
};

