#include "Menu.h"

void Menu::input(sf::RenderWindow & window)
{
	m_objectManager.input(window);
}

void Menu::update(const float elapsedTime)
{
	m_objectManager.update(elapsedTime);
}

void Menu::draw(sf::RenderWindow & window)
{
	window.draw(m_objectManager);
}

void Menu::gainedFocus(sf::RenderWindow& renderWindow)
{
	renderWindow.setTitle(m_title);
	returnedFocus = false;
}

Menu::Menu(MenuStack &menuStack, const std::string &title):
	m_title(title),m_menuStack(menuStack)
{
}

Menu::~Menu()
{
}

void MenuStack::push(Menu * const menu)
{
	m_menuStack.push(menu);
}

Menu * const MenuStack::peek()
{
	if(m_menuStack.empty())
		return nullptr;
	return m_menuStack.top();
}

void MenuStack::checkPop()
{
	if (peek()->shouldPop)
		m_menuStack.pop();
}

MenuStack::~MenuStack()
{
	while(!m_menuStack.empty())
	{
		delete m_menuStack.top();
		m_menuStack.pop();
	}
}
