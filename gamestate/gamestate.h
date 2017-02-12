#pragma once
#include <functional>
#include <vector>

using std::vector;
using std::function;

template <class T>
using update_t = function<void(T)>;

using update = function<void()>;

template <class State>
class game_state
{
	function<void(vector<update_t<State&>>&, const State&)> construct_updates;
	vector<update_t<State&>> updates;

public:

	State st;
	
	game_state(State s, function<void(vector<update_t<State&>>&, const State&)> c_u)
		: st(s)
		, construct_updates(c_u) { }

	~game_state() {}

	void update() {
		construct_updates(vector<update_t<State&>>& updates);
		for (auto update : updates) update(st);
	}
};

