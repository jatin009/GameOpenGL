#include "GL\glew.h"
#include "GLFW\glfw3.h"
#include "Game.h"
#include "GLlogs.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>

std::vector< std::vector<int> > ReadLevelsFile()
{
	std::ifstream file;
	std::stringstream ss;

	file.open("res/Levels.lvl");
	std::vector<std::vector<int> > tileData;

	if (file.is_open())
	{
		std::string line;
		std::vector<int> tileRow;
		while (getline(file, line))
		{
			std::cout << line << std::endl;
			ss << line;

			int tileCode;
			while (ss >> tileCode)
				tileRow.push_back(tileCode);
			tileData.push_back(tileRow);

			ss.clear();
			tileRow.clear();
		}
	}
	return tileData;
}

int main()
{
	const unsigned int WIDTH = 800;
	const unsigned int HEIGHT = 600;

	Game Arena(WIDTH, HEIGHT);

	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(WIDTH, HEIGHT, "Breakout", NULL, NULL);
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

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	std::vector< std::vector<int> > tilesVec = ReadLevelsFile();
	Arena.Init(tilesVec);

	// deltaTime variables
	// -------------------
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		// calculate delta time
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		/* Poll for and process events */
		GLCall(glfwPollEvents());

		Arena.ProcessInput(window, deltaTime);
		Arena.Update(deltaTime);
		
		/* Render here */
		GLCall(glClear(GL_COLOR_BUFFER_BIT));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
		Arena.Render();

		/* Swap front and back buffers */
		GLCall(glfwSwapBuffers(window));
	}

	glfwTerminate();
	return 0;
}