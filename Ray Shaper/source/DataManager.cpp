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

DataManager::DataManager()
{
	pushData("tile", DataManager::Data("Ray Shaper/data/textures/tile.png", DataManager::Type::Texture));
	pushData("reflectionTile", DataManager::Data("Ray Shaper/data/textures/reflectionTile.png", DataManager::Type::Texture));
	pushData("emitter", DataManager::Data("Ray Shaper/data/textures/emitter.png", DataManager::Type::Texture));
	pushData("gate", DataManager::Data("Ray Shaper/data/textures/gate.png", DataManager::Type::Texture));
	pushData("background", DataManager::Data("Ray Shaper/data/textures/background.png", DataManager::Type::Texture));
	getData("background").meta.texture.setRepeated(true);
	pushData("gameBackground", DataManager::Data("Ray Shaper/data/textures/gameBackground.png", DataManager::Type::Texture));
	getData("gameBackground").meta.texture.setRepeated(true);	
	pushData("playerBody", DataManager::Data("Ray Shaper/data/textures/player/body.png", DataManager::Type::Texture));
	pushData("playerEyes", DataManager::Data("Ray Shaper/data/textures/player/eyes.png", DataManager::Type::Texture));
	pushData("playerReflector", DataManager::Data("Ray Shaper/data/textures/player/reflector.png", DataManager::Type::Texture));

	pushData("button", DataManager::Data("Ray Shaper/data/sounds/buttonHover.wav", DataManager::Type::Sound));
	pushData("hit1", DataManager::Data("Ray Shaper/data/sounds/hit1.wav", DataManager::Type::Sound));
	pushData("hit2", DataManager::Data("Ray Shaper/data/sounds/hit2.wav", DataManager::Type::Sound));
	pushData("sectionFinished", DataManager::Data("Ray Shaper/data/sounds/sectionFinished.wav", DataManager::Type::Sound));
	
	// Music files shouldn't be loaded into the memory but instead be read while buffering
	//pushData("gameMusic", DataManager::Data("Ray Shaper/data/sounds/gameMusic.wav", DataManager::Type::Sound));
	//pushData("mainMenuMusic", DataManager::Data("Ray Shpaer/data/sounds/mainMenuMusic.wav", DataManager::Type::Sound));

	pushData("pixeled", DataManager::Data("Ray Shaper/data/fonts/Pixeled.ttf", DataManager::Type::Font));
}

void DataManager::pushData(const std::string & name, Data &&data)
{
	m_data.insert({ name,data });
}

DataManager::Data & DataManager::getData(const std::string & name)
{
	return m_data.find(name)->second;
}

DataManager & DataManager::getInstance()
{
	static DataManager dataManager;
	return dataManager;
}
