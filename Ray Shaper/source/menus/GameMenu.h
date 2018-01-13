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

	// Used to get correct spawn and level area
	int m_level{ 1 };

public:
	virtual void input(sf::RenderWindow &window) override final;
	virtual void update(const float elapsedTime) override final;
	virtual void draw(sf::RenderWindow &window)  override final;

	GameMenu(MenuStack &menuStack,const std::string &levelPath);
};