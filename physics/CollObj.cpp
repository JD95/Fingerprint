#include "CollObj.h"

//Collider functions
Collider::Collider()
{
}

Collider::Collider(float x, float y, float width, float height)
{
	shape.Rect = AABB(x, y, width, height);
	type = 1;
}

Collider::Collider(float x, float y, float rad)
{
	shape.Circle = Circle(x, y, rad);
	type = 0;
}

void Collider::position_correction(glm::vec2& position)
{
	if (type)
		shape.Rect.position_correction(position);
	else
		shape.Circle.position_correction(position);

}

Coll_Shape Collider::get_coll()
{
	return shape;
}
