#pragma once
#include "glm.hpp"
#include "PhysObj.h"
#include "QuadTree.h"

#include <vector>

//Will control gloabal variable like gravity
//list of objects in the world
//instead of world could be read as *Level*.. may change if helps clarity

class World
{
public:

	glm::vec2 gravity_acc;
	std::vector <PhysObj> objects;
	Quadtree quad;

	World();
	World(int world_h, int world_w, float grav_x, float grav_y);
	void step();
	PhysObj* add_object(PhysObj object_in);
};


