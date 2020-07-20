#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Shader.h"
#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader(const std::string& vertexFilePath, const std::string& fragmentFilePath): m_RendererID(0), m_UniformLocs()
{
	//Vertex Shader Creation
	std::stringstream source = ParseShader(vertexFilePath);
	unsigned int vShader = CreateShader(GL_VERTEX_SHADER, source.str());

	//Fragment Shader Creation
	source = ParseShader(fragmentFilePath);
	unsigned int fShader = CreateShader(GL_FRAGMENT_SHADER, source.str());

	//Program creation to attach shader objects
	m_RendererID = glCreateProgram();
	GLCall(glAttachShader(m_RendererID, vShader));
	GLCall(glAttachShader(m_RendererID, fShader));

	GLCall(glLinkProgram(m_RendererID));

	int link;
	GLCall(glGetProgramiv(m_RendererID, GL_LINK_STATUS, &link));

	if (link != GL_TRUE)
	{
		char message[1024];
		int log_length;
		GLCall(glGetProgramInfoLog(fShader, 1024, &log_length, message));

		std::cout << "Program Linking Error: " << message << std::endl;
	}

	//Once the linking of program is successful, we can delete the shaders
	GLCall(glDeleteShader(vShader));
	GLCall(glDeleteShader(fShader));
}

Shader::~Shader()
{
	GLCall(glDeleteProgram(m_RendererID));
}

std::stringstream Shader::ParseShader(const std::string& filepath)
{
	std::ifstream file;
	std::stringstream ss;

	file.open(filepath);

	if (file.is_open())
	{
		std::string line;
		while (getline(file, line))
		{
			ss << line << '\n';
		}
	}
	return ss;
}

unsigned int Shader::CreateShader(unsigned int type, const std::string& source)
{
	GLClearError(); unsigned int shaderid = glCreateShader(type); GLCheckError();
	const char* src = source.c_str();

	GLCall(glShaderSource(shaderid, 1, &src, nullptr));

	GLCall(glCompileShader(shaderid));
	int compile;
	GLCall(glGetShaderiv(shaderid, GL_COMPILE_STATUS, &compile));

	if (compile != GL_TRUE)
	{
		char message[1024];
		int log_length;
		GLCall(glGetShaderInfoLog(shaderid, 1024, &log_length, message));

		std::cout << "Shader Compilation Error: " << message << std::endl;
		return 0;
	}

	return shaderid;
}

void Shader::Use() const
{
	GLCall(glUseProgram(m_RendererID));
}

void Shader::UnUse() const
{
	GLCall(glUseProgram(0));
}

void Shader::SetUniformF(const std::string& name, float f0, float f1, float f2, float f3)
{
	if (m_UniformLocs.find(name) != m_UniformLocs.end())
	{
		GLCall(glUniform4f(m_UniformLocs[name], f0, f1, f2, f3));
		return;
	}

	GLClearError(); int location = glGetUniformLocation(m_RendererID, name.c_str()); GLCheckError();
	if (location != -1)
	{
		m_UniformLocs[name] = location;
		GLCall(glUniform4f(location, f0, f1, f2, f3));
	}
}