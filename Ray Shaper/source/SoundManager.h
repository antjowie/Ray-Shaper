///////////////////////////////////
//
// The SoundManager class
//
// The SoundManager hold a reference too all the 
// soundobjects. This class exists so that the user
// can adjust the volume of the game and so that other sounds
// can influence eachother without needing to know about eachother
//
///////////////////////////////////

#pragma once
#include <SFML\Audio\Sound.hpp>
#include <SFML\Audio\Music.hpp>
#include <SFML\Audio\SoundBuffer.hpp>

#include "Timeline.h"

enum class SoundType
{
	Sound,
	Music
};

class Sound;

class SoundManager
{
private:
	// With help of this forum
	// https://stackoverflow.com/questions/922360/why-cant-i-make-a-vector-of-references
	std::vector<std::reference_wrapper<Sound>> m_sounds;

public:
	void push(Sound &sound);
	// Volume [0,100]
	// FadeTime Time to reach target volume in seconds
	void setTargetVolume(const float volume, const float fadeTime, const SoundType type);
	void update(const float elapsedTime);

	void play(const SoundType type);
	void pause(const SoundType type);

	~SoundManager();
};

class Sound
{
private:
	SoundManager &m_soundManager;

	sf::Sound m_sound;
	sf::Music m_music;

	Timeline m_volumeTimeline;

	float m_oldVolume;
	float m_targetVolume;

public:
	const SoundType soundType;
	// FadeTime in seconds
	void setTargetVolume(const float volume, const float fadeTime);

	void update(const float elapsedTime);
	void play();
	void pause();

	// Used to specify specific values for spatialization
	sf::Sound &getSound();
	// If position is uninitialized, it will not be spatialized
	Sound(SoundManager &soundManager, const std::string soundName, const SoundType &soundType, const sf::Vector3f position = { 0,0,0 });
};