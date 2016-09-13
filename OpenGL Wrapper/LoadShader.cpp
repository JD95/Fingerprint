// ---------------------------------------------------------------------------
// LoadShader.cpp
// Quick and dirty LoadShader function for the OpenGL Programming Guide 4.3
// Red Book.
//
// Author: Qoheleth
// http://www.opengl.org/discussion_boards/showthread.php/180175-Redbook-8th-sample-code?p=1245842#post1245842
// ---------------------------------------------------------------------------
#include "LoadShader.h"

#include <vector>
#include <tuple>

std::vector<std::tuple<const char*, int, const char*>> ShaderInfo::getShaders()
{
	auto shaders = {
		std::make_tuple(vShaderFile, GL_VERTEX_SHADER, "\nVertex Shader compilation failed..."),
		std::make_tuple(fShaderFile, GL_FRAGMENT_SHADER, "\nFragment Shader compilation failed..."),
		std::make_tuple(tShaderFile, GL_TESS_CONTROL_SHADER,"\nTessellation Shader compilation failed..."),
		std::make_tuple(gShaderFile, GL_GEOMETRY_SHADER, "\nGeometry Shader compilation failed...")
	};

	return shaders;
}

void load_shader_text(const char* shader_file, GLuint &vertexShader)
{
	string shaderProgramText;

	const char* text = getShaderProgram(shader_file, shaderProgramText);
	GLint length = shaderProgramText.size();

	glShaderSource(vertexShader, 1, &text, NULL);
	glCompileShader(vertexShader);

	for (int i = 0; i < length; i++)
	{
		cout << text[i];
	}
}

void check_load_status(const GLuint &shader)
{
	check_load_status(shader, "Shader Compilation has Failed...");
}

void check_load_status(const GLuint &shader, const string fail_message)
{
	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

	if (!(status == GL_TRUE))
		cerr << fail_message << '\n';

	char infoLog[100];
	GLsizei bufSize = 100;
	glGetShaderInfoLog(shader, bufSize, NULL, infoLog);
	for (int i = 0; i < bufSize; i++)
		cout << infoLog[i];
}

void load_shader(const char* shader_file, GLuint shader, GLuint program)
{
	load_shader(shader_file, shader, program, "");
}

void load_shader(const char* shader_file, GLuint shader, GLuint program, const string fail_message)
{
	load_shader_text(shader_file, shader);
	check_load_status(shader, fail_message);
	glAttachShader(program, shader);
}

void check_link_status(const GLuint program)
{
	GLint link_status;
	glGetProgramiv(program, GL_LINK_STATUS, &link_status);
	if (!(link_status == GL_TRUE))
		cout << "Link failed..." << endl;
}

GLuint LoadShaders(ShaderInfo shaderInfo)
{
	GLuint program = glCreateProgram();

	for (auto shader : shaderInfo.getShaders())
	{
		if (std::get<0>(shader) != NULL) // File is given
		{
			GLuint shader_type = glCreateShader(std::get<1>(shader));
			load_shader(std::get<0>(shader), shader_type, program, std::get<2>(shader));
		}
	}

	glLinkProgram(program);

	check_link_status(program);

	return program;
}

const char* getShaderProgram(const char *filePath, string &shader)
{
	fstream shaderFile(filePath, ios::in);

	if (shaderFile.is_open())
	{
		std::stringstream buffer;
		buffer << shaderFile.rdbuf();
		shader = buffer.str();
		buffer.clear();
	}
	shaderFile.close();

	return shader.c_str();
}