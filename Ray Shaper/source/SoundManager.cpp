#include "SoundManager.h"
#include "DataManager.h"

void SoundManager::push(Sound & sound)
{
	m_sounds.push_back(sound);
}

void SoundManager::setTargetVolume(const float volume, const float fadeTime, const SoundType type)
{
	for (auto &iter : m_sounds)
		iter.get().setTargetVolume(volume, fadeTime);
}

void SoundManager::play(const SoundType type)
{
	for (auto &iter : m_sounds)
		if (iter.get().soundType == type)
			iter.get().play();
}

void SoundManager::pause(const SoundType type)
{
	for (auto &iter : m_sounds)
		if (iter.get().soundType == type)
			iter.get().pause();
}

void Sound::setTargetVolume(const float volume, const float fadeTime)
{
	m_oldVolume = m_sound.getVolume();
	m_targetVolume = volume;
	m_volumeTimeline.setCap(fadeTime);
	m_volumeTimeline.setTimeline(0);
}

void Sound::update(const float elapsedTime)
{
	m_volumeTimeline.update(elapsedTime);
	
	soundType == SoundType::Sound ? 
		m_sound.setVolume(m_volumeTimeline.lerp(m_oldVolume, m_targetVolume)): 
		m_music.setVolume(m_volumeTimeline.lerp(m_oldVolume, m_targetVolume));
}

void Sound::play()
{
	soundType == SoundType::Sound ? m_sound.play() : m_music.play();
}

void Sound::pause()
{
	soundType == SoundType::Sound ? m_sound.pause() : m_music.pause();
}

Sound::Sound(SoundManager & soundManager, const std::string soundName, const SoundType & soundType, const sf::Vector3f position):
	m_soundManager(soundManager),
	soundType(soundType)
{
	m_soundManager.push(*this);

	m_targetVolume = m_oldVolume = 100.f; // Has to be changed to config
	if (soundType == SoundType::Sound)
	{
		m_sound.setBuffer(DataManager::getInstance().getData(soundName).meta.sound);
		if (position == sf::Vector3f(0, 0, 0))
			m_sound.setRelativeToListener(true);
		else
			m_sound.setPosition(position);
	}
	else if (soundType == SoundType::Music)
	{
		m_music.openFromFile("Ray Shaper/data/music/" + soundName+ ".wav");
		m_music.setRelativeToListener(true);
		m_music.setLoop(true);
	}
}
