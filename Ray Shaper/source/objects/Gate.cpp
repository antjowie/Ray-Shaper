#include "Gate.h"
#include "DataManager.h"
#include "Config.h"

void Gate::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(m_upperSprite, states);
	target.draw(m_lowerSprite, states);
}

void Gate::hit()
{
	m_hitCounter--;
}

void Gate::update(const float elapsedTime)
{
	float height{ m_upperSprite.getGlobalBounds().height };

	// Checks whether gate should be unlocked
	if(m_isPlayed || (m_hitCounter <= 0 && !m_isPlayed))
		{
		// Only play sound once
			if (!m_isPlayed)
			{
				m_sound.play();
			}
			if (m_soundTimeline.getProgress() == 0)
			{
				m_soundManager.setTargetVolume(0, 0.3f, SoundType::Sound);
				m_soundManager.setTargetVolume(0, 0.3f, SoundType::Music);
				m_sound.setTargetVolume(Config::getInstance().getData("soundVolume").code, 0);
			}
			else if (m_soundTimeline.getProgress() > 30)
			{
				m_soundManager.setTargetVolume(Config::getInstance().getData("soundVolume").code, 5.f, SoundType::Sound);
				m_soundManager.setTargetVolume(Config::getInstance().getData("musicVolume").code, 5.f, SoundType::Music);
			}
			m_soundTimeline.update(elapsedTime);
			m_isPlayed = true;
		}

	// Open or closes gate
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

	// Check if gate should be solid, because we don't move hitbounds
	if (newSize.height < 5)
		isSolid = false;
	else
		isSolid = true;

	// Check if hit sound should be played
	if (m_hitCounter != m_maxHit && m_hitCounter < m_previousHitCounter)
		std::rand() % 2 == 0 ? m_hitSound1.play() : m_hitSound2.play();

	m_previousHitCounter = m_hitCounter;
	m_hitCounter = m_maxHit;
}

sf::FloatRect Gate::getHitbox() const
{
	return sf::FloatRect(m_upperSprite.getGlobalBounds().left, m_upperSprite.getGlobalBounds().top ,16.f,32.f);
}

std::map<std::string, std::string> Gate::getSaveData() const
{
	std::map<std::string, std::string> returner(Object::getSaveData());
	returner["state"] = m_isPlayed ? '1' : '0';
	returner["hit"] = std::to_string(m_maxHit);
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

Gate::Gate(ObjectManager & objectManager, SoundManager &soundManager, const int id, const int hits, sf::Vector2f & position, bool isOpened) :
	Object(objectManager), m_id(id), m_maxHit(hits == 0 ? 1 : hits), m_sound(soundManager, "sectionFinished", SoundType::Sound, { position.x + 8.f,position.y,0 }),
	m_soundManager(soundManager), m_isPlayed(isOpened),
	m_hitSound1(soundManager, "hit1", SoundType::Sound, { position.x + 8.f,position.y,0 }),
	m_hitSound2(soundManager, "hit2", SoundType::Sound, { position.x + 8.f,position.y,0 })
{
	// Used for postion saving
	m_sprite.setPosition(position);
	m_upperSprite.setTexture(&DataManager::getInstance().getData("gate").meta.texture);
	m_lowerSprite.setTexture(&DataManager::getInstance().getData("gate").meta.texture);

	// They can never get larger then 16
	//m_upperSprite.setTextureRect({ 0,0,16,16 });
	//m_lowerSprite.setTextureRect({ 0,0,16,16 });

	// Gate start closed
	// NOTE: By starting gates closed player can get stuck between doors
	// when reloading level so only start gates closed when not opened
	if (!isOpened)
	{
		m_upperSprite.setSize({16.f, 16.f});
		m_lowerSprite.setSize({16.f, 16.f});
	}

	// Do some rotation and move origin so that math can stay the same
	m_lowerSprite.setOrigin(0, 16);
	m_upperSprite.setPosition(position + sf::Vector2f{ 0,-16 });
	m_lowerSprite.setPosition(position + sf::Vector2f{ 0, 16 });
	m_lowerSprite.rotate(180);
	m_lowerSprite.move(16, -16);

	m_soundTimeline.setCap(5);
	m_sound.getSound().setAttenuation(0.05f);
	m_sound.getSound().setMinDistance(16.f);

	m_hitSound1.getSound().setAttenuation(0.05f);
	m_hitSound1.getSound().setMinDistance(16.f);

	m_hitSound2.getSound().setAttenuation(0.05f);
	m_hitSound2.getSound().setMinDistance(16.f);
}
