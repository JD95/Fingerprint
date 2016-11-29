#pragma once

constexpr bool within_range(int x, int min, int max)
{
	return min <= x && x < max;
}