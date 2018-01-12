#include <SFML\Window\Keyboard.hpp>

#include "Player.h"
#include "DataManager.h"

void Player::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(m_hitbox, states);
	target.draw(m_sprite, states);
	target.draw(m_eyes, states);
}

void Player::fixMovement(std::vector<std::vector<Tile>>& tiles, const float elapsedTime)
{
	sf::FloatRect hitbox { getHitbox() };
	
	if (m_movement.x > 0)
	{
		if (m_movement.y < 0)
			hitbox = { m_sprite.getGlobalBounds().left,m_sprite.getGlobalBounds().top + m_movement.y, m_sprite.getGlobalBounds().width + m_movement.x,m_sprite.getGlobalBounds().height - m_movement.y };
		else
			hitbox = { m_sprite.getGlobalBounds().left,m_sprite.getGlobalBounds().top,m_sprite.getGlobalBounds().width + m_movement.x,m_sprite.getGlobalBounds().height + m_movement.y };
	}
	else if (m_movement.x < 0)
	{
		if (m_movement.y < 0)
			hitbox = { m_sprite.getGlobalBounds().left + m_movement.x, m_sprite.getGlobalBounds().top + m_movement.y, m_sprite.getGlobalBounds().width - m_movement.x,m_sprite.getGlobalBounds().height - m_movement.y };
		else
			hitbox = { m_sprite.getGlobalBounds().left + m_movement.x, m_sprite.getGlobalBounds().top,m_sprite.getGlobalBounds().width - m_movement.x,m_sprite.getGlobalBounds().height + m_movement.y };
	}
	if (m_movement.y > 0)
	{
		if (m_movement.x > 0)
			hitbox = { m_sprite.getGlobalBounds().left, m_sprite.getGlobalBounds().top,m_sprite.getGlobalBounds().width + m_movement.x, m_sprite.getGlobalBounds().height + m_movement.y };
		else
			hitbox = { m_sprite.getGlobalBounds().left + m_movement.x, m_sprite.getGlobalBounds().top, m_sprite.getGlobalBounds().width - m_movement.x, m_sprite.getGlobalBounds().height + m_movement.y };
	}
	else if (m_movement.y < 0)
	{
		if (m_movement.x > 0)
			hitbox = { m_sprite.getGlobalBounds().left, m_sprite.getGlobalBounds().top + m_movement.y,m_sprite.getGlobalBounds().width + m_movement.x, m_sprite.getGlobalBounds().height - m_movement.y };
		else
			hitbox = { m_sprite.getGlobalBounds().left + m_movement.x, m_sprite.getGlobalBounds().top + m_movement.y, m_sprite.getGlobalBounds().width - m_movement.x, m_sprite.getGlobalBounds().height - m_movement.y };
	}
	// TODO FIX THIS

	for (auto &i: tiles)
		for(auto &j:i)
		{
			const sf::FloatRect &b{ j.getHitbox() };

			if (hitbox.intersects(b))
			{
				if (sf::FloatRect(hitbox.left, getHitbox().top, hitbox.width, getHitbox().height).intersects(b))
				{

					if (m_movement.x > 0)
						m_movement.x = b.left - (getHitbox().left + getHitbox().width);
					else if (m_movement.x < 0)
						m_movement.x = (b.left + b.width) - getHitbox().left;
				}
				if(sf::FloatRect(getHitbox().left,hitbox.top,getHitbox().width,hitbox.height).intersects(b))
				{
					if (m_movement.y > 0)
						m_movement.y = b.top - (getHitbox().top + getHitbox().height);
					else if (m_movement.y < 0)
						m_movement.y = b.top + b.height - getHitbox().top;
				}
				j.setState(true);
			}
			else 
				j.setState(false);
		}			

	m_hitbox.setPosition(hitbox.left,hitbox.top);
	m_hitbox.setSize({ hitbox.width,hitbox.height });

	m_sprite.move(m_movement * m_speed * elapsedTime);
	m_eyes.move  (m_movement * m_speed * elapsedTime);
}

void Player::setPosition(const sf::Vector2f & pos)
{
	m_sprite.setPosition(pos);
	m_eyes.setPosition(pos);
}

void Player::move(const sf::Vector2f & movement)
{
	m_sprite.move(movement);
	m_eyes.move(movement);
}

void Player::input(sf::RenderWindow & window)
{
	for (auto &iter : m_direction)
		iter = false;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		m_direction[Movement::Up] = true;	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		m_direction[Movement::Down] = true;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		m_direction[Movement::Left] = true;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		m_direction[Movement::Right] = true;
}

void Player::update(const float elapsedTime)
{
	m_animHandler.update(elapsedTime);

	// Used to check if axises has to be decelerated
	bool moveVertic{ false }, moveHoriz{ false };

	// Update movement
	if (m_direction[Movement::Up])
	{
		if (m_movement.y > 0)
			m_movement.y -= m_deceleration * elapsedTime;
		m_movement.y -= m_acceleration * elapsedTime;
		moveVertic = true;
	}
	if (m_direction[Movement::Down])
	{
		if (m_movement.y < 0)
			m_movement.y += m_deceleration * elapsedTime;
		m_movement.y += m_acceleration * elapsedTime;
		moveVertic = true;
	}
	if (m_direction[Movement::Left])
	{
		if (m_movement.x > 0)
			m_movement.x -= m_deceleration * elapsedTime;
		m_movement.x -= m_acceleration * elapsedTime;
		moveHoriz = true;
	}

	if (m_direction[Movement::Right])
	{
		if (m_movement.x < 0)
			m_movement.x += m_deceleration * elapsedTime;
		m_movement.x += m_acceleration * elapsedTime;
		moveHoriz = true;
	}

	// Update deceleration values
	if (!moveHoriz)
	{
		if (m_movement.x > 0)
		{
			m_movement.x -= m_deceleration * elapsedTime;
			if (m_movement.x < 0)
				m_movement.x = 0;
		}
		else if (m_movement.x < 0)
		{
			m_movement.x += m_deceleration * elapsedTime;
			if (m_movement.x > 0)
				m_movement.x = 0;
		}
	}
	if (!moveVertic)
	{
		if (m_movement.y > 0)
		{
			m_movement.y -= m_deceleration * elapsedTime;
			if (m_movement.y < 0)
				m_movement.y = 0;
		}
		else if (m_movement.y < 0)
		{
			m_movement.y += m_deceleration * elapsedTime;
			if (m_movement.y > 0)
				m_movement.y = 0;
		}
	}

	// Check movement bounds
	if (m_movement.x > m_maxAcceleration)
		m_movement.x = m_maxAcceleration;
	else if (m_movement.x < -m_maxAcceleration)
		m_movement.x = -m_maxAcceleration;
	if (m_movement.y > m_maxAcceleration)
		m_movement.y = m_maxAcceleration;
	else if (m_movement.y < -m_maxAcceleration)
		m_movement.y = -m_maxAcceleration;

	if (m_movement.x > 0)
		m_animHandler.setAnimation(AnimationId::fRight);
	else if (m_movement.x < 0)
		m_animHandler.setAnimation(AnimationId::fLeft);
	else
		m_animHandler.setAnimation(AnimationId::Idle);

	// Update animation speed
	float m_rate{ std::fabs(m_movement.x) / m_maxAcceleration };
	if (m_rate != 0 && m_rate < 0.25f)
		m_rate = 0.25f;
	m_animHandler.setRate(m_rate);

	m_hitbox.setPosition(getHitbox().left, getHitbox().top);
	m_hitbox.setSize({ getHitbox().width, getHitbox().height });

	m_sprite.setTextureRect(m_animHandler.getFrame());
	m_eyes.setTextureRect(m_animHandler.getFrame());
}

Player::Player(ObjectManager & objectManager, const sf::Vector2f & pos):
	Object(objectManager), m_animHandler(16, 16, { 3,0,10,15 })
{
	m_sprite.setTexture(DataManager::getInstance().getData("playerBody").meta.texture);
	m_eyes.setTexture(DataManager::getInstance().getData("playerEyes").meta.texture);
	
	m_animHandler.pushAnimation(Animation(4, 0.75f, false, false));
	m_animHandler.pushAnimation(Animation(4, 0.75f, false, false));
	for (int i{ 0 }; i < 4; i++)
		m_animHandler.pushAnimation(Animation(4, 1 / 8.f, true, true));

	m_animHandler.setAnimation(0);
	m_sprite.setOrigin(8, 8);
	m_eyes.setOrigin(8, 8);

	m_sprite.setTextureRect(m_animHandler.getFrame());
	m_eyes.setTextureRect(m_animHandler.getFrame());
	
	m_sprite.setPosition(pos);
	m_eyes.setPosition(pos);

	for (int i{ 0 }; i < 4; i++)
		m_direction.push_back(false);

	m_hitbox.setFillColor({ 255,255,255,150 });
	m_hitbox.setOutlineThickness(1);
	m_hitbox.setOutlineColor(sf::Color::Red);
}
