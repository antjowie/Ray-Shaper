#include <SFML\Window\Event.hpp>

#include "GameMenu.h"
#include "objects\Gate.h"

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

	for (auto &iter : m_objectManager.getObjects<Gate*>())
		if (iter->getHitbox().intersects(m_player->getHitbox()))
			iter->isCollided = true;
		else
			iter->isCollided = false;

	// If player is not in a playing area
	m_currentLevel = m_tilemap.getCurrentArea(m_player->getHitbox()).id;
	if (m_currentLevel == -1)
	{
		m_camera.setTargetPosition(m_player->getPosition());
		m_camera.setBounds({ 0,0,0,0 });
		m_camera.setTargetSize(288.f,false);
	}
	else
	{
		m_camera.setTargetPosition({ m_player->getPosition().x,m_tilemap.getArea(m_currentLevel).area.top + m_tilemap.getArea(m_currentLevel).area.height / 2.f });
		m_camera.setBounds({ m_tilemap.getArea(m_currentLevel).area.left-32.f,m_tilemap.getArea(m_currentLevel).area.top,m_tilemap.getArea(m_currentLevel).area.width + 64.f,m_tilemap.getArea(m_currentLevel).area.height });
		m_camera.setTargetSize(m_tilemap.getArea(m_currentLevel).area.height,true);
	}
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

	m_level = m_tilemap.getCurrentArea(m_player->getHitbox()).id;
	m_currentLevel = m_tilemap.getCurrentArea(m_player->getHitbox()).id;

	m_player->setPosition(m_tilemap.getSpawn(m_level).spawn);

	m_camera.setCenter(m_player->getPosition());
	m_camera.setTargetSize(m_tilemap.getArea(m_currentLevel).area.height, true);
	m_camera.setBounds(m_tilemap.getArea(m_currentLevel).area);
}
