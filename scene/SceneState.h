#pragma once

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

class SceneState
{
	SlotMap<Entity> entities;
	std::map<std::string, Polygon*> models; /**< Had to be raw pointers to prevent copy construction.*/
	World physics;

public:

	SceneState();
	~SceneState();

	Entity* spawn(Model model, Transform transform, PhysObj body);
	void render_scene(Camera camera);
};