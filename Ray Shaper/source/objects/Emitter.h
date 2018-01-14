#pragma once
#include <SFML\Graphics\VertexArray.hpp>

#include "Object.h"

class Emitter : public Object
{
private:
	const enum Direction
	{
		Up = 10,
		Down,
		Left,
		Right
	}m_direction;

	bool m_active{ false };

	sf::VertexArray m_vertices;

public:
	virtual void update(const float elapsedTime);

	Emitter(ObjectManager &objectManager, const int id, const sf::Vector2f &position);
};