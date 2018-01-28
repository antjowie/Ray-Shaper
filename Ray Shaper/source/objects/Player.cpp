#include <SFML\Window\Keyboard.hpp>
#include <SFML\Window\Mouse.hpp>
#include <SFML\Audio\Listener.hpp>

#include "Player.h"
#include "DataManager.h"
#include "MathHelper.h"

bool Player::isConfigPressed(Config::Data & data) const
{
	if(data.type == Config::Data::Type::Keyboard && sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Key>(data.code)))
		return true;
	if (data.type == Config::Data::Type::Mouse && sf::Mouse::isButtonPressed(static_cast<sf::Mouse::Button>(data.code)))
		return true;
	return false;
}

void Player::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(m_sprite, states);
	target.draw(m_eyes, states);
	target.draw(*m_reflector, states);
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

	if (isConfigPressed(Config::getInstance().getData("jump")))
		m_direction[Direction::Up] = true;
	if (isConfigPressed(Config::getInstance().getData("left")))
		m_direction[Direction::Left] = true;
	if (isConfigPressed(Config::getInstance().getData("right")))
		m_direction[Direction::Right] = true;

	m_isCrouching = (isConfigPressed(Config::getInstance().getData("crouch")));

	m_wantToGrab = false;
	if (m_grabCooldown.getProgress() == 100 && isConfigPressed(Config::getInstance().getData("grab")))
	{
		m_idleTimeline.setTimeline(0);
		m_grabCooldown.setTimeline(0);
		m_wantToGrab = true;
	}

	m_reflector->input(window);
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
	if (m_acceleration.y > m_maxFallAcceleration)
		m_acceleration.y = m_maxFallAcceleration;
	// This way crouching will not happen mid air
	if (m_acceleration.x > (m_isCrouching && m_acceleration.y == m_fallAcceleration * elapsedTime? m_maxAcceleration *0.25f: m_maxAcceleration))
		m_acceleration.x = (m_isCrouching && m_acceleration.y == m_fallAcceleration * elapsedTime? m_maxAcceleration * 0.25f : m_maxAcceleration);
	else if (m_acceleration.x < -(m_isCrouching && m_acceleration.y == m_fallAcceleration * elapsedTime ? m_maxAcceleration * 0.25f : m_maxAcceleration))
		m_acceleration.x = -(m_isCrouching && m_acceleration.y == m_fallAcceleration * elapsedTime ? m_maxAcceleration * 0.25f : m_maxAcceleration);
	
	// Fix movement to not collide
	oldMovement = movement = m_acceleration * m_speed * elapsedTime;
	m_objectManager.fixMovement<ReflectionTile*>(this, movement,true,true);
	
	// Check if player walked into wall and slow them down if that happens
	if ((oldMovement.x > 0 && movement.x <= 0) || (oldMovement.x < 0 && movement.x >= 0))
		m_acceleration.x = 0;

	// This results into a damped movement when crouching in wall, but because the player is very slow, it will be barely noticeable
	if (movement.y == 0)
	{
		// Else player falls with max acceleration when walking from edge
		m_acceleration.y = 0;
		if (m_isCrouching)
		{
		if (m_acceleration.x >m_maxAcceleration * 0.25f)
			m_acceleration.x = m_maxAcceleration * 0.25f;
		else if (m_acceleration.x < -m_maxAcceleration * 0.25f)
			m_acceleration.x = -m_maxAcceleration * 0.25f;
		}
	}

	// These can be handled with the new movement value
	if (m_direction[Direction::Right])
	{
		m_idleTimeline.setTimeline(0);
		m_animHandler.setAnimation(AnimationId::fRight);
		if (m_isCrouching && m_acceleration.x < m_maxAcceleration)
			m_animHandler.setAnimation(AnimationId::sRight);
	}
	else if (m_direction[Direction::Left])
	{
		m_idleTimeline.setTimeline(0);
		m_animHandler.setAnimation(AnimationId::fLeft);
		if (m_isCrouching && m_maxAcceleration > -m_maxAcceleration)
			m_animHandler.setAnimation(AnimationId::sLeft);
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

	// NOTE: This is now handled after movement value but not tested yet so this will stay for a while
	// Update accelerations based on collision snapping
	//if (movement.x != oldMovement.x);
	// This makes player move back to force him out of corners
	//m_acceleration.x = (oldMovement.x > 0 ? -m_decel - m_accel : m_accel + m_decel)* elapsedTime;
		
	// Player is on ground
	if (oldMovement.y > 0 && movement.y == 0)
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
		// Player should be jumping, jumping animation is prioritezed
		if (oldMovement.x < 0)
			m_animHandler.setAnimation(AnimationId::jLeft);
		else
			m_animHandler.setAnimation(AnimationId::jRight);
	}

	// Update animation speed
	if (m_animHandler.getAnimation() != AnimationId::Idle || m_animHandler.getAnimation() != AnimationId::sLeft || m_animHandler.getAnimation() !=  AnimationId::sRight)
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

	sf::Listener::setPosition({ getPosition().x,getPosition().y,0 });
	
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
					if (!iter->canMove || iter == m_reflector)
						continue;
					iter->isGrabbed = true;
					m_grabbed = iter;
					break;
				}
		}
	}
	// If tile makes illegal movement and wants to be dropped, drop it
	else if (m_grabbed && !m_grabbed->isGrabbed)
	{
		m_grabbed = nullptr;
	}
	if (m_grabbed)
		m_grabbed->move({ movement.x, movement.y });

	// Update reflector
	m_reflector->setPosition(getPosition());
}

std::map<std::string, std::string> Player::getSaveData() const
{
	std::map<std::string, std::string> returner(Object::getSaveData());
	returner["x"] = std::to_string(getPosition().x);
	returner["y"] = std::to_string(getPosition().y);
	return returner;
}

Player::Player(ObjectManager & objectManager, const sf::Vector2f & pos) :
	Object(objectManager), m_animHandler(16, 16, { 3,0,10,15 }),
	// Changing these values in the header file doesn't rebuild the porgram
	m_accel{ 4 },
	m_decel{ 8 },
	m_maxAcceleration{ 4 },
	m_speed{ 16 },
	m_fallAcceleration{ 30 },
	m_maxFallAcceleration{ 15 },
	m_initialJump{ 8 },
	m_jumpDuration{ 0.2f }
{
	m_reflector = new Reflector(objectManager);

	m_sprite.setTexture(DataManager::getInstance().getData("playerBody").meta.texture);
	m_eyes.setTexture(DataManager::getInstance().getData("playerEyes").meta.texture);
	
	// Stationary
	m_animHandler.pushAnimation(Animation(4, 0.75f, false, false));
	m_animHandler.pushAnimation(Animation(4, 0.75f, false, false));
	// Walk animation
	for (int i{ 0 }; i < 2; i++)
		m_animHandler.pushAnimation(Animation(4, 1 / 8.f, true, true));
	// Crouch animation
	for (int i{ 0 }; i < 2; i++)
		m_animHandler.pushAnimation(Animation(3, 0.05f, true, true));
	// Jump animation
	for (int i{ 0 }; i < 2; i++)
		m_animHandler.pushAnimation(Animation(1, 1, false, false));
	
	m_animHandler.setAnimation(0);
	m_animHandler.setAnimation(m_animHandler.getLastFrame());

	m_sprite.setTextureRect(m_animHandler.getFrame());
	m_eyes.setTextureRect(m_animHandler.getFrame());
	
	m_sprite.setOrigin(m_sprite.getLocalBounds().width/2.f, m_sprite.getLocalBounds().height/2.f);
	m_eyes.setOrigin(m_eyes.getLocalBounds().width/2.f, m_eyes.getLocalBounds().height/2.f);
	
	m_sprite.setPosition(pos);
	m_eyes.setPosition(pos);

	for (int i{ 0 }; i < 3; i++)
		m_direction.push_back(false);

	m_idleTimeline.setCap(5);
	m_grabCooldown.setCap(0.2f);
}

void Player::Reflector::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	if (m_shouldDraw)
		target.draw(m_sprite, states);
}

const std::vector<sf::Vector2f> Player::Reflector::getVertices() const
{
	std::vector<sf::Vector2f> returner;

	if (!m_shouldDraw)
		return returner;

	returner.push_back({ getPosition().x - 8.f, getPosition().y - 1.f});
	returner.push_back({ getPosition().x + 8.f, getPosition().y - 1.f});
	returner.push_back({ getPosition().x + 8.f, getPosition().y + 1.f });
	returner.push_back({ getPosition().x - 8.f, getPosition().y + 1.f });

	// Rotate points
	for (auto &iter : returner)
		iter = Math::rotateAroundOrigin(getPosition(), iter, m_sprite.getRotation());

	return returner;
}

void Player::Reflector::input(sf::RenderWindow & window)
{
	if (m_toggleCooldown.getProgress() == 100 && sf::Mouse::isButtonPressed(static_cast<sf::Mouse::Button>(Config::getInstance().getData("toggleReflector").code)))
	{
		m_toggleCooldown.setTimeline(0);
		m_shouldDraw = m_shouldDraw ? false : true;
	}

	m_mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
}

void Player::Reflector::update(const float elapsedTime)
{
	m_toggleCooldown.update(elapsedTime);
	m_sprite.setRotation(Math::getAngle(getPosition(), m_mousePos, getPosition() + sf::Vector2f{0, -1}));
}

Player::Reflector::Reflector(ObjectManager & objectManager):
	ReflectionTile(objectManager, 0, { 0,0 })
{
	isSolid = false;
	m_sprite.setTexture(DataManager::getInstance().getData("playerReflector").meta.texture);
	m_sprite.setTextureRect({ 0,0,16,16 });
	m_sprite.setOrigin(m_sprite.getLocalBounds().width / 2.f, m_sprite.getLocalBounds().height / 2.f);
	m_toggleCooldown.setCap(0.2f);
}