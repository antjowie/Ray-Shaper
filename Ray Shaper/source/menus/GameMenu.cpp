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
			if (event.key.code == sf::Keyboard::Escape)
				shouldPop = true;

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

GameMenu::GameMenu(MenuStack & menuStack, const std::string &levelPath):
	Menu(menuStack,"Ray Shaper - In Game"), m_camera(1,{0,0,128,72})
{

	m_tilemap.load("test.tmx", m_tiles, m_objectManager);
}
