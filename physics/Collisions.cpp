#include "Collisions.h"



void Collisions::keep(object_id id)
{
	auto removed  = std::partition(colors[green].begin(), colors[green].end(),
		[id](auto a) { 
		return a.id.index != id.index; 
	});
	
	colors[red].insert(colors[red].end(), removed, colors[green].end());
	colors[green].erase(removed, colors[green].end());
}

Collisions::Collisions()
	: red(0)
	, blue(1)
	, green(2)
{}

Collisions::~Collisions()
{
}

void Collisions::add_collision(object_id id, glm::vec2 velocity) {
	auto begin = colors[green].begin();
	auto end = colors[green].end();
	auto already_colliding = colors[green].end() != std::find_if(begin, end,
		[id](auto a) { return a.id.index == id.index; });

	if (already_colliding) keep(id);
	else enter.push_back(CollisionInfo{ id, velocity });
}

std::vector<CollisionInfo> Collisions::contact()
{
	return colors[green];
}

std::vector<CollisionInfo> Collisions::exit()
{
	return colors[blue];
}

void Collisions::cycle_phase()
{
	red = (red + 1) % 3;
	blue = (blue + 1) % 3;
	green = (green + 1) % 3;

	colors[green].insert(colors[green].end(), enter.begin(), enter.end());
	enter.clear();
	colors[red].clear();
}
