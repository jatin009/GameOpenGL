# GameOpenGL
First OpenGL game development
# Learnings

- Game.Render()
	should be as light as possible. No repeated creation should be there in this function.
	
- while() loop
	ProcessInput() and Update() calls go after glfwPollEvents()
	Render() after glClear(COLOR_BIT)
	
- Drawing multiple objects using one VAO, VBO
	position all at (0,0) -> (1,1)
	then scale and translate (code wise - translate and then scale)
	
- Object composition
	Renderer has shader
	$ init()- initializes VAO - VBO
			- sets shader.Use() and then assigns projection to shader
			   
	$ drawSprite()- calls glDrawElements
				  -	every object should have its own Draw() method. And to render that object,
					we just have to call obj.Draw(*m_Renderer)
				  - A central Game class for containing & initializing various game objects.
					The class has ProcessInput()
								  Update()
								  Render(), from where we issue the Draw() call on various objects.
				  - If a sprite has a transparent part, the textures image format & internal format
					should be GL_RGBA. Also use GL_BLEND in main() for alpha.