#include "GL\glew.h"
#include "SpriteRenderer.h"
#include "Texture.h"
#include "GLlogs.h"
#include <iostream>

SpriteRenderer::SpriteRenderer(Shader& sh): m_Shader(sh)
{
}

void SpriteRenderer::InitRendererData(glm::vec2 size)
{
	float positions[] = {
		// positions		// textures
		0.0f, 0.0f,	0.0f, 0.0f,
		0.0f, 1.0f,	0.0f, 1.0f,
		1.0f, 1.0f,	1.0f, 1.0f,
		1.0f, 0.0f,	1.0f, 0.0f
	};

	unsigned int buffer;
	GLCall(glGenBuffers(1, &buffer));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));
	GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW));

	GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0));
	GLCall(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float))));
	GLCall(glEnableVertexAttribArray(0));
	GLCall(glEnableVertexAttribArray(1));

	int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	unsigned int ibo;
	GLCall(glGenBuffers(1, &ibo));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));

	m_Shader.Use();
	glm::mat4 proj(1.0f);
	proj = glm::ortho(0.0f, static_cast<float>(size.x), 0.0f, static_cast<float>(size.y), -1.0f, 1.0f);
	m_Shader.SetUniform<glm::mat4>("u_Projection", proj);

}

void SpriteRenderer::DrawSprite(Texture& texture2D, glm::vec3 color, glm::vec3 move_by, glm::vec3 scale_by)
{
	texture2D.Bind(0);
	glm::mat4 model(1.0f);

	//we need to scale first and then rotate, therefore reversing them in operation
	//because that's how it takes
	model = glm::translate(model, move_by);
	model = glm::scale(model, scale_by);

	m_Shader.SetUniform<glm::mat4>("u_Model", model);
	m_Shader.SetUniformF("u_Color", color.x, color.y, color.z, 1.0f);
	GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
}
