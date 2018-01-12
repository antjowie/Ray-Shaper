#include <SFML\Window\Event.hpp>

#include "GameMenu.h"

void GameMenu::input(sf::RenderWindow & window)
{
	m_objectManager.input(window);

	sf::Event event;
	while(window.pollEvent(event))
		switch (event.type)
		{
		case sf::Event::Closed:
			window.close();
			break;

		case sf::Event::KeyPressed:
			if (event.key.code == sf::Keyboard::Escape)
				pop();
		}
}

void GameMenu::update(const float elapsedTime)
{
	m_objectManager.update(elapsedTime);
	m_player->fixMovement(m_tiles,elapsedTime);

	m_camera.update(elapsedTime);
	m_camera.setTargetPosition(m_player->getPosition());
}

void GameMenu::draw(sf::RenderWindow & window)
{
	window.setView(m_camera.getView());
	
	for (auto &i : m_tiles)
		for (auto&j : i)
			window.draw(j);

	window.draw(m_objectManager);
	
	drawFade(window);
}

GameMenu::GameMenu(MenuStack & menuStack, const std::string &levelPath):
	Menu(menuStack,"Ray Shaper - In Game"), m_camera(5,{0,0,128,72})
{
	m_tilemap.load("test.tmx", m_tiles, m_objectManager);
	m_player = new  Player(m_objectManager, { 0,0 });
	m_player->setPosition(m_tilemap.getSpawn(1).spawn);
	m_camera.setCenter(m_player->getPosition());
	m_camera.setTargetSize(m_tilemap.getArea(1).area.height, true);
	
}
