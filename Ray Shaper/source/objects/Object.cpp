#include "Object.h"



void Object::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	for (const auto &iter : m_sprites)
		target.draw(iter.second.sprite, states);
}

void Object::init()
{
	for (auto &iter : m_sprites)
	{
		iter.second.animHandler.setAnimation(0);
		iter.second.sprite.setOrigin(iter.second.animHandler.getFrame().width / 2.f, iter.second.animHandler.getFrame().height/ 2.f);
		iter.second.sprite.setTextureRect(iter.second.animHandler.getFrame());
	}
	m_objectManager.push(this);
}

Sprite & Object::getSprite(const std::string & name)
{
	return m_sprites.find(name)->second;
}

void Object::input(sf::RenderWindow & window)
{
}

void Object::update(const float elapsedTime)
{
}

sf::FloatRect Object::getHitbox() const
{
	return sf::FloatRect(m_sprites.begin()->second.sprite.getPosition().x - m_spriteWidth / 2.f, m_sprites.begin()->second.sprite.getPosition().y - m_spriteHeight/2.f, static_cast<float>(m_spriteWidth), static_cast<float>(m_spriteHeight));
}

const sf::Vector2f &Object::getPosition() const
{
	return m_sprites.begin()->second.sprite.getPosition();
}

void Object::setPosition(const sf::Vector2f & pos)
{
	for (auto &iter : m_sprites)
		iter.second.sprite.setPosition(pos);
}

Object::Object(const int spriteWidth, const int spriteHeight, ObjectManager &objectManager, const bool needPlayerPos):
	m_spriteWidth(spriteWidth),
	m_spriteHeight(spriteHeight),
	m_objectManager(objectManager),
	m_needPlayerPos(needPlayerPos)
{
}

Sprite::Sprite(const int frameWidth, const int frameHeight, sf::Texture &texture):
	animHandler(frameWidth,frameHeight)
{
	sprite.setTexture(texture);
}
