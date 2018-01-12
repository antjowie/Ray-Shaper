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
	
	// Amount of fall acceleration to increase per second
	float m_fallAcceleration{ 10 };
	// Max fall acceleration
	float m_maxFallAcceleration{ 10 };
	// The intial jump acceleration
	float m_initialJump{ 20 };

	// Can only jump if ground has been touched
	// Note: these values could be stored in a vector which allows 
	// a specified number of jumps
	bool m_canJumpSecond{ false };
	bool m_canJump{ false };
	// Used so that player has a time to hold the button
	// Also allows player to jump higer
	float m_jumpDuration{ 0.5f };
	float m_jumpTimeline{ 0.f };
	bool m_wantToJump{ false };

	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override final;

public:
	// Applies the movement vector and updates animation spritesheet based on that
	void fixMovement(std::vector<std::vector<Tile>> &tiles, const float elapsedTime);

	virtual void setPosition(const sf::Vector2f &pos) override final;
	virtual void move(const sf::Vector2f &movement) override final;

	virtual void input(sf::RenderWindow &window) override final;
	virtual void update(const float elaspedTime) override final;

	Player(ObjectManager &objectManager,const sf::Vector2f &pos);
};