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

	// Give surrounding tiles a green color
	sf::FloatRect surrTiles{ m_player->getHitbox() };
	surrTiles = { surrTiles.left - 16, surrTiles.top - 16, surrTiles.width + 32, surrTiles.height + 32 };
	for (auto &i : m_objectManager.getTileVector())
		for (auto &j : i)
			j.setState(j.getHitbox().intersects(surrTiles));

	m_camera.update(elapsedTime);
	m_camera.setTargetPosition({ m_player->getPosition().x,m_tilemap.getArea(m_level).area.top + m_tilemap.getArea(m_level).area.height / 2.f });
}

void GameMenu::draw(sf::RenderWindow & window)
{
	window.setView(m_camera.getView());
	
	window.draw(m_objectManager);
	
	drawFade(window);
}

GameMenu::GameMenu(MenuStack & menuStack, const std::string &levelPath):
	Menu(menuStack,"Ray Shaper - In Game"), m_camera(2,{0,0,128,72}),
	m_music(m_soundManager,"gameMusic",SoundType::Music)
{
	m_tilemap.load("test.tmx", m_objectManager.getTileVector(), m_objectManager);
	m_player = new  Player(m_objectManager, { 0,0 });
	m_player->setPosition(m_tilemap.getSpawn(m_level).spawn);
	m_camera.setCenter(m_player->getPosition());
	m_camera.setTargetSize(m_tilemap.getArea(m_level).area.height, true);
}
