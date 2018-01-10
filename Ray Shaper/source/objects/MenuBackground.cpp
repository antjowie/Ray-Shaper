#include <SFML\Window\Mouse.hpp>

#include "MenuBackground.h"
#include "DataManager.h"

void MenuBackground::input(sf::RenderWindow & window)
{
	m_mousePos = sf::Vector2f(sf::Mouse::getPosition(window));

	m_mousePos.x = m_mousePos.x > window.getSize().x ? window.getSize().x : m_mousePos.x;
	m_mousePos.y = m_mousePos.y > window.getSize().y ? window.getSize().y : m_mousePos.y;

	m_mousePos.x = m_mousePos.x < 0 ? 0 : m_mousePos.x;
	m_mousePos.y = m_mousePos.y < 0 ? 0 : m_mousePos.y;
}

void MenuBackground::update(const float elapsedTime)
{
	const sf::Vector2f distance{ m_mousePos - m_center };
	m_targetPos = distance * 0.25f;

	const sf::Vector2f movement{ m_targetPos - m_sprite.getPosition() };
	move(movement * elapsedTime);
}

MenuBackground::MenuBackground(ObjectManager & objectManager):
	Object(objectManager)
{
	m_sprite.setTexture(DataManager::getInstance().getData("background").meta.texture);
	m_sprite.setTextureRect({ 0,0,1280 * 2,720 * 2 });
	m_sprite.setOrigin(m_sprite.getLocalBounds().width / 2.f, m_sprite.getLocalBounds().height / 2.f);
	m_center = m_sprite.getPosition();
}