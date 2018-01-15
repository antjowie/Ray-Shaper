#include "DataManager.h"

#include "Player.h"

void ReflectionTile::update(const float elapsedTime)
{
	int left = isGrabbed ? 16 : 0;
	sf::IntRect textureRect{ m_sprite.getTextureRect() };
	textureRect.left = left;
	m_sprite.setTextureRect(textureRect);
}

void ReflectionTile::move(const sf::Vector2f & movement)
{
	sf::Vector2f movementCheck(movement);
	m_objectManager.fixMovement<Player*>(this, movementCheck,true,true);
	// If player jumps or walks into objects that the tile can't pass
	if (movementCheck != movement || movement.y != 0)
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
	m_sprite.setTextureRect({ 0,17 * (id - Direction::RightUp), 16,16 });
	m_sprite.setPosition(position + sf::Vector2f{8, 8});
	m_sprite.setOrigin(8, 8);
}
