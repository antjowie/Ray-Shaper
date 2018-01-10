//////////////////////////////////
//
// The Camera class
//  
// The camera class handles an instance of a view
// This class contains special functions  to give the 
// view a smooth movement and certain other functions
//
//////////////////////////////////

#pragma once
#include <SFML\Graphics\View.hpp>
#include "Timeline.h"

class Camera
{
private:
	sf::View m_view;
	sf::Vector2f m_targetPos;
	float m_speed;
	
	// Shake function member variables
	Timeline m_shakeTimeline;
	// This timeline decides how fast the shake changes direction
	Timeline m_shakeIntensity;
	float m_shakeOffset{ 0 };
	int m_random{ 0 };
	sf::Vector2f m_shakeMovement{ 0,0 };

public:
	void update(const float elapsedTime);
	// The time value is the duration off the shake effect in seconds
	// The offset value is the ammount of pixels the camera shakes from the center
	// The intensity is a time value in seconds which decides how fast the shake motions should change direction
	void shake(const float duration, const float offset, const float intensity);
	
	void setTargetPosition(const sf::Vector2f &targetPosition);
	void setCenter(const sf::Vector2f &center);
	// The speed value affects how long it will take to get to target position
	void setSpeed(const float speed);

	const sf::View &getView() const;

	Camera(const float speed = 1, const sf::FloatRect view = { 0,0,0,0 });
};