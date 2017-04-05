#pragma once
#include "glm.hpp"
#include <vector>
#include "CollObj.h"
#include "PhysObj.h"

class Quadtree
{
private: 
	int MAX_OBJECTS = 10;
	int MAX_LEVELS = 5;

	int level;
	std::vector <PhysObj*>  objects;
	AABB bounds;
	std::vector <Quadtree> nodes;	

public:
	Quadtree();
	Quadtree(int pLevel, AABB pBounds);
	~Quadtree();

	void clear();
	void split();
	int getIndex(PhysObj * the_object);
	void insert(PhysObj * the_object);
	std::vector<PhysObj*> retrieve(PhysObj * the_object);

};

