#include "SpriteSheet.h"

SpriteSheet::SpriteSheet(unsigned int r, unsigned int c, std::vector<Animation> as)
	: animations(as)
	, rows(r)
	, cols(c)
	, current_col(0)
	, current_row(0)
	, cell_width(1.0f / (float)c)
	, cell_height(1.0f / (float)r)
	, original_height(1.0f - cell_height)
	, current_animation(0)
	, previous_animation(0)
{
}


SpriteSheet::~SpriteSheet()
{
}

glm::vec2 SpriteSheet::step_animation() {

	Animation& a = animations[current_animation.value];

	if (previous_animation != current_animation.value || current_row == a.end_cell[0] && current_col == a.end_cell[1]) {
		current_row = a.start_cell[0];
		current_col = a.start_cell[1];
		previous_animation = current_animation.value;
	}
	else {
		current_col++;
		if (current_col >= cols) {
			current_col = 0;
			current_row = (current_row + 1) % rows;
		}
	}

	return glm::vec2(current_col * cell_width, original_height - (current_row * cell_height));
}