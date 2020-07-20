#pragma once
#include "GL/glew.h"
#include <iostream>

#define ASSERT(x) if(!(x)) __debugbreak();
#define GLCall(x) GLClearError(); \
	x;\
	ASSERT(GLCheckError());

static void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

static bool GLCheckError()
{
	while (GLenum error = glGetError())
	{
		std::cout << "Error Code: " << error << std::endl;
		return false;
	}
	return true;
}
