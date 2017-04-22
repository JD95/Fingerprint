#pragma once

#include <glm.hpp>
#include <vector>
#include "../reactive/reactive.h"

struct Animation { unsigned int start_cell[2]; unsigned int end_cell[2]; };

struct SpriteSheet
{
	unsigned int rows;
	unsigned int cols;
	float cell_width;
	float cell_height;
	
	float original_height;

	unsigned int current_col;
	unsigned int current_row;

	std::vector<Animation> animations;
	unsigned int previous_animation;

public:

	Reactive<unsigned int> current_animation;

	SpriteSheet(unsigned int rows, unsigned int cols, std::vector<Animation> as);
	~SpriteSheet();

	glm::vec2 step_animation();
};

