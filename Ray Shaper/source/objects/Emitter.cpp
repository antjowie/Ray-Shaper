#include "Emitter.h"
#include "DataManager.h"

#include "objects\Player.h"

#include <iostream>

void Emitter::update(const float elapsedTime)
{
	// When emitter is inactive, lasers will not be casted, so we can return out of this function early
	if (!m_active)
	{
		sf::FloatRect hitbox{ getHitbox() };
		
		// Hitbox offset, we make the hitbox one pixel wider so that if player stands right next to it
		// the emitter will be activated
		hitbox = { hitbox.left - 1, hitbox.top - 1, hitbox.width + 2, hitbox.height + 2 };
		for (auto &iter : m_objectManager.getObjects<Player*>())
			if (iter->getHitbox().intersects(hitbox))
			{
				m_active = true;
				m_sprite.setTextureRect({ 16, m_sprite.getTextureRect().top, 16, 16 });
			}
		return;
	}

	// Check if reflection tiles have been moved, if they have, recalculate laser
	const std::vector<ReflectionTile*> &newTiles{ m_objectManager.getObjects<ReflectionTile*>() };
	bool same{ newTiles.size() == m_oldTiles.size() };
	if(!same)
	for(auto &iter: newTiles)
		m_oldTiles.push_back(iter->getHitbox().left);
	else 
		for (size_t i{ 0 }; i < m_oldTiles.size(); i++)
			if (m_oldTiles[i] != newTiles[i]->getHitbox().left)
			{
				same = false;
				m_oldTiles[i] = newTiles[i]->getHitbox().left;
			}
	if (same)
		return;

	// Postion is top left
	sf::Vector2f spawnPos{ getPosition() };
	sf::Vector2f direction{ 0,0 };
	switch (m_direction)
	{
	case Up:
		spawnPos += {8, 0};
		direction = { 0,-1 };
		break;
	case Down:
		spawnPos += {8, 16};
		direction = { 0,1 };
		break;
	case Left:
		spawnPos += {0, 8};
		direction = { -1,0 };
		break;
	case Right:
		spawnPos += {16, 8};
		direction = { 1,0 };
		break;
	}

	m_vertices.clear();
	m_vertices.append(sf::Vertex(spawnPos, sf::Color(58, 166, 62)));

	// Check in a radius of 100 tiles, its assuming that the nearest collision is in this size, but I'm going to make sure that it is in the bounds
	// by game design. Else, I could make it a loop and add until a collision is met but that would repeat the loop plenty of times.
	// The cost won't be too high, but I don't think it will be neccessary to implement a loop for it
	Object * returned{this};
	do
	{
	sf::Vector2f movement{ direction * 16.f*100.f };
	returned = m_objectManager.fixMovement<Player*>(returned, movement,true);
	spawnPos += movement;
	if (dynamic_cast<ReflectionTile*>(returned))
	{
		spawnPos = { returned->getHitbox().left + returned->getHitbox().width / 2.f, returned->getHitbox().top + returned->getHitbox().height / 2.f };
		// This is not refleciton maths but will be used as a temporary solution
		switch (dynamic_cast<ReflectionTile*>(returned)->m_direction)
		{
		case ReflectionTile::Direction::RightUp:
			if (direction.x < 0)
				direction = { 0,-1 };
			else
				direction = { 1,0 };
			break;

		case ReflectionTile::Direction::RightDown:
			if (direction.x < 0)
				direction = { 0,1 };
			else
				direction = { 1,0 };
			break;

		case ReflectionTile::Direction::LeftDown:
			if (direction.x > 0)
				direction = { 0,1 };
			else
				direction = { -1,0 };
			break;

		case ReflectionTile::Direction::LeftUp:
			if (direction.x > 0)
				direction = { 0,-1 };
			else
				direction = { -1,0 };
			break;
		case ReflectionTile::Direction::Up:
			break;
		case ReflectionTile::Direction::Down:
			break;
		case ReflectionTile::Direction::Left:
			break;
		case ReflectionTile::Direction::Right:
			break;
		}
	}
	else
	{
		if (direction.x > 0)
			spawnPos += {8, 0};
		else if (direction.x < 0)
			spawnPos += {-8, 0};
		else if (direction.y > 0)
			spawnPos += {0, 8};
		else if (direction.y < 0)
			spawnPos += {0, -8};
	}
	m_vertices.append(sf::Vertex(spawnPos, sf::Color(58, 166, 62)));

	} while (dynamic_cast<ReflectionTile*>(returned));
}

sf::VertexArray & Emitter::getVertices()
{
	return m_vertices;
}

Emitter::Emitter(ObjectManager & objectManager, const int id, const sf::Vector2f & position) :
	Object(objectManager), m_direction(static_cast<Direction>(id)), m_vertices(sf::PrimitiveType::LinesStrip)
{
	m_sprite.setTexture(DataManager::getInstance().getData("emitter").meta.texture);
	m_sprite.setTextureRect({ m_active ? 16 : 0,(id - Direction::Up) * 16,16,16 });
	m_sprite.setPosition(position);
}