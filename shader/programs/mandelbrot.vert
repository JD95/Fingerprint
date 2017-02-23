#version 430 core

layout(location = 0) in vec4 vPosition;
layout(location = 1) in vec4 vColor;

out vec4 color;
out vec4 pos;

void main() { 
	color = vColor;
	pos = vPosition;
	gl_Position = vPosition; 
} 