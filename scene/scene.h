#pragma once
#include <functional>
#include <vector>
#include <memory>

#include <SDL.h>

#include "../reactive/source.h"
#include "SceneState.h"

using std::vector;
using std::function;

template<class T, 
	typename = std::enable_if_t<std::is_base_of<SceneState, T>::value>>
class Scene
{
	vector<std::unique_ptr<Updater>> updates;

public:

	T st;
	
	Scene() {
		st.construct_updates(updates);
	}

	~Scene() {}

	void update(const vector<SDL_Event>& keyboard_events) {
		st.keyboard_events = keyboard_events;
		for (auto& update : updates) {
			update->calculate();
		}
		for (auto& update : updates) {
			update->update();
		}
		st.render_scene(st.main_camera.value);
	}
};

