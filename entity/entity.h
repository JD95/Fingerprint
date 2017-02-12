#pragma once

#include <memory>

#include "transform.h"
#include "../primitive_shapes/polygon.h"
#include "../graphics/model.h"

struct Entity
{
	std::unique_ptr<Transform> transform;
	std::unique_ptr<Polygon> model;
	//std::unique_ptr<PhysObj> physics;
public:
	Entity();
	Entity(Transform t, Model m);
	~Entity();
};

