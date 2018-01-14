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
		m_direction[Direction::Up] = true;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		m_direction[Direction::Left] = true;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		m_direction[Direction::Right] = true;

	m_wantToGrab = false;
	if (m_grabCooldown.getProgress() == 100 && sf::Keyboard::isKeyPressed(sf::Keyboard::F))
	{
		m_grabCooldown.setTimeline(0);
		m_wantToGrab = true;
	}
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
	if (m_direction[Direction::Up] && m_canJumpSecond)
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

	if (m_direction[Direction::Left])
	{
		if (m_acceleration.x > 0)
			m_acceleration.x -= m_decel * elapsedTime;
		m_acceleration.x -= m_accel * elapsedTime;
		moveHoriz = true;
	}

	if (m_direction[Direction::Right])
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
	sf::Vector2f tileMovement{ oldMovement };
	m_objectManager.fixMovement<ReflectionTile*>(this, movement,true,true);

	// These can be handled with the new movement value
	if (m_direction[Direction::Right])
	{
		m_idleTimeline.setTimeline(0);
		m_animHandler.setAnimation(AnimationId::fRight);
	}
	else if (m_direction[Direction::Left])
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
		else
		{
			m_animHandler.setRate(1);
		}
		if (m_direction[Direction::Up])
			m_idleTimeline.setTimeline(0);
	}

	// Update accelerations based on collision snapping
	if (movement.x != oldMovement.x)
		// This makes player move back to force him out of corners
		m_acceleration.x = (oldMovement.x > 0 ? -m_decel - m_accel : m_accel + m_decel)* elapsedTime;
	// Player is on ground
	if (oldMovement.y != 0 && movement.y == 0)
	{
		m_canJumpSecond = true;
		m_canJump = true;
	}
	// Head touched top
	if (oldMovement.y < 0 && movement.y >= 0)
	{
		m_canJump = false;
		m_acceleration.y = 0;
	}
	// Player is falling
	if (movement.y != 0)
	{
		m_canJump = false;
	}

	// Update animation speed
	if (m_animHandler.getAnimation() != AnimationId::Idle)
	{
		float m_rate{ std::fabs(m_acceleration.x) / m_maxAcceleration };
		if (m_rate != 0 && m_rate < 0.25f)
			m_rate = 0.25f;
		m_animHandler.setRate(m_rate);
	}

	m_sprite.setTextureRect(m_animHandler.getFrame());
	m_eyes.setTextureRect(m_animHandler.getFrame());

	m_sprite.move(movement);
	m_eyes.move(movement);

	// Manage block holding mechanics
	m_grabCooldown.update(elapsedTime);
	if (m_wantToGrab)
	{
		// If player already grabbed, let go of it
		if (m_grabbed)
		{
			m_grabbed->isGrabbed = false;
			m_grabbed = nullptr;
		}
		else
		{
			for(auto &iter:m_objectManager.getObjects<ReflectionTile*>())
				if (iter->getHitbox().intersects(getHitbox()))
				{
					iter->isGrabbed = true;
					m_grabbed = iter;
					break;
				}
		}
	}
	// If player jumps, let go of the object
	else if (m_direction[Direction::Up])
	{
		if (m_grabbed)
		{
			m_grabbed->isGrabbed = false;
			m_grabbed = nullptr;
		}
	}
	// If tile makes illegal movement and wants to be dropped, drop it
	else if (m_grabbed && !m_grabbed->isGrabbed)
	{
		m_grabbed = nullptr;
	}
	if (m_grabbed)
		m_grabbed->move({ movement.x, 0 });
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
	m_grabCooldown.setCap(0.2f);
}
