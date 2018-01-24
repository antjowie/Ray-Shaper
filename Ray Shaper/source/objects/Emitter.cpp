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

	/*
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
		Collided collided;
		// Update laser if needed
		sf::Vector2f movement{ m_vertices[m_vertices.getVertexCount() - 1].position - m_vertices[m_vertices.getVertexCount() - 2].position };
		movement /= std::sqrtf(std::powf(movement.x, 2.f) + std::powf(movement.y, 2.f)); // Make unit vector and add onto the end position (fake collision)
		collided = raycastIntersection<Gate*>(m_vertices[m_vertices.getVertexCount()-2].position, m_vertices[m_vertices.getVertexCount() - 1].position + movement, false, true);
		if (collided.object)
			static_cast<Gate*>(collided.object)->laserHit = true;
		return;
	}
	*/

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
	m_vertices.append(sf::Vertex(spawnPos, sf::Color(58, 166, 62,0)));
	Collided * closestCollision{ nullptr };

	do
	{
		// Used for last fade
		bool gateHit{ false };
		const sf::Vector2f movement{ direction * 100.f * 16.f };

		// Calculate closest of the three tiles
		Collided rtile(raycastIntersection<ReflectionTile*>(spawnPos, spawnPos + movement,false,true));
		Collided tile(raycastIntersection<>(spawnPos, spawnPos + movement, true, false));
		Collided gate(raycastIntersection<Gate*>(spawnPos, spawnPos + movement, false, true));

		std::cout << rtile.percentage << '\n';

		closestCollision = rtile.percentage < tile.percentage ? &rtile : &tile;
		closestCollision = closestCollision->percentage < gate.percentage ? closestCollision : &gate;

		// If it is gate
		if (dynamic_cast<Gate*>(closestCollision->object))
		{
			// If gate immediatly is hit
			if (m_vertices.getVertexCount() == 1)
				m_vertices.append({ spawnPos + ((closestCollision->point - spawnPos) *0.5f), sf::Color(58, 166, 62)});
			static_cast<Gate*>(closestCollision->object)->laserHit = true;
			gateHit = true;
			closestCollision->object = nullptr;
		}
		// If it is reflection tile
		//else if(!closestCollision->tile)
		else if (dynamic_cast<ReflectionTile*>(closestCollision->object))
		{
			// Update direction
			// Get vectors relative to collision point
			sf::Vector2f b{ closestCollision->endVertex - closestCollision->point };
			sf::Vector2f a{ spawnPos - closestCollision->point };
			sf::Vector2f ra{ closestCollision->point - spawnPos };

			// Get correct projection
			sf::Vector2f normal{ Math::projectionUnit(a, closestCollision->normal1) };
			if (normal == sf::Vector2f{ 0, 0 })
				normal = Math::projectionUnit(a, closestCollision->normal2);

			// Duplicate projection
			normal *= 2.f;

			// Add first vector to projection
			spawnPos = normal + closestCollision->point + ra;

			// Update the direction in the new direction
			direction = spawnPos - closestCollision->point;
			Math::normalizeVector(direction);
		}
		else if(!closestCollision->tile)
		{
			closestCollision->point = spawnPos;
		}
		spawnPos = closestCollision->point;
		m_vertices.append(sf::Vertex(spawnPos, sf::Color(58, 166, 62,gateHit?0:255)));
	} while (closestCollision->object);
}

sf::VertexArray & Emitter::getVertices()
{
	return m_vertices;
}

Emitter::Emitter(ObjectManager & objectManager, const int id, const sf::Vector2f & position, const bool activated) :
	Object(objectManager), m_direction(static_cast<Direction>(id)), m_vertices(sf::PrimitiveType::LinesStrip),
	m_active(activated)
{
	m_sprite.setTexture(DataManager::getInstance().getData("emitter").meta.texture);
	m_sprite.setTextureRect({ m_active ? 16 : 0,(id - Direction::Up) * 16,16,16 });
	m_sprite.setPosition(position);
}

std::map<std::string, std::string> Emitter::getSaveData() const
{
	std::map<std::string, std::string> returner (Object::getSaveData());
	returner["state"] = m_active ? '1': '0';
	returner["id"] = std::to_string(m_direction);

	return returner;
}
