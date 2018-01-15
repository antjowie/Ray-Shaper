#include "Object.h"

void Object::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(m_sprite, states);
}

void Object::input(sf::RenderWindow & window)
{
}

void Object::update(const float elapsedTime)
{
}

sf::FloatRect Object::getHitbox() const
{
	return m_sprite.getGlobalBounds();
}

const sf::Vector2f &Object::getPosition() const
{
	return m_sprite.getPosition();
}

void Object::setPosition(const sf::Vector2f & pos)
{
	m_sprite.setPosition(pos);
}

void Object::move(const sf::Vector2f & movement)
{
	m_sprite.move(movement);
}

Object::Object(ObjectManager &objectManager):
	m_objectManager(objectManager)
{
	objectManager.push(this);
}