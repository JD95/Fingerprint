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
#include <tuple>
using namespace std;

#include <GL\glew.h>

struct ShaderInfo {

	const char *vShaderFile;
	const char *fShaderFile;
	const char *tShaderFile;
	const char *gShaderFile;

	ShaderInfo( const char *v	 // Vertex
			  , const char *f	 // Fragment
			  , const char *t    // Tesselation
		      , const char *g )  // Geometry
	{
		vShaderFile = v;
		fShaderFile = f;
		tShaderFile = t;
		gShaderFile = g;
	}

	std::vector<std::tuple<const char*, int, const char*>> getShaders();
};

void load_shader_text(const char * shader_file, GLuint & vertexShader);

void check_load_status(const GLuint & shader);

void check_load_status(const GLuint & shader, const string fail_message);

void load_shader(const char * shader_file, GLuint shader, GLuint program);

void load_shader(const char * shader_file, GLuint shader, GLuint program, const string fail_message);

void check_link_status(const GLuint program);

GLuint LoadShaders(ShaderInfo shaderInfo);

const char* getShaderProgram(const char *filePath, string &shaderProgramText);