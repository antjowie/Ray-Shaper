#pragma once
#include "AnimationHandler.h"
#include "Object.h"
#include "Timeline.h"
#include "Tilemap.h"

#include <SFML\Graphics\RectangleShape.hpp>

class Player : public Object
{
private:
	// If this timeline is completed the player will enter idle state
	Timeline m_idleTimeline;

	// This enumerator is used to keep track of the animation sheet
	enum AnimationId
	{
		InSleep,
		Idle,
		fRight,
		sRight,
		fLeft,
		sLeft
	};

	// Used for direction vector
	enum Movement
	{
		Up,
		Left,
		Right
	};
	// Used to get direction in input function
	std::vector<bool>m_direction;

	AnimationHandler m_animHandler;
	sf::Sprite m_eyes;

	// The movement per frame
	sf::Vector2f m_movement{ 0,0 };
	// The at which speed increases per second
	float m_acceleration{ 4 };
	// The rate at which speed decreases per second
	float m_deceleration{ 8 };
	// The max acceleration
	float m_maxAcceleration{ 4 };
	// The amount of pixels to travel per accelartion
	float m_speed{ 16 };

	sf::RectangleShape m_hitbox;

	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override final;

public:
	// Applies the movement vector and updates animation spritesheet based on that
	void fixMovement(const std::vector<std::vector<Tile>> &tiles);
	
	sf::FloatRect getHitbox() const;
	sf::Vector2f getPosition() const;

	virtual void input(sf::RenderWindow &window) override final;
	virtual void update(const float elaspedTime) override final;

	Player(ObjectManager &objectManager,const sf::Vector2f &pos);
};