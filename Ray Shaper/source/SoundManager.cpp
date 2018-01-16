#include "SoundManager.h"
#include "DataManager.h"
#include "Config.h"

void SoundManager::push(Sound & sound)
{
	m_sounds.push_back(sound);
}

void SoundManager::setTargetVolume(const float volume, const float fadeTime, const SoundType type)
{
	for (auto &iter : m_sounds)
		if(iter.get().soundType == type)
			iter.get().setTargetVolume(volume, fadeTime);
}

void SoundManager::update(const float elapsedTime)
{
	for (auto &iter : m_sounds)
		iter.get().update(elapsedTime);
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

SoundManager::~SoundManager()
{
	pause(SoundType::Music);
	pause(SoundType::Sound);
}

void Sound::setTargetVolume(const float volume, const float fadeTime)
{
	m_oldVolume = soundType == SoundType::Sound ? m_sound.getVolume() : m_music.getVolume();
	m_targetVolume = volume;
	m_volumeTimeline.setCap(fadeTime);
	m_volumeTimeline.setTimeline(0);
}

void Sound::update(const float elapsedTime)
{
	m_volumeTimeline.update(elapsedTime);
	const float newVolume{ m_volumeTimeline.lerp(m_oldVolume, m_targetVolume) };

	if(soundType == SoundType::Sound ? m_sound.getVolume() : m_music.getVolume() != m_targetVolume)
		soundType == SoundType::Sound ?
			m_sound.setVolume(newVolume): m_music.setVolume(newVolume);
}

void Sound::play()
{
	soundType == SoundType::Sound ? m_sound.play() : m_music.play();
}

void Sound::pause()
{
	soundType == SoundType::Sound ? m_sound.pause() : m_music.pause();
}

sf::Sound & Sound::getSound()
{
	return m_sound;
}

Sound::Sound(SoundManager & soundManager, const std::string soundName, const SoundType & soundType, const sf::Vector3f position):
	m_soundManager(soundManager),
	soundType(soundType)
{
	m_soundManager.push(*this);

	m_targetVolume = m_oldVolume = (soundType == SoundType::Sound ? Config::getInstance().getData("soundVolume").code : Config::getInstance().getData("musicVolume").code);
	m_sound.setVolume(m_targetVolume);
	m_music.setVolume(m_targetVolume);
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
