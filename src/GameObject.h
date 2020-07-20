#pragma once

#include "Texture.h"

struct GameObject
{
	GameObject(float xpos, float ypos, float width, float height, Texture& tex, glm::vec3 color = glm::vec3(1.0f), glm::vec2 vel = glm::vec2(0.0f, 0.0f))
		: Position(xpos, ypos)
		, Size(std::make_pair(width, height))
		, Color(color)
		, Sprite()
		, Velocity(vel)
	{
	}

	glm::vec2 Position;	//position
	std::pair<float, float> Size;
	glm::vec3 Color;
	Texture Sprite;
	glm::vec2 Velocity;

	void Draw(SpriteRenderer& renderer)
	{
		//renderer.DrawSprite(m_Sprite, m_Color, glm::vec3(m_Pos.x, m_Pos.y, 0.0f), glm::vec3(m_Size.first, m_Size.second, 1.0));
	}

	void Move(glm::vec2 move_x_y)
	{
		Position.x += move_x_y.x;
		Position.y += move_x_y.y;
	}
};

struct BrickObject : public GameObject
{
	BrickObject(float xpos, float ypos, float width, float height, Texture& tex, bool solid, glm::vec3 color = glm::vec3(1.0f), glm::vec2 vel = glm::vec2(0.0f, 0.0f)) : GameObject(xpos, ypos, width, height, tex, color, vel)
		, Destroyed(false)
		, Solid(solid) {}

	bool Destroyed;
	bool Solid;
};
