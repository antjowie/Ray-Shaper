#pragma once
#include <SFML\Graphics\RectangleShape.hpp>

#include "Object.h"
#include "SoundManager.h"

class Gate : public Object
{
private:
	const int m_id;
	sf::RectangleShape m_upperSprite;
	sf::RectangleShape m_lowerSprite;
	
	// Timeline to mute other sounds
	SoundManager &m_soundManager;
	Timeline m_soundTimeline;
	Sound m_sound;
	bool m_isPlayed;
	bool m_soundIsPlayed{ false };

	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override final;

public:
	// Used when player is near
	bool isCollided{ false };
	bool laserHit{ false };
	
	// Used when laser hits
	
	virtual void update(const float elapsedTime) override final;

	virtual sf::FloatRect getHitbox() const override final;

	Gate(ObjectManager &objectManager, SoundManager &soundManager, const int id, sf::Vector2f &position);
};