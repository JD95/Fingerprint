#pragma once

#include <tuple>

#include <GL\glew.h>
#include "mat4x4.hpp"

//#include "Fingerprint/utilities/Type_Utitilies.h"
//#include "Fingerprint/utilities/Checks.h"

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
	Uniform() {}
	Uniform(GLuint program, char* name) 
		: location(glGetUniformLocation(program, name)) {};
	~Uniform() {};

	// Sets the values for all uniform variables in a tuple
	template <class Ts, class Vs>
	static void set_values(Ts& uniforms, Vs& values) {
		set_uniforms_impl<std::tuple_size<Ts>::value>()(uniforms, values);
	}
};

// Manages setting the value of an integer
struct UniformInt : public Uniform {
	UniformInt() {};
	UniformInt(GLuint program, char* name) : Uniform(program, name) {};
	void operator() (const GLint& value) const {
		glUniform1i(location, value);
	}
};

// Manages setting the value of an integer
struct UniformUint : public Uniform {
	UniformUint() {};
	UniformUint(GLuint program, char* name) : Uniform(program, name) {};
	void operator() (const GLuint& value) const {
		glUniform1ui(location, value);
	}

};

// Manages setting the value of a float
struct UniformFloat : public Uniform {
	UniformFloat() {};
	UniformFloat(GLuint program, char* name) : Uniform(program, name) {};
	void operator() (const GLfloat& value) const {
		glUniform1f(location, value);
	}
};

// Manages setting the value of a double
struct UniformDouble : public Uniform {
	UniformDouble() {};
	UniformDouble(GLuint program, char* name) : Uniform(program, name) {};
	void operator() (const GLdouble& value) const {
		glUniform1d(location, value);
	}
};

// Manages setting the value of a 4x4 Matrix
struct UniformVec2 : public Uniform {
	UniformVec2() {};
	UniformVec2(GLuint program, char* name) : Uniform(program, name) {};
	void operator() (const glm::vec2& value) const {
		auto test = &value[0];
		glUniform2fv(location, 1, test);
	}
};

// Manages setting the value of a 4x4 Matrix
struct UniformMat4 : public Uniform {
	UniformMat4() {};
	UniformMat4(GLuint program, char* name) : Uniform(program, name) {};
	void operator() (const glm::mat4& value) const {
		glUniformMatrix4fv(location, 1, false, &value[0][0]);
	}
};

// Manages setting the value of a 4x4 Matrix
struct UniformSampler2D : public Uniform {
	UniformSampler2D(){}
	UniformSampler2D(GLuint program, char* name)
		: Uniform(program, name)
	{
	};
	void operator() (const std::tuple<GLint, GLuint>& value) const {
		glUniform1i(location, std::get<0>(value));
		switch (std::get<0>(value)) {
		case 0: glActiveTexture(GL_TEXTURE0);
		}
		glBindTexture(GL_TEXTURE_2D, std::get<1>(value));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}
};