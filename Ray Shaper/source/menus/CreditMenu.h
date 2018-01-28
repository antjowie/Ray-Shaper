#pragma once
#include "Menu.h"

class CreditMenu : public Menu
{
private:
	Timeline m_readTime;
	bool m_hasClicked{ false };
	bool m_canClose{ false };

public:
	virtual void input(sf::RenderWindow &window) override final;
	virtual void update(const float elapsedTime) override final;

	CreditMenu(MenuStack &menuStack);
};