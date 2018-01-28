#include "Text.h"
#include "DataManager.h"

void Text::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(m_text, states);
}

Text::Text(ObjectManager &objectManager, const sf::Vector2f & position, std::string & text, const unsigned int characterSize):
	Object(objectManager)
{
	isSolid = false;
	m_text.setFont(DataManager::getInstance().getData("pixeled").meta.font);
	std::transform(text.begin(), text.end(), text.begin(), toupper);
	m_text.setString(text);
	m_text.setCharacterSize(characterSize);
	m_text.setOrigin(m_text.getGlobalBounds().width / 2.f, m_text.getGlobalBounds().height / 2.f);
	m_text.setPosition(position);

	// For hitbox
	m_sprite.setPosition(position);
	m_sprite.setTextureRect({ 0,0,static_cast<int>(m_text.getGlobalBounds().width),static_cast<int>(m_text.getGlobalBounds().height) });
}

void Text::setColor(const sf::Color & color)
{
	m_text.setFillColor(color);
}

void Text::setString(const std::string & string)
{
	m_text.setString(string);
	m_sprite.setPosition({ m_text.getGlobalBounds().left,m_text.getGlobalBounds().top });
	m_sprite.setTextureRect({ 0,0,static_cast<int>(m_text.getGlobalBounds().width),static_cast<int>(m_text.getGlobalBounds().height) });
}

void Text::setOrigin(const sf::Vector2f & origin)
{
	m_text.setOrigin(origin);
}
