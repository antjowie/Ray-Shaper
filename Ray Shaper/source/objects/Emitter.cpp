#include "Emitter.h"
#include "DataManager.h"

#include "objects\Player.h"
#include "objects\Gate.h"

#include <iostream>


bool Emitter::intersects(const double l1, const double r1, const double l2, const double r2) const
{
	return 
		(
			(l1 >= l2 && l1 <= r2) ||
			(l1 <= l2 && r1 >= r2) ||
			(r1 >= l2 && r1 <= r2) ||
			(l2 >= l1 && l2 <= r1) ||
			(l2 <= l1 && r2 >= r1) ||
			(r2 >= l1 && r2 <= r1)
		);
}

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
	Collided collided;

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
	{
		// Update laser if needed
		sf::Vector2f movement{ m_vertices[m_vertices.getVertexCount() - 1].position - m_vertices[m_vertices.getVertexCount() - 2].position };
		movement /= std::sqrtf(std::powf(movement.x, 2.f) + std::powf(movement.y, 2.f)); // Make unit vector and add onto the end position (fake collision)
		collided = raycastIntersection<Gate*>(m_vertices[m_vertices.getVertexCount()-2].position, m_vertices[m_vertices.getVertexCount() - 1].position + movement, false, true);
		if (collided.object)
			static_cast<Gate*>(collided.object)->laserHit = true;
		return;
	}

	// Postion is top left
	sf::Vector2f spawnPos{ getPosition()};
	sf::Vector2f direction{ 0,0 };
	// Lookup initial direction
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

	do
	{
		const sf::Vector2f movement{ direction * 100.f * 16.f };
		// Check for reflectionTile collision
		collided = raycastIntersection<ReflectionTile*>(spawnPos, spawnPos + movement,false,true);
		// If no collision with reflectionTile is met, check for tile collision
		if (!collided.object)
		{
			collided = raycastIntersection<>(spawnPos, spawnPos + movement, true, false);
			// If no collision with tile is met, check for gate collision
			// If no gate collision is met, level design is bad, because the ray had a larger distance then the max length, 
			if (!collided.tile)
			{
				collided = raycastIntersection<Gate*>(spawnPos, spawnPos + movement, false, true);
				if (!collided.object)
					spawnPos += movement;
				else
				{
					spawnPos = collided.point;
					static_cast<Gate*>(collided.object)->laserHit = true;
					collided.object = nullptr;
				}
			}
			else
				spawnPos = collided.point;
		}
		else
		{
			spawnPos = collided.point;
			// Update direction
			// TODO, update correctly
			switch (dynamic_cast<ReflectionTile*>(collided.object)->m_direction)
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
		m_vertices.append(sf::Vertex(spawnPos, sf::Color(58, 166, 62)));
	} while (collided.object);
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