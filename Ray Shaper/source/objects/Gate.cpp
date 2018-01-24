#include "Gate.h"
#include "DataManager.h"
#include "Config.h"

#include <iostream>

void Gate::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(m_upperSprite, states);
	target.draw(m_lowerSprite, states);
}

void Gate::update(const float elapsedTime)
{
	float height{ m_upperSprite.getGlobalBounds().height };
	if(m_isPlayed || (laserHit && !m_isPlayed))
		{
		// Only play sound once
			if (!m_isPlayed)
			{
				m_sound.play();
			}
			if (m_soundTimeline.getProgress() == 0)
			{
				m_soundManager.setTargetVolume(0, 0.5f, SoundType::Sound);
				m_soundManager.setTargetVolume(0, 0.5f, SoundType::Music);
				m_sound.setTargetVolume(Config::getInstance().getData("soundVolume").code, 0);
			}
			else if (m_soundTimeline.getProgress() > 50)
			{
				m_soundManager.setTargetVolume(Config::getInstance().getData("soundVolume").code, 5.f, SoundType::Sound);
				m_soundManager.setTargetVolume(Config::getInstance().getData("musicVolume").code, 5.f, SoundType::Music);
			}
			m_soundTimeline.update(elapsedTime);
			m_isPlayed = true;
		}

	if (isCollided && m_isPlayed)
		height -= 16.f * elapsedTime;
	else
		height += 16.f * elapsedTime;
	
	// Bounds checking
	height = height > 16.f ? 16.f : height;
	height = height < 0.f ? 0.f : height;

	m_upperSprite.setSize({ 16.f,height });
	m_lowerSprite.setSize({ 16.f,height });

	sf::IntRect newSize{ m_isPlayed ? 16 : 0,16 - static_cast<int>(height),16, static_cast<int>(height) + 2 };
	m_upperSprite.setTextureRect(newSize);
	m_lowerSprite.setTextureRect(newSize);

	if (newSize.height < 5)
		isSolid = false;
	else
		isSolid = true;
}

sf::FloatRect Gate::getHitbox() const
{
	return sf::FloatRect(m_upperSprite.getGlobalBounds().left, m_upperSprite.getGlobalBounds().top ,16.f,32.f);
}

std::map<std::string, std::string> Gate::getSaveData() const
{
	std::map<std::string, std::string> returner(Object::getSaveData());
	returner["state"] = m_isPlayed ? '1':'0';
	returner["id"] = std::to_string(m_id);
	returner["y"] = std::to_string(getHitbox().top+getHitbox().height*0.5f);
	
	return returner;
}

Gate::Meta Gate::hasBeenHit() const
{
	Meta meta;
	meta.id = m_id;
	meta.hasBeenHit = m_isPlayed;
	return meta;
}

Gate::Gate(ObjectManager & objectManager, SoundManager &soundManager, const int id, sf::Vector2f & position, bool isOpened) :
	Object(objectManager), m_id(id), laserHit(false), m_sound(soundManager, "sectionFinished", SoundType::Sound, { position.x + 8.f,position.y,0 }),
	m_soundManager(soundManager), m_isPlayed(isOpened)
{
	// Used for postion saving
	m_sprite.setPosition(position);
	m_upperSprite.setTexture(&DataManager::getInstance().getData("gate").meta.texture);
	m_lowerSprite.setTexture(&DataManager::getInstance().getData("gate").meta.texture);

	// They can never get larger then 16
	//m_upperSprite.setTextureRect({ 0,0,16,16 });
	//m_lowerSprite.setTextureRect({ 0,0,16,16 });

	// Gate start closed
	m_upperSprite.setSize({16.f, 16.f});
	m_lowerSprite.setSize({16.f, 16.f});

	// Do some rotation and move origin so that math can stay the same
	m_lowerSprite.setOrigin(0, 16);
	m_upperSprite.setPosition(position + sf::Vector2f{ 0,-16 });
	m_lowerSprite.setPosition(position + sf::Vector2f{ 0, 16 });
	m_lowerSprite.rotate(180);
	m_lowerSprite.move(16, -16);

	m_soundTimeline.setCap(5);
	m_sound.getSound().setAttenuation(0.05f);
	m_sound.getSound().setMinDistance(16.f);
}
