#include <SFML\Window\Event.hpp>

#include "GameMenu.h"

void GameMenu::input(sf::RenderWindow & window)
{
	sf::Event event;
	while(window.pollEvent(event))
		switch (event.type)
		{
		case sf::Event::Closed:
			window.close();
			break;

		case sf::Event::KeyPressed:
			if(event.key.code == sf::Keyboard::Escape)
				window.close();

			else if (event.key.code == sf::Keyboard::Space)
				for (auto &i : m_tiles)
					for (auto &j : i)
						j.setState(true);
			else
				for (auto &i : m_tiles)
					for (auto &j : i)
						j.setState(false);
			break;
		}
}

void GameMenu::update(const float elapsedTime)
{
}

void GameMenu::draw(sf::RenderWindow & window)
{
	window.setView(m_camera.getView());
	
	for (auto &i : m_tiles)
		for (auto&j : i)
			window.draw(j);
}

void GameMenu::gainedFocus(sf::RenderWindow & renderWindow)
{
	Menu::gainedFocus(renderWindow);
}

GameMenu::GameMenu(MenuStack & menuStack, const std::string &levelPath):
	Menu(menuStack,"Ray Shaper - In Game"), m_camera(1,{0,0,128,72})
{
	m_dataManager.pushData("tile", DataManager::Data("Ray Shaper/data/textures/tile.png", DataManager::Type::Texture));
	m_dataManager.pushData("reflectionTile", DataManager::Data("Ray Shaper/data/textures/reflectionTile.png", DataManager::Type::Texture));
	m_dataManager.pushData("emitter", DataManager::Data("Ray Shaper/data/textures/emitter.png", DataManager::Type::Texture));
	m_dataManager.pushData("laser", DataManager::Data("Ray Shaper/data/textures/laser.png", DataManager::Type::Texture));
	m_dataManager.pushData("gate", DataManager::Data("Ray Shaper/data/textures/gate.png", DataManager::Type::Texture));
	
	m_dataManager.pushData("hit1", DataManager::Data("Ray Shaper/data/sounds/hit1.wav", DataManager::Type::Sound));
	m_dataManager.pushData("hit2", DataManager::Data("Ray Shaper/data/sounds/hit2.wav", DataManager::Type::Sound));
	m_dataManager.pushData("sectionFinished", DataManager::Data("Ray Shaper/data/sounds/sectionFinished.wav", DataManager::Type::Sound));
	m_dataManager.pushData("gameMusic", DataManager::Data("Ray Shaper/data/sounds/gameMusic.wav", DataManager::Type::Sound));

	m_dataManager.pushData("pixeled", DataManager::Data("Ray Shaper/data/fonts/Pixeled.ttf", DataManager::Type::Font));

	m_tilemap.load("test.tmx", m_tiles, m_dataManager, m_objectManager);
}
