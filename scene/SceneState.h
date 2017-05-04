#pragma once

#include <SDL.h>
#include <map>
#include <memory>
#include <vec2.hpp>
#include <type_traits>

#include "../entity/entity.h"
#include "../primitive_shapes/polygon.h"
#include "../utilities/slotmap.h"
#include "../graphics/model.h"
#include "../entity/transform.h"
#include "../scene/camera.h"
#include "../physics/World.h"

#include "../reactive/source.h"

const float world_step = 0.0167f;

constexpr float model_body_ratio = 1.23f;

using Int64 = unsigned long long;

class SceneState
{
protected:
	World physics;
	SlotMap<Entity> entities;
	std::map<std::string, Polygon*> models; /**< Had to be raw pointers to prevent copy construction.*/

public:
	SlotMap<Entity> gui_entities;
	Reactive<Camera> main_camera;
	Reactive<std::vector<SDL_Event>> keyboard_events;
	Reactive<Int64> time;
	Reactive<int> current_level;

	SceneState();
	~SceneState();

	Entity* spawn(Model model, Transform transform);
	Entity* spawn(Model model, Transform transform, PhysObj body);
	Entity* spawn(AnimatedModel model, Transform transform);
	Entity* spawn(AnimatedModel model, Transform transform, PhysObj body);
	Entity* spawn_body(Model model, float layer, float x, float y, float width, float height, float mass);
	Entity* spawn_body(AnimatedModel model, float layer, float x, float y, float width, float height, float mass);
	Entity* spawn_massless(Model model, float layer, float x, float y, float width, float height);
	Entity * spawn_massless(AnimatedModel model, float layer, float x, float y, float width, float height);
	Entity* gui_spawn(Model model, float x, float y, float width, float height);
	void render_scene(Camera camera);
	virtual void construct_updates(vector<Updater>& updates) {}

	glm::vec2 get_grav_norm();
};