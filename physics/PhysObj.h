#include "glm.hpp"
#include <iostream>
#include <algorithm>

#include "CollObj.h"

#pragma once
//Items directly realted to the phys obj
struct Material
{
	float density;
	float restitution = 0;
};

struct MassData
{
	float mass;
	float inv_mass;
};

//The actual physics object
class PhysObj
{
public:
	Collider shape;

	//Basic physics resources
	glm::vec2 force;
	glm::vec2 position;
	Material material;
	MassData mass;
	glm::vec2 acceleration;
	glm::vec2 velocity;

	//Used for time controlling elements
	float original_time;
	float time_step;

	//for fine tuning
	glm::vec2 gravity_scale;
	
	PhysObj();

	//Constructor
	PhysObj(float pos_x, float pos_y, float m, float width, float height, float step);
	PhysObj(float pos_x, float pos_y, float m, float radius, float step);

	//Modification Functions
	void add_force(float fx, float fy);
	void reset_object();
	void reset_force(float x, float y);

	void add_gravity(glm::vec2 gravity);

	//Does the heavy lifting
	void calculate_positon();
	
	//Utility
	void print_out_info();
};

//store information on each collision
struct Manifold {
	PhysObj *A;
	PhysObj *B;
	float penetration;
	glm::vec2 normal;
};

//Calculates the resulting velocities following an impact
void calculate_resolution(Manifold& m);

//Corrects for sinking due to hardware error
void sink_correction(Manifold& m);

//Checks collisions
bool Collide(Manifold& m);

//calculates and verifies the collisions of two AABB. Claculates normal and penetration
bool AABB_vs_AABB(Manifold& m);

bool Circle_vs_Circle(Manifold *m);

bool Circle_vs_AABB(Manifold *m);

bool AABB_vs_Circle(Manifold *m);