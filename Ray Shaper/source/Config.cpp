#include <SFML\Window\Keyboard.hpp>
#include <SFML\Window\Mouse.hpp>

#include "Config.h"

Config::Config()
{
	pushData("soundVolume", Config::Data(100, Config::Data::Type::Integer));
	pushData("musicVolume", Config::Data(10, Config::Data::Type::Integer));
	
	pushData("left", Config::Data(sf::Keyboard::Key::A, Data::Type::Keyboard));
	pushData("right", Config::Data(sf::Keyboard::Key::D, Data::Type::Keyboard));
	pushData("jump", Config::Data(sf::Keyboard::Key::Space, Data::Type::Keyboard));

	pushData("crouch", Config::Data(sf::Keyboard::Key::LShift, Data::Type::Keyboard));
	pushData("grab", Config::Data(sf::Keyboard::Key::F, Data::Type::Keyboard));
	pushData("toggleReflector", Config::Data(sf::Mouse::Left, Data::Type::Mouse));
}

void Config::pushData(const std::string & key, Data & data)
{
	m_data.insert({ key,data });
}

Config::Data Config::getData(const std::string & key)
{
	return m_data.find(key)->second;
}

Config & Config::getInstance()
{
	static Config instance;
	return instance;
}

Config::Data::Data(const int code, const Type type):
	code(code), type(type)
{
}