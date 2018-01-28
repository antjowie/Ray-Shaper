#include <SFML\Window\Keyboard.hpp>
#include <SFML\Window\Mouse.hpp>

#include "Config.h"
#include "pugixml.hpp"

Config::Config()
{
	// If data was successfully loaded
	if (load())
		return;
	
	pushData("soundVolume", Config::Data(100, Config::Data::Type::Integer));
	pushData("musicVolume", Config::Data(10, Config::Data::Type::Integer));
	
	pushData("left", Config::Data(sf::Keyboard::Key::A, Data::Type::Keyboard));
	pushData("right", Config::Data(sf::Keyboard::Key::D, Data::Type::Keyboard));
	pushData("jump", Config::Data(sf::Keyboard::Key::Space, Data::Type::Keyboard));

	pushData("crouch", Config::Data(sf::Keyboard::Key::LShift, Data::Type::Keyboard));
	pushData("grab", Config::Data(sf::Keyboard::Key::F, Data::Type::Keyboard));
	pushData("toggleReflector", Config::Data(sf::Mouse::Left, Data::Type::Mouse));

	pushData("frameLimit", Config::Data(60, Data::Type::Integer));
}

Config::~Config()
{
	save();
}

void Config::pushData(const std::string & key, Data & data)
{
	// Used to overwrite value
	if (!m_data.insert({ key,data }).second)
	{
		m_data.erase(key);
		m_data.insert({ key,data });
	}
}

Config::Data Config::getData(const std::string & key)
{
	return m_data.find(key)->second;
}

bool Config::load()
{
	pugi::xml_document xmlHead;

	if (!xmlHead.load_file("config.xml"))
		return false;

	for (pugi::xml_node xmlConfigGroup{ xmlHead.first_child().first_child() }; xmlConfigGroup; xmlConfigGroup = xmlConfigGroup.next_sibling())
	{
		if (std::string(xmlConfigGroup.name()) == "integer")
			for (pugi::xml_node xmlConfig{ xmlConfigGroup.first_child() }; xmlConfig; xmlConfig = xmlConfig.next_sibling())
				pushData(std::string(xmlConfig.name()), Data(xmlConfig.attribute("data").as_int(), Data::Type::Integer));
		else if (std::string(xmlConfigGroup.name()) == "mouse")
			for (pugi::xml_node xmlConfig{ xmlConfigGroup.first_child() }; xmlConfig; xmlConfig = xmlConfig.next_sibling())
				pushData(std::string(xmlConfig.name()), Data(xmlConfig.attribute("data").as_int(), Data::Type::Mouse));
		else if (std::string(xmlConfigGroup.name()) == "keyboard")
			for (pugi::xml_node xmlConfig{ xmlConfigGroup.first_child() }; xmlConfig; xmlConfig = xmlConfig.next_sibling())
				pushData(std::string(xmlConfig.name()), Data(xmlConfig.attribute("data").as_int(),Data::Type::Keyboard));
	}

	return true;
}

void Config::save()
{
	pugi::xml_document xmlHead;
	pugi::xml_node xmlDecleration{ xmlHead.append_child(pugi::xml_node_type::node_declaration) };
	xmlDecleration.append_attribute("version") = "1.0";
	xmlDecleration.append_attribute("encoding") = "UTF-8";

	pugi::xml_node xmlConfigGroup{ xmlHead.append_child("config") };

	pugi::xml_node xmlInteger{ xmlConfigGroup.append_child("integer") };
	pugi::xml_node xmlMouse{ xmlConfigGroup.append_child("mouse") };
	pugi::xml_node xmlKeyboard{ xmlConfigGroup.append_child("keyboard") };

	for (const auto &iter : m_data)
	{
		if (iter.second.type == Data::Type::Integer)
		{
			pugi::xml_node node{ xmlInteger.append_child(iter.first.c_str()) };
			node.append_attribute("data").set_value(iter.second.code);
		}
		else if (iter.second.type == Data::Type::Mouse)
		{
			pugi::xml_node node{ xmlMouse.append_child(iter.first.c_str()) };
			node.append_attribute("data").set_value(iter.second.code);
		}
		else if (iter.second.type == Data::Type::Keyboard)
		{
			pugi::xml_node node{ xmlKeyboard.append_child(iter.first.c_str()) };
			node.append_attribute("data").set_value(iter.second.code);
		}
	}

	xmlHead.save_file("config.xml");
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