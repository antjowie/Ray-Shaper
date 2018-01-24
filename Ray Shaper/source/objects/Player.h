#pragma once
#include "AnimationHandler.h"
#include "Object.h"
#include "Timeline.h"
#include "Tilemap.h"

#include "objects\ReflectionTile.h"

class Player : public Object
{
public:
	// This class represents the reflector
	class Reflector : public ReflectionTile
	{
	private:
		sf::Vector2f m_mousePos;
		Timeline m_toggleCooldown;
		bool m_shouldDraw{ false };

		virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override final;

	public:
		virtual const std::vector<sf::Vector2f> getVertices() const override final;

		virtual void input(sf::RenderWindow &window) override final;
		virtual void update(const float elapsedTime) override final;

		Reflector(ObjectManager &objectManager);
	};
private:
	Reflector* m_reflector;

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
	enum Direction
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
	sf::Vector2f m_acceleration{ 0,0 };
	// The at which speed increases per second
	float m_accel;
	// The rate at which speed decreases per second
	float m_decel;
	// The max acceleration
	float m_maxAcceleration;
	// The amount of pixels to travel per accelartion
	float m_speed;
	
	// Amount of fall acceleration to increase per second
	float m_fallAcceleration;
	// Max fall acceleration
	float m_maxFallAcceleration;
	// The intial jump acceleration
	float m_initialJump;

	// Can only jump if ground has been touched
	// Note: these values could be stored in a vector which allows 
	// a specified number of jumps
	bool m_canJumpSecond{ false };
	bool m_canJump{ false };
	// Used so that player has a time to hold the button
	// Also allows player to jump higer
	float m_jumpDuration;
	float m_jumpTimeline{ 0.f };
	bool m_wantToJump{ false };

	// Block movement logic
	Timeline m_grabCooldown;
	bool m_wantToGrab{ false };
	ReflectionTile * m_grabbed{ nullptr };

	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override final;

public:
	virtual void setPosition(const sf::Vector2f &pos) override final;
	virtual void move(const sf::Vector2f &movement) override final;

	virtual void input(sf::RenderWindow &window) override final;
	virtual void update(const float elaspedTime) override final;

	Player(ObjectManager &objectManager,const sf::Vector2f &pos);
};