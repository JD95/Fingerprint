#pragma once

#include "..\gamestate\gamestate.h"

template <class T>
struct Reactive
{
	T value;

	Reactive();
	~Reactive();

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

