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
		quad.insert(&objects[i]);
	}
	
	//create manifold for collision use, same manifold used repeatedly
	Manifold m;
	
	//begin verlet integrations, most likely will be a for loop for all objects
	for (auto& object : objects)
	{
		object.add_gravity(gravity_acc);
		object.normals_acting.clear();
	}

	std::vector<PhysObj*> check_objects;
	for (size_t concern = 1; concern < objects.size(); concern++) {

		
		//clear the check objects list
		check_objects.clear();
		//with regards to object of concern, check which quadtree division hosts it
		check_objects = quad.retrieve(&objects[concern]);

		//check collisions of all objects that are proximate to the target object
		//for (int i = 0; i < check_objects.size(); i++)
		for (size_t i = 0; i < check_objects.size(); i++)
		{
			//Checks to see if the pointer matches the address of the original object so as to avoid checking colliding with self
			if (check_objects[i] == &objects[concern])
				continue;
			{
				if (objects[i].mass.mass == 2.0)
					int what = 1;

				m.A = &objects[concern];
				m.B = check_objects[i];;//check_objects[i];

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
	}

	for (auto& object : objects)
	{
		object.calculate_positon();
		//object.print_out_info();
	}
}

PhysObj* World::add_object(PhysObj object_in)
{
	objects.push_back(object_in);
	return &(*(objects.begin() + objects.size()-1));
}

