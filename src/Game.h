#pragma once
#include "GLFW\glfw3.h"
#include "SpriteRenderer.h"
#include "Texture.h"
#include "GameObject.h"

class Game
{
public:
	
	enum Direction
	{
		LEFT,
		UP,
		RIGHT,
		DOWN,
		NONE
	};

	Game(unsigned int w, unsigned int h);

	enum GameState
	{
		GAME_IDLE,
		GAME_PLAY
	};

	void Init(const std::vector< std::vector <int> >& tilesVec);
	void ProcessInput(GLFWwindow* win, float dt);
	void Update(float dt);
	void Render();

private:	//members
	unsigned int m_Width, m_Height;
	SpriteRenderer *m_Renderer;
	std::vector<BrickObject> m_Bricks;
	GameObject* m_Player;
	GameObject* m_Ball;
	GameState m_State;

	typedef std::tuple<bool, Direction, glm::vec2> Collision;

	//methods
	Direction VectorDirection(glm::vec2 dir);
	Collision CheckCollision(GameObject*, GameObject*);
};
