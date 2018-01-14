#include "Emitter.h"
#include "DataManager.h"

#include "objects\Player.h"

void Emitter::update(const float elapsedTime)
{
	// When emitter is inactive, lasers will not be casted, so we can return out of this function early
	if (!m_active)
	{
		sf::FloatRect hitbox{ getHitbox() };
		hitbox = { hitbox.left - 1, hitbox.top - 1, hitbox.width + 2, hitbox.height + 2 };
		for (auto &iter : m_objectManager.getObjects<Player*>())
			if (iter->getHitbox().intersects(hitbox))
			{
				m_active = true;
				m_sprite.setTextureRect({ 16, m_sprite.getTextureRect().top, 16, 16 });
			}
		return;
	}

	

}

Emitter::Emitter(ObjectManager & objectManager, const int id, const sf::Vector2f & position) :
	Object(objectManager), m_direction(static_cast<Direction>(id))
{
	m_sprite.setTexture(DataManager::getInstance().getData("emitter").meta.texture);
	m_sprite.setTextureRect({ m_active ? 16 : 0,(id - Direction::Up) * 16,16,16 });
	m_sprite.setPosition(position);
}