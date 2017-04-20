#version 430 core

uniform sampler2D tex;
uniform vec2 sprite_box;
uniform vec2 sprite_position;

in vec4 color;

out vec4 fColor;

void main() {

	float x = (color[0] * sprite_box[0]) + sprite_position[0];
	float y = (color[1] * sprite_box[1]) + sprite_position[1];
	fColor = texture(tex, vec2(x, y)); 
} 