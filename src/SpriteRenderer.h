#pragma once
#include "glm/gtc/matrix_transform.hpp"
#include "Shader.h"

class Texture;

class SpriteRenderer
{
public:
	SpriteRenderer(Shader& sh);
	void DrawSprite(Texture& texture2D, glm::vec3 color, glm::vec3 move_by, glm::vec3 scale_by = glm::vec3(1.0, 1.0, 1.0));
	void InitRendererData(glm::vec2 size);

private:
	Shader m_Shader;
};