#pragma once

#include "Position2D.h"

namespace Physics
{
	// Forward declaration of Physics Body Type
	template <int Dimension, int GridSize> class PhysicsBody {};

	// Definition of Physics Body for 2D
	template <int GridSize> class PhysicsBody<2, GridSize>
	{
		char id;
		Position2D<GridSize> coords;
		int collapsed_x;

	public:

		PhysicsBody(char id_, int x_, int y_)
			: id(id_), coords(x_, y_)
		{
			collapsed_x = coords.to_1D();
		}

		~PhysicsBody() { }

		int get_collapsed_x() const { return collapsed_x; }

		char get_id() const { return id; }

		bool PhysicsBody::operator<(const PhysicsBody & other) const
		{
			return collapsed_x < other.collapsed_x;
		}
	};

	template <int GridSize>
	using Body2D = PhysicsBody<2, GridSize>;
}


