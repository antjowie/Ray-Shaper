#include "DataManager.h"
#include "ReflectionTile.h"
#include "Player.h"

void ReflectionTile::update(const float elapsedTime)
{
	if (!canMove)
		return;
	int left = isGrabbed ? 17 : 0;
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

const std::vector<sf::Vector2f> ReflectionTile::getVertices() const
{
	std::vector<sf::Vector2f> returner;
	switch (m_direction)
	{
	case ReflectionTile::Direction::RightUp:
		returner.push_back({ getHitbox().left , getHitbox().top});
		returner.push_back({ getHitbox().left + getHitbox().width, getHitbox().top + getHitbox().height });
		returner.push_back({ getHitbox().left, getHitbox().top + getHitbox().height });
		break;

	case ReflectionTile::Direction::RightDown:
		returner.push_back({ getHitbox().left , getHitbox().top });
		returner.push_back({ getHitbox().left + getHitbox().width, getHitbox().top});
		returner.push_back({ getHitbox().left, getHitbox().top + getHitbox().height });
		break;

	case ReflectionTile::Direction::LeftDown:
		returner.push_back({ getHitbox().left , getHitbox().top });
		returner.push_back({ getHitbox().left + getHitbox().width, getHitbox().top});
		returner.push_back({ getHitbox().left + getHitbox().width, getHitbox().top + getHitbox().height });
		break;

	case ReflectionTile::Direction::LeftUp:
		returner.push_back({ getHitbox().left + getHitbox().width , getHitbox().top });
		returner.push_back({ getHitbox().left + getHitbox().width, getHitbox().top + getHitbox().height });
		returner.push_back({ getHitbox().left, getHitbox().top + getHitbox().height });
		break;
	case ReflectionTile::Direction::Up:
		returner.push_back({ getHitbox().left , getHitbox().top });
		returner.push_back({ getHitbox().left + getHitbox().width, getHitbox().top });
		returner.push_back({ getHitbox().left + getHitbox().width, getHitbox().top + getHitbox().height });
		returner.push_back({ getHitbox().left, getHitbox().top + getHitbox().height }); 
		break;
	case ReflectionTile::Direction::Down:
		returner.push_back({ getHitbox().left , getHitbox().top });
		returner.push_back({ getHitbox().left + getHitbox().width, getHitbox().top });
		returner.push_back({ getHitbox().left + getHitbox().width, getHitbox().top + getHitbox().height });
		returner.push_back({ getHitbox().left, getHitbox().top + getHitbox().height }); 
		break;
	case ReflectionTile::Direction::Left:
		returner.push_back({ getHitbox().left , getHitbox().top });
		returner.push_back({ getHitbox().left + getHitbox().width, getHitbox().top });
		returner.push_back({ getHitbox().left + getHitbox().width, getHitbox().top + getHitbox().height });
		returner.push_back({ getHitbox().left, getHitbox().top + getHitbox().height }); 
		break;
	case ReflectionTile::Direction::Right:
		returner.push_back({ getHitbox().left , getHitbox().top });
		returner.push_back({ getHitbox().left + getHitbox().width, getHitbox().top });
		returner.push_back({ getHitbox().left + getHitbox().width, getHitbox().top + getHitbox().height });
		returner.push_back({ getHitbox().left, getHitbox().top + getHitbox().height });		
		break;
	}

	return returner;
}

std::map<std::string, std::string> ReflectionTile::getSaveData() const
{
	std::map<std::string, std::string> returner(Object::getSaveData());
	returner["id"] = std::to_string(m_id);

	return returner;
}

ReflectionTile::ReflectionTile(ObjectManager &objectManager, const int id, const sf::Vector2f & position) :
	Object(objectManager),
	m_id(id),
	canMove(id < 9 ? true:false),
	// Loop around texture and add one if looping
	m_direction(static_cast<Direction>(id% static_cast<int>(Direction::Capacity) + (id >= static_cast<int>(Direction::Capacity) ? 1:0)))
{
	m_sprite.setTexture(DataManager::getInstance().getData("reflectionTile").meta.texture);
	m_sprite.setTextureRect({ canMove ? 0 : 17,17 * (static_cast<int>(m_direction) - 1), 16,16 });
	m_sprite.setPosition(position + sf::Vector2f{8, 8});
	m_sprite.setOrigin(8, 8);
}
