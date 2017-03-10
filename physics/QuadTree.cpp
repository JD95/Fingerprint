#include "QuadTree.h"

Quadtree::Quadtree()
{

}

Quadtree::Quadtree(int pLevel, AABB pBounds)
{
	level = pLevel;
	bounds = pBounds;
}

Quadtree::~Quadtree()
{
}

void Quadtree::clear()
{
	objects.clear();

	for (size_t i = 0; i < nodes.size(); i++)
	{
		if (!nodes.empty())
		{
			nodes[i].clear();
		}
	}
}

void Quadtree::split()
{
	int subHeight = int((bounds.max.y - bounds.min.y) / 2);
	int subWidth = int((bounds.max.x - bounds.min.x) / 2);

	float xL = float(bounds.min.x);
	float yL = float(bounds.min.y);
	float xU = float(bounds.min.x);
	float yU = float(bounds.min.y);

	nodes[0] = Quadtree(level + 1, AABB(xL, yL + subHeight, xU - subWidth, yU));	//NW	-	A
	nodes[1] = Quadtree(level + 1, AABB(xL + subHeight, yL + subHeight, xU, yU));	//NE	-	B
	nodes[2] = Quadtree(level + 1, AABB(xL, yL, xL + subWidth, xU + subHeight));	//SW	-	C
	nodes[3] = Quadtree(level + 1, AABB(xL, yL + subWidth, xU, xU - subHeight));	//SE	-	D


}

int Quadtree::getIndex(PhysObj the_object)
{
	int index = -1;
	if (the_object.shape.type)		//is a circle
	{
		
		double verticalMidpoint = bounds.min.x + (bounds.width / 2);
		double horizontalMidpoint = bounds.min.y + (bounds.height / 2);

		// Object can completely fit within the top quadrants
		bool topQuadrant = (the_object.position.x < horizontalMidpoint && the_object.position.y + the_object.shape.get_coll().Rect.height < horizontalMidpoint);
		// Object can completely fit within the bottom quadrants
		bool bottomQuadrant = (the_object.shape.get_coll().Rect.min.y > horizontalMidpoint);

		// Object can completely fit within the left quadrants
		if (the_object.shape.get_coll().Rect.min.x < verticalMidpoint && the_object.shape.get_coll().Rect.min.x + the_object.shape.get_coll().Rect.height < verticalMidpoint) {
			if (topQuadrant) {
				index = 1;
			}
			else if (bottomQuadrant) {
				index = 2;
			}
		}
		// Object can completely fit within the right quadrants
		else if (the_object.shape.get_coll().Rect.min.x > verticalMidpoint) {
			if (topQuadrant) {
				index = 0;
			}
			else if (bottomQuadrant) {
				index = 3;
			}
		}
	}
	else				//is a Circle
	{
		double verticalMidpoint = bounds.min.x + (bounds.width / 2);
		double horizontalMidpoint = bounds.min.y + (bounds.height / 2);

		// Object can completely fit within the top quadrants
		bool topQuadrant = (the_object.position.x < horizontalMidpoint && the_object.position.y + the_object.shape.get_coll().Circle.radius < horizontalMidpoint);
		// Object can completely fit within the bottom quadrants
		bool bottomQuadrant = (the_object.shape.get_coll().Circle.position.y + the_object.shape.get_coll().Circle.radius > horizontalMidpoint);

		// Object can completely fit within the left quadrants
		if (the_object.shape.get_coll().Circle.position.x + the_object.shape.get_coll().Circle.radius < verticalMidpoint && the_object.shape.get_coll().Circle.position.x + the_object.shape.get_coll().Circle.radius < verticalMidpoint) {
			if (topQuadrant) {
				index = 1;
			}
			else if (bottomQuadrant) {
				index = 2;
			}
		}
		// Object can completely fit within the right quadrants
		else if (the_object.shape.get_coll().Circle.position.x + the_object.shape.get_coll().Circle.radius > verticalMidpoint) {
			if (topQuadrant) {
				index = 0;
			}
			else if (bottomQuadrant) {
				index = 3;
			}
		}
	}
	
	return index;
}

void Quadtree::insert(PhysObj the_object)
{
	//check to see if the current quad node has already been split
	if (!nodes.empty())
	{
		int index = getIndex(the_object);

		if (index != -1)
		{
			nodes[index].insert(the_object);

			return;
		}
	}

	objects.push_back(the_object);

	if ((int)objects.size() > MAX_OBJECTS && level < MAX_LEVELS)
	{
		if (nodes.empty())
			split();

		std::vector<PhysObj>::iterator it = objects.begin();
		while (it != objects.end())
		{
			int index = getIndex(*it);

			if (index != -1)
			{
				nodes[index].insert(*it);
				it = objects.erase(it);
			}
			else
			{
				++it;
			}
		}
	}

}

std::vector<PhysObj> Quadtree::retrieve(PhysObj the_object)
{
	int index = getIndex(the_object);

	if (index != -1 && !nodes.empty())
	{
		nodes[index].retrieve(the_object);
	}

	std::vector<PhysObj> objectsFound = objects;
	
	return objectsFound;
}
