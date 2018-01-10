//////////////////////////////////
// 
// Data Manager class
// The Data Manager class keeps an instance of all the local files.
// This class is used to load textures, soundbuffers and fonts for 
// entities which uses them.
//
// Comments
// This could be a singleton class because this game doesn't have many textures
//
//////////////////////////////////

#pragma once
#include <map>
#include <SFML\Graphics\Texture.hpp>
#include <SFML\Graphics\Font.hpp>
#include <SFML\Audio\SoundBuffer.hpp>

class DataManager
{
public:
	enum class Type
	{
		Texture,
		Font,
		Sound
	};

	struct Data
	{
		Type type;

		struct Meta
		{
			sf::Texture texture;
			sf::Font font;
			sf::SoundBuffer sound;
		}meta;

		Data(const std::string &path, const Type &type);
	};

private:
	std::map<const std::string,Data> m_data;

public:
	void pushData(const std::string &name, Data &&data);
	Data & getData(const std::string &name);
};