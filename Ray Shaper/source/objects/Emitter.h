#pragma once
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
public:

	Emitter(ObjectManager &objectManager, const int id, const sf::Vector2f &position);
};