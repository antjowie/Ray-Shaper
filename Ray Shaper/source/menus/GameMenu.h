//////////////////////////////////
//
// The GameMenu class
//
// The GameMenu class is a gamestate where the user
// is playing the game. 
//
//////////////////////////////////

#pragma once
#include "Menu.h"
#include "Tilemap.h"
#include "Camera.h"
#include "objects\Player.h"

class GameMenu : public Menu
{
private:
	Tilemap m_tilemap;
	Camera m_camera;
	Player *m_player;
	Sound m_music;
	// All the lasers are loaded here
	std::vector<std::reference_wrapper<sf::VertexArray>> m_vertices;

	// Highest level, the same as progress
	int m_level{ 1 };
	// Level the player is in right now, used for camera
	int m_currentLevel{ 0 };

public:
	virtual void input(sf::RenderWindow &window) override final;
	virtual void update(const float elapsedTime) override final;
	virtual void draw(sf::RenderWindow &window)  override final;

	virtual void gainedFocus(sf::RenderWindow &window) override final;

	GameMenu(MenuStack &menuStack,const std::string &levelPath);
	virtual ~GameMenu();
};