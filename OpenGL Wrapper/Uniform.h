#pragma once

#include <GL\glew.h>
#include <tuple>
#include "Type_Utitilies.h"
#include "Checks.h"
#include "glm\mat4x4.hpp"

/*
	The classes defined here are helpers for managing variables which
	are shared between shaders.

	More details on them can be founder here: https://www.opengl.org/sdk/docs/man/html/glUniform.xhtml
*/

// Base class of a Uniform Variable, stores the location
class Uniform
{
	template <int n> struct set_uniforms_impl {
		template<class Ts, class Vs>
		void operator() (Ts& uniforms, Vs& values) {
			std::get<n-1>(uniforms)(std::get<n-1>(values));
			set_uniforms_impl<n - 1>()(uniforms, values);
		}
	};

	template <> struct set_uniforms_impl <0> {
		template<class Ts, class Vs>
		void operator() (Ts& uniforms, Vs& values) { }
	};

protected:
	// Specifies the location of the uniform variable to be modified.
	GLint location;

public:
	Uniform(GLint l) : location(l) {};
	~Uniform() {};

	// Sets the values for all uniform variables in a tuple
	template <class Ts, class Vs>
	static void set_values(Ts& uniforms, Vs& values) {
		set_uniforms_impl<std::tuple_size<Ts>::value>()(uniforms, values);
	}
};

// Manages setting the value of an integer
struct UniformInt : public Uniform {
	UniformInt(GLint location) : Uniform(location) {};
	void operator() (const GLint& value) const {
		glUniform1i(location, value);
	}
};

// Manages setting the value of a float
struct UniformFloat : public Uniform {
	UniformFloat(GLint location) : Uniform(location) {};
	void operator() (const GLfloat& value) const {
		glUniform1f(location, value);
	}
};

// Manages setting the value of a double
struct UniformDouble : public Uniform {
	UniformDouble(GLint location) : Uniform(location) {};
	void operator() (const GLdouble& value) const {
		glUniform1d(location, value);
	}
};

// Manages setting the value of a 4x4 Matrix
struct UniformMat4 : public Uniform {
	UniformMat4(GLint location) : Uniform(location) {};
	void operator() (const glm::mat4& value) const {
		glUniformMatrix4fv(location, 1, false, &value[0][0]);
	}
};