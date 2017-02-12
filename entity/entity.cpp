#include "entity.h"



Entity::Entity()
{
}

Entity::Entity(Transform t, Model m) {
	transform = std::make_unique<Transform>(std::move(t));
	model = std::make_unique<Polygon>(m.model_name, m.tex_coords);
}


Entity::~Entity()
{
}
