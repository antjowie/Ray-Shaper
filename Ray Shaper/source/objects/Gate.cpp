#include "Gate.h"
#include "DataManager.h"

void Gate::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(m_upperSprite, states);
	target.draw(m_lowerSprite, states);
}

void Gate::update(const float elapsedTime)
{
	float height{ m_upperSprite.getGlobalBounds().height };
	if (isCollided)
		height -= 16.f * elapsedTime;
	else
		height += 16.f * elapsedTime;
	
	// Bounds checking
	height = height > 16.f ? 16.f : height;
	height = height < 0.f ? 0.f : height;

	m_upperSprite.setSize({ 16.f,height });
	m_lowerSprite.setSize({ 16.f,height });

	sf::IntRect newSize{ isCollided ? 16 : 0,16 - static_cast<int>(height),16, static_cast<int>(height) + 2 };
	m_upperSprite.setTextureRect(newSize);
	m_lowerSprite.setTextureRect(newSize);

	if (newSize.height < 5)
		isSolid = false;
	else
		isSolid = true;
}

sf::FloatRect Gate::getHitbox() const
{
	return sf::FloatRect(m_upperSprite.getGlobalBounds().left, m_upperSprite.getGlobalBounds().top ,16.f,32.f);
}

Gate::Gate(ObjectManager & objectManager, const int id, sf::Vector2f & position):
	Object(objectManager),m_id(id)
{
	m_upperSprite.setTexture(&DataManager::getInstance().getData("gate").meta.texture);
	m_lowerSprite.setTexture(&DataManager::getInstance().getData("gate").meta.texture);

	m_upperSprite.setTextureRect({ 0,0,16,16 });
	m_upperSprite.setTextureRect({ 0,0,16,16 });

	m_lowerSprite.setOrigin(0, 16);

	m_upperSprite.setPosition(position + sf::Vector2f{ 0,-16 });
	m_lowerSprite.setPosition(position + sf::Vector2f{ 0, 16 });
	m_lowerSprite.rotate(180);
	m_lowerSprite.move(16, -16);
}
