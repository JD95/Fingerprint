#pragma once

// Allows for strong type aliases
template <class T, class UnitType> class strong_type
{
protected:
	T value;
public:
	strong_type(T val) : value(val) { }

	inline explicit operator T() const { return value; }
};

// Wraps an int with a certain type of distance to prevent misuing units
template <class UnitType> struct Distance 
	: strong_type<int, UnitType>
{
	Distance(int value) : strong_type(value) { }

	inline Distance<UnitType> operator+ (const Distance<UnitType>& other) const
	{
		return Distance<UnitType>(this->value + other.value);
	}

	inline Distance<UnitType> operator- (const Distance<UnitType>& other) const
	{
		return Distance<UnitType>(this->value - other.value);
	}

	inline Distance<UnitType> operator* (const Distance<UnitType>& other) const
	{
		return Distance<UnitType>(this->value * other.value);
	}

	inline Distance<UnitType> operator/ (const Distance<UnitType>& other) const
	{
		return Distance<UnitType>(this->value / other.value);
	}

	inline bool operator== (const Distance<UnitType>& other) const
	{
		return this->value == other.value;
	}

	inline bool operator< (const Distance<UnitType>& other) const
	{
		return this->value < other.value;
	}

	inline bool operator> (const Distance<UnitType>& other) const
	{
		return this->value > other.value;
	}

	inline bool operator<= (const Distance<UnitType>& other) const
	{
		return this->value <= other.value;
	}

	inline bool operator>= (const Distance<UnitType>& other) const
	{
		return this->value >= other.value;
	}

	inline bool operator!= (const Distance<UnitType>& other) const
	{
		return this->value != other.value;
	}
};

struct Meters {};