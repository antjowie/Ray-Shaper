#pragma once
#include "Object.h"

class ReflectionTile : public Object
{
private:
	const int m_id;

public:
	const enum Direction
	{
		// Special direction for the reflector, the non direction
		// Used to omit loading
		Reflector_Non = 0,
		RightUp,
		RightDown,
		LeftDown,
		LeftUp,
		Up,
		Down,
		Left,
		Right,

		// Used to loop around
		Capacity
	}m_direction;

	bool isGrabbed{ false };
	const bool canMove{ true };

	virtual void update(const float elapsedTime) override;
	virtual void move(const sf::Vector2f &movement) override;
	
	// Get the edges, used by laser to calculate acurate hitboxes
	virtual const std::vector<sf::Vector2f> getVertices() const;

	virtual std::map<std::string, std::string> getSaveData() const override;

	ReflectionTile(ObjectManager &objectManager, const int id, const sf::Vector2f &position);
};