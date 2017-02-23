#pragma once

#include "../scene/scene.h"

/*!
	A Reactive value is anything whose value is 
	calculated by values from the previous frame.

	@code
	struct Unit {
		Reactive<glm::vec2> position;
	};

	struct demo {
		Unit objA;
		Unit objB;
	};

	update_t<glm::vec2&> five_away_from(const glm::vec2& other) {
		return [=](glm::vec2& attr) {
			attr = glm::vec2(other[0] + 5, other[1] + 5);
		};
	}

	void game_updates(vector<update>& updates, const demo& prev) {
		updates.push_back(prev.objA.position(five_away_from(prev.objB.position)));
	}
	@endcode

*/
template <class T>
struct Reactive
{
	T value; /**< The reactive value.*/

	Reactive()
	{
	}

	Reactive(const T& v) {
		value = v;
	}

	~Reactive()
	{
	}

	/*!
		Generates a callback to update value using the function f.
	*/
	update operator()(update_t<T&> f) const {
		return [f,this]() {
			// Cheating here so game state
			// in the construct update function 
			// can be const
			f(const_cast<T&>(value));
		};
	}

	operator T() const { return value; }
	operator T&() { return value; }
};

