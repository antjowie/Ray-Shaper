#include "DataManager.h"

#include "Player.h"

void ReflectionTile::update(const float elapsedTime)
{
	m_sprite.setTextureRect(isGrabbed ? sf::IntRect{16, 16 * (m_direction - 2), 16, 16} : sf::IntRect{ 0,16 * (m_direction - 2),16,16 });
}

void ReflectionTile::move(const sf::Vector2f & movement)
{
	sf::Vector2f movementCheck(movement);
	m_objectManager.fixMovement<Player*>(this, movementCheck,true,true);
	if (movementCheck != movement)
	{
		m_sprite.move(movementCheck.x, 0);
		isGrabbed = false;
	}
	else
	m_sprite.move(movement.x, 0);
}

ReflectionTile::ReflectionTile(ObjectManager &objectManager, const int id, const sf::Vector2f & position) :
	Object(objectManager),
	m_direction(static_cast<Direction>(id))
{
	m_sprite.setTexture(DataManager::getInstance().getData("reflectionTile").meta.texture);
	m_sprite.setTextureRect({ 0,16 * (id - Direction::RightUp), 16,16 });
	m_sprite.setPosition(position + sf::Vector2f{8, 8});
	m_sprite.setOrigin(8, 8);
}
