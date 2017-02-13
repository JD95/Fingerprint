#pragma once
#include <functional>
#include <vector>

using std::vector;
using std::function;

template <class T>
using update_t = function<void(T)>;

using update = function<void()>;

template <class State>
class Scene
{
	function<void(vector<update_t<State&>>&, const State&)> construct_updates;
	vector<update_t<State&>> updates;

public:

	State st;
	
	Scene(function<void(vector<update_t<State&>>&, const State&)> c_u)
		: construct_updates(c_u) 
	{ 
		int x = 5;
	}

	~Scene() {}

	void update() {
		construct_updates(vector<update_t<State&>>& updates);
		for (auto update : updates) update(st);
	}
};

