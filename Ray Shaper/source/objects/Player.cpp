#include <SFML\Window\Keyboard.hpp>

#include "Player.h"
#include "DataManager.h"

#include <iostream>

void Player::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(m_sprite, states);
	target.draw(m_eyes, states);
}

void Player::fixMovement(std::vector<std::vector<Tile>>& tiles, const float elapsedTime)
{
	// Potential errors
	// This function slows the hitbox down rather then snapping it into place

	// Note:
	// If I have enough time to implement object movement this function will be put into 
	// the objectManager. Then, every object can check if it's movements are not illegal.
	// I'm not sure if I have enough time to finish it all, but if I do, some variables 
	// should be changed and the whole section that manages animation and jump state.
	sf::FloatRect hitbox { getHitbox() };
	sf::Vector2f oldMovement{ m_movement };

	// This section updates the hitbox, the hitbox is a rectangle which contains the whole movement
	// This rectangle is used to check nearby tiles whether or not they will collide with the hitbox
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

	// Iterate though all tiles and check whether or not they collided
	for (auto &i: tiles)
		for(auto &j:i)
		{
			const sf::FloatRect &b{ j.getHitbox() };

			// We first check the horizontal movement of the hitbox and see if it collided or not
			// By doing this, the snapping will only happen when the appropiate tiles are touched.
			// If we do not do this. Tiles on the bottom of the hitbox may be confused as tiles on the horizontal
			// movement. That will give incorrect snapping of position
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
	
	//if (static_cast<int>(oldMovement.y) >= 0 && static_cast<int>(m_movement.y) <= 0 && m_potentialJump)
	//	m_canJump = true;
	//else
	//	m_potentialJump = false;

	m_sprite.move(m_movement * m_speed * elapsedTime);
	m_eyes.move(m_movement * m_speed * elapsedTime);

	// The snapping back of movement is not always 0, so we round it down
	if (static_cast<int>(m_movement.y) <= 0 && static_cast<int>(oldMovement.y) > 0.f)
	{
		m_canJumpSecond = true;
		m_canJump = true;
	}
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

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		m_direction[Movement::Up] = true;	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		m_direction[Movement::Left] = true;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		m_direction[Movement::Right] = true;
}

void Player::update(const float elapsedTime)
{
	m_animHandler.update(elapsedTime);
	m_jumpTimeline -= elapsedTime;
	if (m_jumpTimeline < 0)
		m_jumpTimeline = 0;

	// Gravity 
	m_movement.y += m_fallAcceleration * elapsedTime;

	// Used to check if horizontal axis has to be decelerated
	bool moveHoriz{ false };

	// Update movement
	if (m_direction[Movement::Up] && m_canJumpSecond)
	{
		// We first update jump conditions
		// Allows double jump
		if (!m_canJump && m_canJumpSecond && !m_wantToJump)
		{
			m_jumpTimeline = m_jumpDuration;
			m_canJumpSecond = false;
		}

		if (m_canJump && !m_wantToJump)
		{
			m_jumpTimeline = m_jumpDuration;
			m_canJump = false;
		}

		// Update jump
		if (m_jumpTimeline != 0)
			m_movement.y = -m_initialJump;
		m_wantToJump = true;
	}
	else
		m_wantToJump = false;
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

	// Check movement bounds
	if (m_movement.y > m_maxAcceleration)
		m_movement.y = m_maxFallAcceleration;
	if (m_movement.x > m_maxAcceleration)
		m_movement.x = m_maxAcceleration;
	else if (m_movement.x < -m_maxAcceleration)
		m_movement.x = -m_maxAcceleration;
	
	// These can be handles with the new movement value,
	// but I like the slow walk into the wall
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

	for (int i{ 0 }; i < 3; i++)
		m_direction.push_back(false);
}
