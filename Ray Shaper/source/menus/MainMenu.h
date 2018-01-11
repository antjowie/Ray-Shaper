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

	Sound m_music;

public:
	virtual void input(sf::RenderWindow &window) override final;

	MainMenu(MenuStack &menuStack);
};