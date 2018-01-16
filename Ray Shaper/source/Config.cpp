#include "Config.h"

Config::Config()
{
	pushData("soundVolume", Config::Data(100, Config::Data::Type::Integer));
	pushData("musicVolume", Config::Data(10, Config::Data::Type::Integer));
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