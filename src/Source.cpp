#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexLayout.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK)
		std::cout << "Error" << std::endl;

	std::cout << glGetString(GL_VERSION) << std::endl;

	//Texture creation
	Texture tex("res/images/block.png", GL_CLAMP_TO_BORDER);
	Texture tex2("res/images/block_solid.png", GL_CLAMP_TO_BORDER);
	Texture bgTexture("res/images/background.jpg", GL_CLAMP_TO_BORDER);

	//Vertex Array creation
	VertexArray vao;
	vao.Bind();

	float positions[] = {
		//positions			//textures
		-180.0f, -180.0f,	0.0f, 0.0f,
		-180.0f, -130.0f,	0.0f, 1.0f,
		-130.0f, -130.0f,	1.0f, 1.0f,
		-130.0f, -180.0f,	1.0f, 0.0f
	};

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	//Vertex buffer creation
	VertexBuffer vbo(positions, sizeof(positions));

	//Index buffer creation
	IndexBuffer ibo(indices, sizeof(indices));

	//Adding layout on VertexArray
	VertexLayout layout;
	layout.Push<float>(2, 0);
	layout.Push<float>(2, (void*)(2 * sizeof(float)));
	layout.SetStride(4 * sizeof(float));
	vao.AddLayout(layout);

	//Shader creation part
	Shader shader("res/Vertex.shader", "res/Fragment.shader");
	shader.Use();
	
	//passing projection matrix
	glm::mat4 proj(1.0f);
	proj = glm::ortho(-200.0f, 200.0f, -200.0f, 200.0f, -2.0f, 2.0f);
	shader.SetUniform<glm::mat4>("u_Projection", proj);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);
		
		//background rendering
		glm::mat4 bgmodel(1.0f);
		bgmodel = glm::scale(bgmodel, glm::vec3(8.0, 8.0, 1.0));
		bgmodel = glm::translate(bgmodel, glm::vec3(155.0f, 155.0f, 0.0f));
		shader.SetUniform<glm::mat4>("u_Model", bgmodel);
		bgTexture.Bind(0);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

		//multiple objects creation at equal distance
		float x_Shift = 0.0f;
		for (int i = 0; i < 6; i++)
		{
			//passing model matrix
			glm::mat4 model(1.0f);
			model = glm::translate(model, glm::vec3(x_Shift, 0.0f, 0.0f));
			shader.SetUniform<glm::mat4>("u_Model", model);
			x_Shift += 55.0f;

			if (i % 2 == 0)
			{
				tex.Bind(0);
			}
			else
			{
				tex2.Bind(0);
			}
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
		}
		
		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}