#pragma once
#include <SFML\Graphics\VertexArray.hpp>

#include "Object.h"
#include "ReflectionTile.h"

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

	// Compares this to the new tiles, if one tile moved, recalculate laser
	std::vector<float> m_oldTiles;
	sf::VertexArray m_vertices;
	// This function will take the closest collision between a object or tile
	struct Collided
	{
		sf::Vector2f point{ 0,0 };
		Object* object;
		Tile* tile;
	};
	
	// Checks if the two ranges intersect eachother
	bool intersects(const double left1, const double right1, const double left2, const double right2) const;

public:
	virtual void update(const float elapsedTime)override final;

	// This has to be loaded into the game menu class
	sf::VertexArray &getVertices();
	Emitter(ObjectManager &objectManager, const int id, const sf::Vector2f &position);
};