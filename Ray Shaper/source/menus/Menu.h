//////////////////////////////////
//
// The Menu class
//
// A menu is a certain type of screen. It can be compared with
// a gamestate.
//
//////////////////////////////////

#pragma once
#include <SFML\Graphics\RenderWindow.hpp>
#include <stack>

#include "ObjectManager.h"
#include "DataManager.h"

class MenuStack;

class Menu
{
private:
	std::string m_title;

protected:
	MenuStack &m_menuStack;
	ObjectManager m_objectManager;
	
public:
	// Used to call function when focus is returned to the menu
	bool returnedFocus{ true };
	bool shouldPop{ false };

	virtual void input(sf::RenderWindow &window);
	virtual void update(const float elapsedTime);
	virtual void draw(sf::RenderWindow &window);
	
	// Is called when state is selected after the previous state has been popped
	virtual void gainedFocus(sf::RenderWindow& renderWindow);

	Menu(MenuStack &menuStack, const std::string &title = "Ray Shaper");
	virtual ~Menu();
};

class MenuStack
{
private:
	std::stack<Menu *> m_menuStack;

public:

	void push(Menu * const menu);
	Menu* const peek();
	void checkPop();

	~MenuStack();
};