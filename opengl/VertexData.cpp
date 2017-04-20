#include "vertexdata.h"

VertexData2D::VertexData2D(RGBA cs, Coord2D vs)
	: color(cs), coord(vs)
{
}

VertexData2D::~VertexData2D()
{
}

RGBA::RGBA(GLfloat r, GLfloat g, GLubyte b, GLubyte a)
	: r(r), g(g), b(b), a(a)
{
}

RGBA::~RGBA()
{
}

Coord2D::Coord2D(GLfloat x, GLfloat y)
	: x(x), y(y)
{
}

Coord2D::~Coord2D()
{
}

TexturedVertexData2D::TexturedVertexData2D(Coord2D t, Coord2D v)
	: text_coord(t), vert_coord(v)
{
}

TexturedVertexData2D::~TexturedVertexData2D()
{
}
