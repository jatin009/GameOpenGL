#include "GL/glew.h"
#include "Game.h"
#include "Shader.h"
#include <iostream>

Game::Game(unsigned int w, unsigned int h)
	: m_Width(w)
	, m_Height(h)
	, m_Renderer(nullptr)
	, m_Player(nullptr)
	, m_Ball(nullptr)
	, m_State(GAME_IDLE)
{
}

void Game::Init(const std::vector< std::vector <int> >& tilesVec)
{
	float BRICK_WIDTH = (float)m_Width / tilesVec[0].size();
	float BRICK_HEIGHT = m_Height / (2*tilesVec.size());

	Shader sh("res/shaders/vertex.shader", "res/shaders/fragment.shader");
	
	m_Renderer = new SpriteRenderer(sh);
	m_Renderer->InitRendererData(glm::vec2(m_Width, m_Height));

	//Texture creation
	Texture block_tex("res/images/block.png", GL_CLAMP_TO_BORDER);
	Texture block_solid_tex("res/images/block_solid.png", GL_CLAMP_TO_BORDER);
	Texture paddle_tex("res/images/paddle.png", GL_CLAMP_TO_BORDER, true);
	Texture ball_tex("res/images/awesomeface.png", GL_CLAMP_TO_BORDER, true);

	float x_shift = 0.0f, y_shift = m_Height-BRICK_HEIGHT;
	for (auto itr = tilesVec.begin(); itr != tilesVec.end(); itr++)
	{
		auto rowVec = (*itr);
		glm::vec3 color;
		for (auto level = rowVec.begin(); level != rowVec.end(); level++)
		{
			glm::vec3 color;

			if (*level == 5)
				color = glm::vec3(1.0f, 0.5f, 0.0f);
			else if (*level == 4)
				color = glm::vec3(0.8f, 0.8f, 0.4f);
			else if (*level == 3)
				color = glm::vec3(0.0f, 0.7f, 0.0f);
			else if (*level == 2)
				color = glm::vec3(0.2f, 0.6f, 1.0f);
			else if (*level == 1)
				color = glm::vec3(0.8f, 0.8f, 0.7f);

			if (*level > 1)
				m_Bricks.push_back(BrickObject(x_shift, y_shift, BRICK_WIDTH, BRICK_HEIGHT, block_tex, false, color));
			else if (*level == 1)
				m_Bricks.push_back(BrickObject(x_shift, y_shift, BRICK_WIDTH, BRICK_HEIGHT, block_solid_tex, true, color));

			x_shift += BRICK_WIDTH;
		}
		y_shift -= BRICK_HEIGHT;
		x_shift = 0.0f;
	}

	m_Player = new GameObject(300.0f, 0.0f, 100.0, 20.0, paddle_tex, glm::vec3(1.0f), glm::vec2(500.0f, 0.0f));
	m_Ball = new GameObject(340.0f, 20.0f, 25.0, 25.0, ball_tex, glm::vec3(1.0f), glm::vec2(100.0f, 350.0f));
}

void Game::ProcessInput(GLFWwindow* win, float dt)
{
	float xpos = m_Player->Position.x;
	float PLAYER_VEL = m_Player->Velocity.x* dt;

	// player movement
	if (glfwGetKey(win, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		if (xpos > 0.0f)
		{
			m_Player->Move(glm::vec2(-PLAYER_VEL, 0.0f));
			if (m_State == GAME_IDLE)		// if game is idle, move ball on bar
				m_Ball->Move(glm::vec2(-PLAYER_VEL, 0.0f));
		}
	}
	if (glfwGetKey(win, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		if (xpos + m_Player->Size.first < m_Width)
		{
			m_Player->Move(glm::vec2(PLAYER_VEL, 0.0f));
			if (m_State == GAME_IDLE)		// if game is idle, move ball on bar
				m_Ball->Move(glm::vec2(PLAYER_VEL, 0.0f));
		}
	}

	glm::vec2 BALL_VELOCITY = m_Ball->Velocity*dt;
	// ball movement
	if (m_State == GAME_IDLE)
	{		
		if (glfwGetKey(win, GLFW_KEY_SPACE) == GLFW_PRESS)
		{
			m_State = GAME_PLAY;
			m_Ball->Move(BALL_VELOCITY);
		}
	}
	else if (m_State == GAME_PLAY)
	{
		m_Ball->Move(BALL_VELOCITY);
		glm::vec2 ball_pos = m_Ball->Position;

		if (ball_pos.x > m_Width - m_Ball->Size.first
			|| ball_pos.x < 0.0f)															// reflection from sides
		{
			m_Ball->Velocity.x = -m_Ball->Velocity.x;
		}
		else if (ball_pos.y > m_Height - m_Ball->Size.second)		// reflection from top
		{
			m_Ball->Velocity.y = -m_Ball->Velocity.y;
		}
		else if (ball_pos.y < 0.0f)		// game over
		{
			m_State = GAME_IDLE;

			// resetting bricks
			for (BrickObject& brick : m_Bricks)
			{
				brick.Destroyed = false;
			}

			// resetting ball and bar
			m_Player->Position = glm::vec2(300.0f, 0.0f);
			m_Ball->Position = glm::vec2(340.0f, 20.0f);
		}
	}
}

void Game::Update(float dt)
{
	if (m_State == GAME_PLAY)
	{
		for (BrickObject& brick: m_Bricks)
		{
			if (!brick.Destroyed)
			{
				// AABB-AABB collision
				// collision X axis
/*				bool collisionX = (m_Ball->Position.x + m_Ball->Size.first > brick.Position.x)
					&& (m_Ball->Position.x + m_Ball->Size.first < brick.Position.x + brick.Size.first);

				// collision Y axis
				bool collisionY = (m_Ball->Position.y + m_Ball->Size.second > brick.Position.y)
					&& (m_Ball->Position.y + m_Ball->Size.second < brick.Position.y + brick.Size.second);
					
				brick.Destroyed = collisionX && collisionY;
*/
				// AABB-Circle collision
				Collision coll_params = CheckCollision(m_Ball, &brick);

				if (std::get<0>(coll_params))
				{
					Direction dr = std::get<1>(coll_params);
					if (dr == LEFT || dr == RIGHT)
					{
						float penetration = m_Ball->Size.first/2.0f - std::abs(std::get<2>(coll_params).x);
						m_Ball->Velocity.x = -m_Ball->Velocity.x;
						if (dr == LEFT)
							m_Ball->Move(glm::vec2(penetration, 0.0f));
						else
							m_Ball->Move(glm::vec2(-penetration, 0.0f));
					}
					else if (dr == UP || dr == DOWN)
					{
						float penetration = m_Ball->Size.first / 2.0f - std::abs(std::get<2>(coll_params).y);
						m_Ball->Velocity.y = -m_Ball->Velocity.y;
						if (dr == UP)
							m_Ball->Move(glm::vec2(0.0f, -penetration));
						else
							m_Ball->Move(glm::vec2(0.0f, penetration));
					}
					if (!brick.Solid)
						brick.Destroyed = true;
				}
			}
		}

		// Ball - Player collision
		Collision coll_params = CheckCollision(m_Ball, m_Player);
		if (std::get<0>(coll_params))
		{
			float centerboard = m_Player->Position.x + m_Player->Size.first/2.0f;
			float distance = m_Ball->Position.x + m_Ball->Size.first / 2.0f - centerboard;
			float percentage = distance / (m_Player->Size.first / 2.0f);

			glm::vec2 oldVel = m_Ball->Velocity;
			m_Ball->Velocity.x = 100.0f * percentage * 2.0f;
			m_Ball->Velocity.y = std::abs(m_Ball->Velocity.y);
			m_Ball->Velocity = glm::normalize(m_Ball->Velocity) * glm::length(oldVel);
		}
	}
}

Game::Collision Game::CheckCollision(GameObject* ball, GameObject* obj)
{
	glm::vec2 obj_center = glm::vec2(obj->Position.x + obj->Size.first / 2.0f, obj->Position.y + obj->Size.second / 2.0f);
	glm::vec2 ball_center = glm::vec2(ball->Position.x + ball->Size.first / 2.0f, ball->Position.y + ball->Size.second / 2.0f);
	glm::vec2 diff = ball_center - obj_center;
	glm::vec2 clamped_diff = glm::vec2(
		abs(diff.x) <= obj->Size.first / 2.0f ? diff.x : (diff.x < 0 ? -obj->Size.first / 2.0f : obj->Size.first / 2.0f),
		abs(diff.y) <= obj->Size.second / 2.0f ? diff.y : (diff.y < 0 ? -obj->Size.second / 2.0f : obj->Size.second / 2.0f)
	);
	glm::vec2 closest = clamped_diff + obj_center;
	diff = closest - ball_center;
	bool collision = glm::length(diff) < ball->Size.first / 2.0f;
	if (collision)
		return Collision(true, VectorDirection(diff), diff);
	else
		return Collision(false, NONE, glm::vec2(0.0f, 0.0f));
}

Game::Direction Game::VectorDirection(glm::vec2 direction)
{
	glm::vec2 compass[] = {
		glm::vec2(-1.0f, 0.0f),
		glm::vec2(0.0f, 1.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(0.0f, -1.0f)
	};
	float max = 0.0f;
	unsigned int best_match = -1;
	for (unsigned int i = 0; i < 4; i++)
	{
		float dot_prod = glm::dot(glm::normalize(direction), compass[i]);
		if (dot_prod > max)
		{
			max = dot_prod;
			best_match = i;
		}
	}
	return Direction(best_match);
}

void Game::Render()
{
	//Texture creation
	static Texture block_tex("res/images/block.png", GL_CLAMP_TO_BORDER);
	static Texture paddle_tex("res/images/paddle.png", GL_CLAMP_TO_BORDER, true);
	static Texture ball_tex("res/images/awesomeface.png", GL_CLAMP_TO_BORDER, true);
	static Texture bg_tex("res/images/background.jpg", GL_CLAMP_TO_BORDER);

	//m_Player->Draw(*m_Renderer);
	m_Renderer->DrawSprite(bg_tex, glm::vec3(1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(m_Width, m_Height, 1.0));
	m_Renderer->DrawSprite(paddle_tex, m_Player->Color, glm::vec3(m_Player->Position.x, m_Player->Position.y, 0.0f), glm::vec3(m_Player->Size.first, m_Player->Size.second, 1.0));
	for (auto itr = m_Bricks.begin(); itr != m_Bricks.end(); itr++)
	{
		auto brick = *itr;
		if (!brick.Destroyed)
			m_Renderer->DrawSprite(block_tex, brick.Color, glm::vec3(brick.Position.x, brick.Position.y, 0.0f), glm::vec3(brick.Size.first, brick.Size.second, 1.0));
		//brick.Draw(*m_Renderer);
	}	
	m_Renderer->DrawSprite(ball_tex, m_Ball->Color, glm::vec3(m_Ball->Position.x, m_Ball->Position.y, 0.0f), glm::vec3(m_Ball->Size.first, m_Ball->Size.second, 1.0));
}
