#pragma once

#include <GL\glew.h>

class Uniform
{
protected:
	GLint location;
public:
	Uniform(GLint l) : location(l) {};
	~Uniform() {};
};

class UniformInt : public Uniform {
	UniformInt(GLint location) : Uniform(location) {};
	UniformInt& operator= (const GLint& value) {
		glUniform1i(location, value);
	}
};

class UniformFloat : public Uniform {
	UniformFloat(GLint location) : Uniform(location) {};
	UniformFloat& operator= (const GLint& value) {
		glUniform1f(location, value);
	}
};

class UniformDouble : public Uniform {
	UniformDouble(GLint location) : Uniform(location) {};
	UniformDouble& operator= (const GLint& value) {
		glUniform1d(location, value);
	}
};