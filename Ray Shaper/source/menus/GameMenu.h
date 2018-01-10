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

class GameMenu : public Menu
{
private:
	std::vector<std::vector<Tile>> m_tiles;
	Tilemap m_tilemap;
	Camera m_camera;

public:
	virtual void input(sf::RenderWindow &window) override final;
	virtual void update(const float elapsedTime) override final;
	virtual void draw(sf::RenderWindow &window)  override final;

	GameMenu(MenuStack &menuStack,const std::string &levelPath);
};