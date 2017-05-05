#include "Collisions.h"



void Collisions::keep(object_id id)
{
	auto removed  = std::find_if(colors[green].begin(), colors[green].end(),
		[id](auto a) { 
		return a.id.index == id.index; 
	});

	if (removed == colors[green].end()) return;
	
	colors[red].insert(*removed);
	colors[green].erase(removed);
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

	auto already_colliding = colors[green].end() != colors[green].find({ id, velocity });

	if (already_colliding) keep(id);
	else enter.insert(CollisionInfo{ id, velocity });
}

std::set<CollisionInfo> Collisions::contact()
{
	return colors[green];
}

std::set<CollisionInfo> Collisions::exit()
{
	return colors[blue];
}

void Collisions::cycle_phase()
{
	red = (red + 1) % 3;
	blue = (blue + 1) % 3;
	green = (green + 1) % 3;

	colors[green].insert(enter.begin(), enter.end());
	enter.clear();
	colors[red].clear();
}
