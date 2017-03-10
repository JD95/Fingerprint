#include "World.h"
#include "windows.h"

World::World()
{

}

World::World(int world_h, int world_w, float grav_x, float grav_y)
{
	objects.reserve(100);
	quad = Quadtree(0, AABB(0.0f, 0.0f, (float)world_w, (float)world_h));
	gravity_acc = glm::vec2(grav_x, grav_y);
}

void World::step()
{
	//objects[0].add_force(1.0, 2.0);
	quad.clear();
	for (size_t i = 0; i < objects.size(); i++)
	{
		quad.insert(objects[i]);
	}
	
	//create manifold for collision use, same manifold used repeatedly
	Manifold m;

	std::vector<PhysObj> check_objects;
	int concern = 0;
		
	//clear the check objects list
	check_objects.clear();
	//with regards to object of concern, check which quadtree division hosts it
	check_objects = quad.retrieve(objects[concern]);
		
	//begin verlet integrations, most likely will be a for loop for all objects
	for(auto& object : objects)
		object.add_gravity(gravity_acc);

	//check collisions of all objects that are proximate to the target object
	for (size_t i = 0; i < check_objects.size(); i++)
	{
		if (i == concern)
			continue;
		else
		{
			m.A = &objects[i];
			m.B = &objects[concern];

			//Check actual colliosns
			//TODO: 
			//	make it so that there is an initial simple check then a more in depth check
			if (Collide(m))
			{

				calculate_resolution(m);
				sink_correction(m);
			}
		}
	}

	for (auto& object : objects)
		object.calculate_positon();
}

PhysObj* World::add_object(PhysObj object_in)
{
	objects.push_back(object_in);
	return &(*(objects.begin() + objects.size()-1));
}
