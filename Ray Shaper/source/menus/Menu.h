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
#include <SFML\Graphics\RectangleShape.hpp>
#include <stack>

#include "ObjectManager.h"
#include "DataManager.h"
#include "SoundManager.h"
#include "Timeline.h"


class MenuStack;

class Menu
{
private:
	std::string m_title;

	// Used for a nice fade effect
	Timeline m_fade;
	Menu * m_newMenu;
	bool m_wantToPush{ false };
	bool m_wantToPop{ false };

protected:
	// Do not call these functions
	MenuStack &m_menuStack;

	ObjectManager m_objectManager;
	SoundManager m_soundManager;
	sf::RectangleShape m_darkOverlay;
	void drawFade(sf::RenderWindow &window);

public:
	// Used to call function when focus is returned to the menu
	bool returnedFocus{ true };

	void push(Menu * const menu);
	void pop();
	bool shouldPop() const;

	// This has to be called in every update function. But because some menu's override the update function,
	// all the fade values are updates in it's own function
	void updateFade(sf::RenderWindow &window, const float elapsedTime);

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