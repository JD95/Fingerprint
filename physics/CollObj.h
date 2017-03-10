#pragma once

#include <iostream>
#include "glm.hpp"



struct AABB
{
	glm::vec2 min;
	glm::vec2 max;
	float height;
	float width;

	AABB()
	{

	}

	AABB(float minX, float minY, float hei, float wid)
	{
		min = glm::vec2(minX, minY);
		height = hei;
		width = wid;
		max = glm::vec2(minX + width, minY + height);
		
	}

	void position_correction(glm::vec2 & position)
	{
		min = position;
		max = glm::vec2((position.x + width), (position.y + height));
	}

	//void get_rect()
	//{
	//	std::cout << "GET RICKITTY WRECKTED SON!!!11!!!1 \N";
	//}
};

struct Circle
{
	float radius;
	glm::vec2 position; //can also be treated as the center

	Circle(float x, float y, float rad)
	{
		position.x = x;
		position.y = y;
		radius = rad;

	}

	void position_correction(glm::vec2 & pos)
	{
		position = pos;
	}
};

union Coll_Shape {
	AABB Rect;
	Circle Circle;

	Coll_Shape() { memset(this, 0, sizeof(Coll_Shape)); }
};

//Collider and collision equations
class Collider
{
private:
	Coll_Shape shape;

public:
	int type; //------------ 1 for rect / 0 for circle ------------------

	Collider();

	Collider(float x, float y, float height, float width);
	Collider(float x, float y, float rad);

	void position_correction(glm::vec2& position);
	Coll_Shape get_coll();
};
