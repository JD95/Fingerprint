#pragma once
#include <functional>
#include <vector>

#include <SDL.h>

#include "../reactive/source.h"

using std::vector;
using std::function;

template <class T>
using update_t = function<void(T)>;

using update = function<void()>;

template <class State>
class Scene
{
	function<void(
		vector<update_t<State&>>&,		// Updates to run
		const vector<SDL_Event>&,		// Keyboard Events
		const State&					// The previous frame's state
		)> construct_updates;

	vector<update_t<State&>> updates;

public:

	State st;
	
	Scene() {}

	~Scene() {}

	void update(const vector<SDL_Event>& keyboard_events) {
		st.construct_updates(updates, keyboard_events, st);
		for (auto update : updates) update(st);
		st.render_scene(st.main_camera);
		updates.clear();
	}
};

