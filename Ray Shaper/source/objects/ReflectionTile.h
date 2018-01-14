#pragma once
#include "Object.h"

class ReflectionTile : public Object
{
private:
	const enum Direction
	{
		RightUp = 2,
		RightDown,
		LeftDown,
		LeftUp,
		Up,
		Down,
		Left,
		Right
	}m_direction;
public:
	bool isGrabbed{ false };

	virtual void update(const float elapsedTime) override final;
	virtual void move(const sf::Vector2f &movement) override final;

	ReflectionTile(ObjectManager &objectManager, const int id, const sf::Vector2f &position);
};