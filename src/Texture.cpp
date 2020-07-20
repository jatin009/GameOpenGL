#include "GL/glew.h"
#include "Texture.h"
#include "GLlogs.h"
#include "stb_image.h"
#include <string>
#include <iostream>

Texture::Texture(const char* filepath, int wrapMode, bool alpha)
{
	GLCall(glGenTextures(1, &m_RendererID));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

	//set the texture filtering options
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));

	stbi_set_flip_vertically_on_load(1);

	m_LocalBuffer = stbi_load(filepath, &m_Width, &m_Height, &m_BPP, 0);
	unsigned int format = alpha ? GL_RGBA : GL_RGB;

	if (m_LocalBuffer)
	{
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, format, m_Width, m_Height, 0, format, GL_UNSIGNED_BYTE, m_LocalBuffer));
	}
	else
	{
		std::cout << "Image file not loaded successfully - " << filepath << "." << std::endl;
	}
	stbi_image_free(m_LocalBuffer);

	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

Texture::~Texture()
{
	GLCall(glDeleteTextures(1, &m_RendererID));
}

void Texture::Bind(unsigned int unit)
{
	GLCall(glActiveTexture(GL_TEXTURE0 + unit));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}

void Texture::UnBind()
{
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}