#include "DataManager.h"

DataManager::Data::Data(const std::string & path, const Type & type):
	type(type)
{
	switch (type)
	{
	case Type::Texture:
		meta.texture.loadFromFile(path);
		break;
	case Type::Font:
		meta.font.loadFromFile(path);
		break;
	case Type::Sound:
		meta.sound.loadFromFile(path);
		break;
	}
}

void DataManager::pushData(const std::string & name, Data &&data)
{
	m_data.insert({ name,data });
}

DataManager::Data & DataManager::getData(const std::string & name)
{
	return m_data.find(name)->second;
}
