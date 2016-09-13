#pragma once
// ---------------------------------------------------------------------------
// LoadShader.h
// Quick and dirty LoadShader function for the OpenGL Programming Guide 4.3
// Red Book.
//
// Author: Qoheleth
// [url]http://www.opengl.org/discussion_boards/showthread.php/180175-Redbook-8th-sample-code?p=1245842#post1245842[/url]
// ---------------------------------------------------------------------------
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

#include <GL\glew.h>

struct ShaderInfo {
	GLenum vTarget;
	const char *vShaderFile;
	GLenum fTarget;
	const char *fShaderFile;

	ShaderInfo(const char *v, const char *f)
	{
		vShaderFile = v;
		fShaderFile = f;
	}
};

void load_shader(const char * shader_file, GLuint & vertexShader);

void check_load_status(GLuint & shader);

void check_load_status(GLuint & shader, string fail_message);

void check_link_status(GLuint program);

GLuint LoadShaders(ShaderInfo shaderInfo);

const char* getShaderProgram(const char *filePath, string &shaderProgramText);