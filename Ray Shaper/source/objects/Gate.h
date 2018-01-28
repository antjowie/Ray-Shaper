#pragma once
#include <SFML\Graphics\RectangleShape.hpp>

#include "Object.h"
#include "SoundManager.h"

class Gate : public Object
{
private:
	// Id is used to keep track of which level gate belongs to
	const int m_id;
	sf::RectangleShape m_upperSprite;
	sf::RectangleShape m_lowerSprite;
	
	// Timeline to mute other sounds
	SoundManager &m_soundManager;
	Timeline m_soundTimeline;
	Sound m_sound;
	Sound m_hitSound1;
	Sound m_hitSound2;
	// Makes sure sound is only played once
	bool m_isPlayed;

	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override final;
	
	// Used to check if gate is hit enough times
	const int m_maxHit;
	int m_hitCounter;
	// Checks if hit sound should be played
	int m_previousHitCounter{ 0 };

	// When horizontal width = height, so this keeps track for the math
	const bool m_isHorizontal{ false };

public:
	// Used when player is near
	bool isCollided{ false };
	// Used when laser hits
	void hit();

	virtual void update(const float elapsedTime) override final;

	virtual sf::FloatRect getHitbox() const override final;

	virtual std::map<std::string, std::string> getSaveData() const;
	// Returns bool whether gate has ever been hit or not
	struct Meta
	{
		int id;
		bool hasBeenHit;
	}; 
	Meta hasBeenHit() const;

	Gate(ObjectManager &objectManager, SoundManager &soundManager, const int id, const int hits, sf::FloatRect &hitbox, bool isOpened = false );
};