#include "ReflectionTile.h"
#include "DataManager.h"

void ReflectionTile::update(const float elapsedTime)
{
	m_sprite.setTextureRect(isGrabbed ? sf::IntRect{16, 16 * (m_direction - 2), 16, 16} : sf::IntRect{ 0,16 * (m_direction - 2),16,16 });
}

void ReflectionTile::move(const sf::Vector2f & movement)
{
	sf::Vector2f movementCheck(movement);
	m_objectManager.fixMovement(this, movementCheck);
	if (movementCheck != movement)
		isGrabbed = false;
	else
		m_sprite.move(movement);
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
