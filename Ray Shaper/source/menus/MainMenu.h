//////////////////////////////////
//
// The MainMenu class
//
// Self-explanatory
//
//////////////////////////////////

#pragma once
#include "Menu.h"

class MainMenu : public Menu
{
private:
	enum Action
	{
		NewGame,
		Continue,
		Options,
		Exit
	};

	sf::View m_view{ {1280 / 2,720 / 2},{1280,720} };
	Sound m_music;

public:

	virtual void input(sf::RenderWindow &window) override final;

	MainMenu(MenuStack &menuStack);
};