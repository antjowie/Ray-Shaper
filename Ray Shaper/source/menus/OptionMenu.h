#pragma once
#include "Menu.h"

class OptionMenu : public Menu
{
private:
	Sound m_music;

public:
	virtual void input(sf::RenderWindow &window) override final;
	virtual void update(const float elapsedTime) override final;

	OptionMenu(MenuStack &menuStack);
};