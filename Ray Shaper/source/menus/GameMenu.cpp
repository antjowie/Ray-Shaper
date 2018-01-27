#include <SFML\Window\Event.hpp>

#include "GameMenu.h"

#include "objects\Gate.h"
#include "objects\Emitter.h"
#include "objects\Player.h"

void GameMenu::update(const float elapsedTime)
{
	// Reset laserHit, this value is updates in object manager update
	//for (auto &iter : m_objectManager.getObjects<Gate*>())
	//	iter->laserHit = false;
	m_currentSection = m_tilemap.getCurrentArea(m_player->getHitbox()).id;
	// Checks whether emitter should update or not
	for (const auto &iter : m_objectManager.getObjects<Emitter*>())
		iter->shouldUpdate = iter->getHitbox().intersects(m_tilemap.getArea(m_currentSection).area);

	m_objectManager.update(elapsedTime);
	m_soundManager.update(elapsedTime);

	// Give surrounding tiles a green color
	sf::FloatRect surrTiles{ m_player->getHitbox() };
	surrTiles = { surrTiles.left - 16, surrTiles.top - 16, surrTiles.width + 32, surrTiles.height + 32 };
	for (auto &i : m_objectManager.getInnerTiles())
			i.get().setState(i.get().getHitbox().intersects(surrTiles));

	// Update camera 
	sf::Vector2f cameraMovement{ m_camera.getView().getCenter() };
	m_camera.update(elapsedTime);

	// If player is not in a playing area
	if (m_currentSection == -1)
	{
		m_camera.setTargetPosition(m_player->getPosition());
		m_camera.setBounds({ 0,0,0,0 });
		m_camera.setTargetSize(288.f,289.f);
	}
	else
	{
		m_camera.setTargetPosition({ m_player->getPosition().x,m_tilemap.getArea(m_currentSection).area.top + m_tilemap.getArea(m_currentSection).area.height / 2.f });
		m_camera.setBounds({ m_tilemap.getArea(m_currentSection).area.left-32.f,m_tilemap.getArea(m_currentSection).area.top,m_tilemap.getArea(m_currentSection).area.width + 64.f,m_tilemap.getArea(m_currentSection).area.height });
		m_camera.setTargetSize(m_tilemap.getArea(m_currentSection).area.width,m_tilemap.getArea(m_currentSection).area.height);
	}

	// Update scrolling background
	m_background.move((m_camera.getView().getCenter() - cameraMovement) * 0.25f);

	// If gates is hit with laser, this will open them when they are near
	sf::FloatRect playerHitbox{ m_player->getHitbox() };
	playerHitbox = { playerHitbox.left - 1, playerHitbox.top, playerHitbox.width + 2, playerHitbox.height };
	
	// Check for new hitcircles
	for (const auto &gate : m_objectManager.getObjects<Gate*>())
	{
		if (gate->getHitbox().intersects(playerHitbox))
			gate->isCollided = true;
		else
			gate->isCollided = false;

		// Check if a gate has been hit
		// We keep checking this because hitCircles are not saved
		if (!gate->hasBeenHit().hasBeenHit)
			continue;

		bool isHit{ false };
		for (const auto &circle : m_hitCircles)
			if (circle.id == gate->hasBeenHit().id)
			{
				isHit = true;
				break;
			}

		if (isHit)
			continue;

		m_hitCircles.push_back(HitCircle(gate->getPosition(), gate->hasBeenHit().id));
	}

	// Update tiles color
	for (auto &gate : m_hitCircles)
	{
		//if (gate.shouldNotCheck())
		//	continue;
		gate.update(elapsedTime);
		for (auto &vertic : m_objectManager.getTiles())
			for (auto &horiz : vertic)
				if(m_tilemap.getArea(gate.id).tileArea.intersects(horiz.getHitbox()))
					gate.checkCollision(horiz);
	}
}

void GameMenu::draw(sf::RenderWindow & window)
{
	window.setView(m_camera.getView());
	
	window.draw(m_background);
	window.draw(m_objectManager);
	for (const auto &iter : m_objectManager.getObjects<ReflectionTile*>())
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
	
	m_currentSection = m_tilemap.getCurrentArea(m_player->getHitbox()).id;
	
	m_camera.setCenter(m_player->getPosition());
	m_camera.setTargetSize(m_tilemap.getArea(m_currentSection).area.height, true);
	m_camera.setBounds(m_tilemap.getArea(m_currentSection).area);

	// Load all emitter vertices into global vertices
	for (auto &iter : m_objectManager.getObjects<Emitter*>())
		m_vertices.push_back(iter->getVertices());

	m_background.setTextureRect({ 0,0, static_cast<int>(m_objectManager.getTiles()[0].size()) * 16, static_cast<int>(m_objectManager.getTiles().size()) * 16});
	m_background.setOrigin(m_camera.getView().getCenter());
	m_background.setPosition(m_camera.getView().getCenter());

	m_objectManager.loadInnerTiles();
}

GameMenu::~GameMenu()
{
	m_objectManager.saveObjects();
}

void GameMenu::HitCircle::update(const float elapsedTime)
{
	const float pixelsPerSecond{ 10.f*16.f };
	if (shape.getSize().x < maxRadius)
		shape.setSize(shape.getSize() + sf::Vector2f{pixelsPerSecond * elapsedTime, pixelsPerSecond * elapsedTime});
}

void GameMenu::HitCircle::checkCollision(Tile & tile)
{
	sf::Vector2f movement{ sf::Vector2f{ tile.getHitbox().left, tile.getHitbox().top } -this->shape.getPosition() };
	tile.setState(std::powf(movement.x, 2) + std::powf(movement.y,2) <= std::powf(this->shape.getGlobalBounds().width,2));
}

bool GameMenu::HitCircle::shouldNotCheck() const
{
	return shape.getGlobalBounds().width >= maxRadius;
}

GameMenu::HitCircle::HitCircle(const sf::Vector2f & position, const int id, const int maxRadius):
	maxRadius(maxRadius), id(id)
{
	shape.setPosition(position);
	shape.setSize({ 0,0 });
}
