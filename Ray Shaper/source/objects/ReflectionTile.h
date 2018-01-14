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

	// Offset puts its value inside of this, when its higher then the offset value,
	// This value will be used to fill the position in the tile because it was behind
	float offsetBuffer{ 0 };
public:
	bool isGrabbed{ false };

	virtual void update(const float elapsedTime) override final;
	virtual void move(const sf::Vector2f &movement) override final;

	ReflectionTile(ObjectManager &objectManager, const int id, const sf::Vector2f &position);
};