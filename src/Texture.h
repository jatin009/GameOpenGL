#pragma once

class Texture
{
private:
	unsigned int m_RendererID;
	int m_Width, m_Height, m_BPP;
	unsigned char* m_LocalBuffer;

public:
	Texture() {}
	Texture(const char* filepath, int wrapMode, bool alpha = false);
	~Texture();

	void Bind(unsigned int unit);
	void UnBind();
};