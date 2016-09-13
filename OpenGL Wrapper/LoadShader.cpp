// ---------------------------------------------------------------------------
// LoadShader.cpp
// Quick and dirty LoadShader function for the OpenGL Programming Guide 4.3
// Red Book.
//
// Author: Qoheleth
// http://www.opengl.org/discussion_boards/showthread.php/180175-Redbook-8th-sample-code?p=1245842#post1245842
// ---------------------------------------------------------------------------
#include "LoadShader.h"

void load_shader(const char* shader_file, GLuint &vertexShader)
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

void check_load_status(GLuint &shader)
{
	check_load_status(shader, "Shader Compilation has Failed...");
}

void check_load_status(GLuint &shader, string fail_message)
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

void check_link_status(GLuint program)
{
	GLint link_status;
	glGetProgramiv(program, GL_LINK_STATUS, &link_status);
	if (!(link_status == GL_TRUE))
		cout << "Link failed..." << endl;
}

GLuint LoadShaders(ShaderInfo shaderInfo)
{
	GLuint program;
	GLuint vertexShader;
	GLuint fragmentShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);	 // create a vertex shader object
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER); // create a fragment shader object

	// load and compile vertex shader
	load_shader(shaderInfo.vShaderFile, vertexShader);
	check_load_status(vertexShader, "\nVertex Shader compilation failed...");
	
	// load and compile fragment shader
	load_shader(shaderInfo.fShaderFile, fragmentShader);
	check_load_status(fragmentShader, "\nFragment Shader compilation failed...");

	// create the shader program
	program = glCreateProgram();

	// attach the vertex and fragment shaders to the program
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);

	// link the objects for an executable program
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