#version 430 core

uniform sampler2D tex;

in vec4 color;

out vec4 fColor;

void main() { 
	fColor = texture(tex, vec2(color[0],color[1])); 
} 