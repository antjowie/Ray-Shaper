#include "Emitter.h"
#include "DataManager.h"

#include "objects\Player.h"

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

	ReflectionTile * previousReflection{ nullptr };
	ReflectionTile * reflection{ nullptr };
	do
	{
	reflection = nullptr;
	sf::Vector2f movement{ direction * 16.f *100.f };
	// We check for intersections using the SAT theorem
	// Because all objects are normal to the grid, we only have to check one of the tiles
	for (auto &iter : m_objectManager.getObjects<ReflectionTile*>())
	{
		if (iter == previousReflection)
			continue;
		// See if laser collides with reflection tile
		if (intersects(m_vertices[m_vertices.getVertexCount() -1].position.x, m_vertices[m_vertices.getVertexCount() - 1].position.x + movement.x, iter->getHitbox().left, iter->getHitbox().left + iter->getHitbox().width) &&
			intersects(m_vertices[m_vertices.getVertexCount() - 1].position.y, m_vertices[m_vertices.getVertexCount() - 1].position.y + movement.y, iter->getHitbox().top, iter->getHitbox().top + iter->getHitbox().height))
			if (!reflection)
				reflection = iter;
			// Check if tile is closer
			else if(std::powf((iter->getHitbox().left + iter->getHitbox().width * 0.5f) - spawnPos.x, 2.f) +
					std::powf((iter->getHitbox().top + iter->getHitbox().height * 0.5f) - spawnPos.y, 2.f) <
					std::powf((reflection->getHitbox().left + reflection->getHitbox().width * 0.5f) - spawnPos.x, 2.f) +
					std::powf((reflection->getHitbox().top + reflection->getHitbox().height * 0.5f) - spawnPos.y, 2.f))
				reflection = iter;
	}
	previousReflection = reflection;
	// Recalculate laser direction
	// TODO
	// Calculate correct reflection
	// Use point of collision instead of center
	if (reflection)
	{
		switch (reflection->m_direction)
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
			m_vertices.append(sf::Vertex(reflection->getPosition(), sf::Color(58, 166, 62)));
	}
	// Calculate distance till first tile
	// TODO
	// Get calculation to first tile
	else
	{
		m_vertices.append(sf::Vertex(m_vertices[m_vertices.getVertexCount() -1].position + movement, sf::Color(58, 166, 62)));
	}
	} while (reflection);
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