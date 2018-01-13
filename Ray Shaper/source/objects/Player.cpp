#include <SFML\Window\Keyboard.hpp>

#include "Player.h"
#include "DataManager.h"

#include <iostream>

void Player::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(m_sprite, states);
	target.draw(m_eyes, states);
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
	sf::Vector2f movement{ 0,0 };
	sf::Vector2f oldMovement{ 0,0 };
	
	m_animHandler.update(elapsedTime);
	m_jumpTimeline -= elapsedTime;
	if (m_jumpTimeline < 0)
		m_jumpTimeline = 0;

	// Gravity 
	m_acceleration.y += m_fallAcceleration * elapsedTime;

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
			m_acceleration.y = -m_initialJump;
		m_wantToJump = true;
	}
	else
		m_wantToJump = false;
	
	if (m_direction[Movement::Left])
	{
		if (m_acceleration.x > 0)
			m_acceleration.x -= m_decel * elapsedTime;
		m_acceleration.x -= m_accel * elapsedTime;
		moveHoriz = true;
	}

	if (m_direction[Movement::Right])
	{
		if (m_acceleration.x < 0)
			m_acceleration.x += m_decel * elapsedTime;
		m_acceleration.x += m_accel * elapsedTime;
		moveHoriz = true;
	}

	// Update deceleration values
	if (!moveHoriz)
	{
		if (m_acceleration.x > 0)
		{
			m_acceleration.x -= m_decel * elapsedTime;
			if (m_acceleration.x < 0)
				m_acceleration.x = 0;
		}
		else if (m_acceleration.x < 0)
		{
			m_acceleration.x += m_decel * elapsedTime;
			if (m_acceleration.x > 0)
				m_acceleration.x = 0;
		}
	}

	// Check movement bounds
	if (m_acceleration.y > m_maxAcceleration)
		m_acceleration.y = m_maxFallAcceleration;
	if (m_acceleration.x > m_maxAcceleration)
		m_acceleration.x = m_maxAcceleration;
	else if (m_acceleration.x < -m_maxAcceleration)
		m_acceleration.x = -m_maxAcceleration;
	
	// Fix movement to not collide
	oldMovement = movement = m_acceleration * m_speed * elapsedTime;
	m_objectManager.fixMovement(this, movement);
	
	// Update accelerations based on collision snapping
	if (oldMovement.x != 0 && movement.x == 0)
		m_acceleration.x = 0;

	// These can be handled with the new movement value
	if (movement.x > 0)
	{
		m_idleTimeline.setTimeline(0);
		m_animHandler.setAnimation(AnimationId::fRight);
	}
	else if (movement.x < 0)
	{
		m_idleTimeline.setTimeline(0);
		m_animHandler.setAnimation(AnimationId::fLeft);
	}
	else
	{
		m_animHandler.setAnimation(AnimationId::Idle);
			m_idleTimeline.update(elapsedTime);
			if (m_idleTimeline.getProgress() != 100)
			{
				m_animHandler.setTimeline(0);
				m_animHandler.setFrame(0);
			}
			if (m_direction[Movement::Up])
				m_idleTimeline.setTimeline(0);
	}

	// Update animation speed
	if (m_animHandler.getAnimation() != AnimationId::Idle)
	{
		float m_rate{ std::fabs(movement.x) / m_maxAcceleration };
		if (m_rate != 0 && m_rate < 0.25f)
			m_rate = 0.25f;
		m_animHandler.setRate(m_rate);
	}

	m_sprite.setTextureRect(m_animHandler.getFrame());
	m_eyes.setTextureRect(m_animHandler.getFrame());

	// Player touches ground
	if (oldMovement.y != 0 && movement.y == 0)
	{
		m_canJumpSecond = true;
		m_canJump = true;
	}

	m_sprite.move(movement);
	m_eyes.move(movement);
}

Player::Player(ObjectManager & objectManager, const sf::Vector2f & pos):
	Object(objectManager), m_animHandler(16, 16, { 3,0,10,15 }),
	// Changing these values in the header file doesn't rebuild the porgram
	m_accel{ 4 },
	m_decel{ 8 },
	m_maxAcceleration{ 4 },
	m_speed{ 16 },
	m_fallAcceleration{ 20 },
	m_maxFallAcceleration{ 10 },
	m_initialJump{ 6 },
	m_jumpDuration{ 0.3f }
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

	m_idleTimeline.setCap(5);
}
