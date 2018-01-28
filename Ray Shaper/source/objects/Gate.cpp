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
	float height{ m_isHorizontal ? m_upperSprite.getGlobalBounds().width : m_upperSprite.getGlobalBounds().height };

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
		height -= (m_isHorizontal ? m_sprite.getGlobalBounds().width : m_sprite.getGlobalBounds().height) * elapsedTime;
	else
		height += (m_isHorizontal ? m_sprite.getGlobalBounds().width : m_sprite.getGlobalBounds().height) * elapsedTime;
	
	// Bounds checking
	if (!m_isHorizontal)
		height = height > m_sprite.getGlobalBounds().height * .5f? m_sprite.getGlobalBounds().height*.5f : height;
	else
		height = height > m_sprite.getGlobalBounds().width * .5f ? m_sprite.getGlobalBounds().width*.5f : height;
	height = height < 0.f ? 0.f : height;

	if (!m_isHorizontal)
	{
		m_upperSprite.setSize({ m_sprite.getGlobalBounds().width ,height });
		m_lowerSprite.setSize({ m_sprite.getGlobalBounds().width ,height });
	}
	else
	{
		m_upperSprite.setSize({ m_sprite.getGlobalBounds().height ,height });
		m_lowerSprite.setSize({ m_sprite.getGlobalBounds().height ,height });
	}

	// Makes gate not seem warped
	sf::IntRect newSize{ m_isPlayed ? 16 : 0,
		static_cast<int>((((m_isHorizontal ? m_sprite.getGlobalBounds().width : m_sprite.getGlobalBounds().height) * .5f - height)) /	// This section converts the global
		((m_isHorizontal ? m_sprite.getGlobalBounds().width : m_sprite.getGlobalBounds().height) * .5f) * 16.f),						// sizes to local sizes
		16,static_cast<int>(height / ((m_isHorizontal ? m_sprite.getGlobalBounds().width : m_sprite.getGlobalBounds().height) * .5f) * 16.f)};
	m_upperSprite.setTextureRect(newSize);
	m_lowerSprite.setTextureRect(newSize);
	
	// Check if gate should be solid, because we don't move hitbounds
	if (newSize.height < (m_isHorizontal ? m_sprite.getGlobalBounds().width : m_sprite.getGlobalBounds().height) * 0.125f)
		isSolid = false;
	else
		isSolid = true;

	// Check if hit sound should be played
	if (m_hitCounter != m_maxHit && m_hitCounter != 0 && m_hitCounter < m_previousHitCounter)
		std::rand() % 2 == 0 ? m_hitSound1.play() : m_hitSound2.play();

	m_previousHitCounter = m_hitCounter;
	m_hitCounter = m_maxHit;
}

sf::FloatRect Gate::getHitbox() const
{
	return m_sprite.getGlobalBounds();
}

std::map<std::string, std::string> Gate::getSaveData() const
{
	std::map<std::string, std::string> returner(Object::getSaveData());
	returner["state"] = m_isPlayed ? '1' : '0';
	returner["hit"] = std::to_string(m_maxHit);
	returner["id"] = std::to_string(m_id);
	
	returner["x"] = std::to_string		(m_sprite.getGlobalBounds().left);
	returner["y"] = std::to_string		(m_sprite.getGlobalBounds().top);
	returner["width"] = std::to_string	(m_sprite.getGlobalBounds().width);
	returner["height"] = std::to_string	(m_sprite.getGlobalBounds().height);

	return returner;
}

Gate::Meta Gate::hasBeenHit() const
{
	Meta meta;
	meta.id = m_id;
	meta.hasBeenHit = m_isPlayed;
	return meta;
}

Gate::Gate(ObjectManager & objectManager, SoundManager &soundManager, const int id, const int hits, sf::FloatRect &hitbox, bool isOpened) :
	Object(objectManager), m_id(id), m_maxHit(hits == 0 ? 1 : hits), m_sound(soundManager, "sectionFinished", SoundType::Sound, { hitbox.left + hitbox.width * 0.5f ,hitbox.top + hitbox.height * 0.5f,0 }),
	m_soundManager(soundManager), m_isPlayed(isOpened),m_hitCounter(m_maxHit),m_previousHitCounter(m_hitCounter),
	m_hitSound1(soundManager, "hit1", SoundType::Sound,  { hitbox.left + hitbox.width * 0.5f ,hitbox.top + hitbox.height * 0.5f,0 }),
	m_hitSound2(soundManager, "hit2", SoundType::Sound,  { hitbox.left + hitbox.width * 0.5f ,hitbox.top + hitbox.height * 0.5f,0 }),
	m_isHorizontal(hitbox.width > hitbox.height)
{
	// Used for hitbox
	m_sprite.setPosition({ hitbox.left,hitbox.top });
	m_sprite.setTextureRect(static_cast<sf::IntRect>(hitbox));

	m_upperSprite.setTexture(&DataManager::getInstance().getData("gate").meta.texture);
	m_lowerSprite.setTexture(&DataManager::getInstance().getData("gate").meta.texture);
	
	// Gate start closed
	// NOTE: By starting gates closed player can get stuck between doors
	// when reloading level so only start gates closed when not opened
	if (!m_isHorizontal)
	{
		m_upperSprite.setSize({ hitbox.width,hitbox.height*(isOpened ? 0.f:.5f) });
		m_lowerSprite.setSize({ hitbox.width,hitbox.height*(isOpened ? 0.f:.5f) });
	}													
	else												
	{													
		m_upperSprite.setSize({ hitbox.height,hitbox.width*(isOpened ? 0.f:.5f) });
		m_lowerSprite.setSize({ hitbox.height,hitbox.width*(isOpened ? 0.f:.5f) });
	}

	m_upperSprite.setTextureRect({ 0,0,16,16 });
	m_lowerSprite.setTextureRect({ 0,0,16,16 });

	m_upperSprite.setPosition({ hitbox.left,hitbox.top });
	m_lowerSprite.setPosition({ hitbox.left,hitbox.top });

	// If gate is vertical, meaning it will open up and down
	if (hitbox.height > hitbox.width)
	{
		m_lowerSprite.rotate(180);
		m_lowerSprite.move(hitbox.width, hitbox.height);
	}
	else
	{
		m_upperSprite.rotate(-90);
		m_upperSprite.move(0, hitbox.height);
		m_lowerSprite.rotate(90);
		m_lowerSprite.move(hitbox.width, 0);
	}

	m_soundTimeline.setCap(5);
	m_sound.getSound().setAttenuation(0.05f);
	m_sound.getSound().setMinDistance(16.f);

	m_hitSound1.getSound().setAttenuation(0.05f);
	m_hitSound1.getSound().setMinDistance(16.f);

	m_hitSound2.getSound().setAttenuation(0.05f);
	m_hitSound2.getSound().setMinDistance(16.f);
}
