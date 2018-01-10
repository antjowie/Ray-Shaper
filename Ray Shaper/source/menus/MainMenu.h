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
	sf::View m_view{ {1280 / 2,720 / 2},{1280,720} };
	enum Action
	{
		NewGame,
		Continue,
		Options,
		Exit
	};

public:
	virtual void input(sf::RenderWindow &window) override final;
	
	MainMenu(MenuStack &menuStack);
};