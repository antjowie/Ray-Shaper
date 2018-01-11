#include "Text.h"
#include "DataManager.h"

void Text::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(m_text, states);
}

Text::Text(ObjectManager &objectManager, const sf::Vector2f & position, std::string & text, const unsigned int characterSize):
	Object(objectManager)
{
	m_text.setFont(DataManager::getInstance().getData("pixeled").meta.font);
	std::transform(text.begin(), text.end(), text.begin(), toupper);
	m_text.setString(text);
	m_text.setCharacterSize(characterSize);
	m_text.setOrigin(m_text.getGlobalBounds().width / 2.f, m_text.getGlobalBounds().height / 2.f);
	m_text.setPosition(position);
}