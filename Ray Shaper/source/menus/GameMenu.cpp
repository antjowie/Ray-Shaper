#include <SFML\Window\Event.hpp>

#include "GameMenu.h"

#include "objects\Gate.h"
#include "objects\Emitter.h"
#include "objects\Player.h"

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
	// Reset laserHit, this value is updates in object manager update
	for (auto &iter : m_objectManager.getObjects<Gate*>())
		iter->laserHit = false;

	m_objectManager.update(elapsedTime);
	m_soundManager.update(elapsedTime);

	// Give surrounding tiles a green color
	sf::FloatRect surrTiles{ m_player->getHitbox() };
	surrTiles = { surrTiles.left - 16, surrTiles.top - 16, surrTiles.width + 32, surrTiles.height + 32 };
	for (auto &i : m_objectManager.getTiles())
		for (auto &j : i)
			j.setState(j.getHitbox().intersects(surrTiles));


	// If gates is hit with laser, this will open them when they are near
	sf::FloatRect playerHitbox{ m_player->getHitbox() };
	playerHitbox = { playerHitbox.left - 1, playerHitbox.top, playerHitbox.width + 2, playerHitbox.height };
	for (auto &iter : m_objectManager.getObjects<Gate*>())
	{
		if (iter->getHitbox().intersects(playerHitbox))
			iter->isCollided = true;
		else
			iter->isCollided = false;
	}

	// Camera
	sf::Vector2f cameraMovement{ m_camera.getView().getCenter() };
	
	m_camera.update(elapsedTime);

	m_currentLevel = m_tilemap.getCurrentArea(m_player->getHitbox()).id;
	// If player is not in a playing area
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

	// Update scrolling background
	m_background.move((m_camera.getView().getCenter() - cameraMovement) * 0.25f);
}

void GameMenu::draw(sf::RenderWindow & window)
{
	window.setView(m_camera.getView());
	
	window.draw(m_background);
	window.draw(m_objectManager);
	for (const auto &iter : m_objectManager.getObjects<ReflectionTile*>())
		if(!dynamic_cast<Player::Reflector*>(iter))
			window.draw(*iter);
	for(const auto &iter:m_vertices)
		window.draw(iter);

	drawFade(window);
}

void GameMenu::gainedFocus(sf::RenderWindow & window)
{
	Menu::gainedFocus(window);
	window.setMouseCursorGrabbed(true);
}

GameMenu::GameMenu(MenuStack & menuStack, const std::string &levelPath, bool newGame):
	Menu(menuStack,"Ray Shaper - In Game"), m_camera(2,{0,0,128,72}),
	m_music(m_soundManager,"gameMusic",SoundType::Music)
{
	m_background.setTexture(DataManager::getInstance().getData("gameBackground").meta.texture);
	m_background.setColor(sf::Color(200,200,200));

	m_tilemap.load("test", m_objectManager.getTiles(), m_objectManager,m_soundManager,!newGame);
	if (!m_objectManager.getObjects<Player*>().empty())
		m_player = m_objectManager.getObjects<Player*>().front();
	else
		m_player = new Player(m_objectManager, m_tilemap.getSpawn(1).spawn);
	
	m_level = m_tilemap.getCurrentArea(m_player->getHitbox()).id;
	m_currentLevel = m_tilemap.getCurrentArea(m_player->getHitbox()).id;
	
	m_camera.setCenter(m_player->getPosition());
	m_camera.setTargetSize(m_tilemap.getArea(m_currentLevel).area.height, true);
	m_camera.setBounds(m_tilemap.getArea(m_currentLevel).area);

	// Load all emitter vertices into global vertices
	for (auto &iter : m_objectManager.getObjects<Emitter*>())
		m_vertices.push_back(iter->getVertices());

	m_background.setTextureRect({ 0,0, static_cast<int>(m_objectManager.getTiles()[0].size()) * 16, static_cast<int>(m_objectManager.getTiles().size()) * 16});
	m_background.setOrigin(m_camera.getView().getCenter());
	m_background.setPosition(m_camera.getView().getCenter());
}

GameMenu::~GameMenu()
{
	m_objectManager.saveObjects();
}
