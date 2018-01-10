#include "Camera.h"

#include <iostream>

const sf::View & Camera::getView() const
{
	return m_view;
}

void Camera::shake(const float duration, const float offset, const float intensity)
{
	m_shakeTimeline.setCap(duration);
	m_shakeTimeline.setTimeline(0);
	m_shakeIntensity.setCap(intensity);
	m_shakeIntensity.setTimeline(intensity);
	m_shakeOffset = offset;
	std::cout << m_shakeIntensity.getProgress() << '\n';
}

void Camera::setTargetPosition(const sf::Vector2f & targetPosition)
{
	m_targetPos = targetPosition;
}

void Camera::setCenter(const sf::Vector2f & center)
{
	m_view.setCenter(center);
	m_targetPos = center;
}

void Camera::update(const float elapsedTime)
{
	sf::Vector2f movement{ (m_targetPos - m_view.getCenter()) * m_speed};
	
	// If it's inbetween one pixel of target pos
	if (std::powf(movement.x, 2.f) + std::powf(movement.y, 2.f) < 0.1f)
		m_view.setCenter(m_targetPos);
	else
		m_view.move(movement * elapsedTime);
	
	// Shake camera if needed
	if (m_shakeIntensity.getProgress() == 100.f)
	{
		int old{ m_random };
		m_random = std::rand() % 4;
		if (old == m_random)
			++m_random %= 4;
		m_shakeIntensity.setTimeline(0);
	}
	switch (m_random)
	{
	case 0:
		m_shakeMovement.x = 1;
		m_shakeMovement.y = 0;
		break;
	case 1:
		m_shakeMovement.x = -1;
		m_shakeMovement.y = 1;
		break;
	case 2:
		m_shakeMovement.y = 0;
		m_shakeMovement.x = -1;
		break;
	case 3:
		m_shakeMovement.y = 0;
		m_shakeMovement.x = 1;
		break;
	}

	float progress{ 1.f - m_shakeTimeline.getProgress() /100.f};
	// Happens when getProgress == 0 (zero division)
	if (progress != progress)
		progress = 0;

	m_shakeMovement *= m_shakeOffset* progress;
	m_shakeTimeline.update(elapsedTime);
	m_shakeIntensity.update(elapsedTime);
	m_view.move(m_shakeMovement * elapsedTime);

	// This line fixes texture rendering
	m_view.setCenter(m_view.getCenter().x - std::fmod(m_view.getCenter().x, 0.1f), m_view.getCenter().y - std::fmod(m_view.getCenter().y, 0.1f));
}

void Camera::setSpeed(const float speed)
{
	m_speed = speed;
	if (m_speed < 0)
		m_speed = 0;
}

Camera::Camera(const float speed, const sf::FloatRect view):
	m_view(view)
{
	setSpeed(speed);
}
