#include "Emitter.h"
#include "DataManager.h"

Emitter::Emitter(ObjectManager & objectManager, const int id, const sf::Vector2f & position):
	Object(objectManager), m_direction(static_cast<Direction>(id))
{
	m_sprite.setTexture(DataManager::getInstance().getData("emitter").meta.texture);
	m_sprite.setTextureRect({ 0,(id - Direction::Up) * 16,16,16 });
	m_sprite.setPosition(position);
}
