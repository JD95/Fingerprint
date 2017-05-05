#pragma once

#include <glm.hpp>
#include <set>
#include <algorithm>
#include <string>

#include "../utilities/SlotMap.h"
#include <iostream>

struct CollisionInfo {
	object_id id;
	glm::vec2 velocity;
	friend bool operator<(const CollisionInfo& a, const CollisionInfo& b) {
		return a.id < b.id;
	}
};

class Collisions
{
	int red;
	int blue;
	int green;
	std::set<CollisionInfo> colors[3];

	/*
		There are three vectors used: green, blue, and red.

		Green are the objects which stay in contanct.
		Blue are the objects which are exiting contact.
		Red are the objects about to be destroyed.

		When the game runs through the update functions,
		red will be clear.

		Each time there is a continued collisions with an object,
		its collision info is moved from green to red.

		At the end of the udpates, the colors for each array are
		then cycled.

		Red -> Green
		Green -> Blue
		Blue -> Red

		In this way, we can keep track of continued collisions and exits.
	*/

	void keep(object_id id);

public:

	std::set<CollisionInfo> enter;

	Collisions();
	~Collisions();

	void add_collision(object_id id, glm::vec2 velocity);
	std::set<CollisionInfo> contact();
	std::set<CollisionInfo> exit();
	void cycle_phase();

	friend std::ostream& operator<<(std::ostream& out, const Collisions& c) {
		out << "enter: ";
		for (auto& e : c.enter)
			out << e.id.index << " ";
		out << "\n";
		
		out << "green: ";
		for (auto& g : c.colors[c.green])
			out << g.id.index << " ";
		out << "\n";

		out << "blue: ";
		for (auto& b : c.colors[c.blue])
			out << b.id.index << " ";
		out << "\n";

		out << "red: ";
		for (auto& r : c.colors[c.red])
			out << r.id.index << " ";
		out << "\n";

		return out;

	}
};

