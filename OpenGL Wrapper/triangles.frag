#version 430 core
in vec4 color;

uniform uint test;

out vec4 fColor;

void main() { 
	vec4 nColor = vec4(color[0], color[1] * test, color[2] * test, color[3]);
	fColor = nColor; 
} 