#pragma once

#include "GL\glew.h"

struct RGBA {
	GLubyte r;
	GLubyte g;
	GLubyte b;
	GLubyte a;
	RGBA(GLubyte r, GLubyte g, GLubyte b, GLubyte a);
	~RGBA();
};

struct Coord2D {
	GLfloat x;
	GLfloat y;
	Coord2D(GLfloat x, GLfloat y);
	~Coord2D();
};

/*
	All of the vertex data is stored in a single
	structure. The vertex attributes will later be
	told how to extract the individual components.

	For example:
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData2D), buffer_offset(sizeof(RGBA)));

	This tells OpenGL to send everything after the RGBA value as the data
*/
struct VertexData2D
{
	RGBA color;
	Coord2D coord;
	VertexData2D(RGBA cs, Coord2D vs);
	~VertexData2D();
};

struct TexturedVertexData2D
{
	Coord2D text_coord;
	Coord2D vert_coord;
	TexturedVertexData2D(Coord2D t, Coord2D v);
	~TexturedVertexData2D();
};
