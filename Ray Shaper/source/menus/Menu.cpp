#include <SFML\Window\Event.hpp>

#include "Menu.h"

void Menu::input(sf::RenderWindow & window)
{
	sf::Event event;
	while(window.pollEvent(event))
		switch (event.type)
		{
		case sf::Event::Closed:
			window.close();
			break;
		
		case sf::Event::KeyPressed:
			if(event.key.code == sf::Keyboard::Escape)
				shouldPop = true;
		}

	m_objectManager.input(window);
}

void Menu::update(const float elapsedTime)
{
	m_objectManager.update(elapsedTime);
}

void Menu::draw(sf::RenderWindow & window)
{
	window.setView(window.getDefaultView());
	window.draw(m_objectManager);
}

void Menu::gainedFocus(sf::RenderWindow& renderWindow)
{
	renderWindow.setTitle(m_title);
	m_soundManager.play(SoundType::Music);
	returnedFocus = false;
}

Menu::Menu(MenuStack &menuStack, const std::string &title):
	m_title(title),m_menuStack(menuStack)
{
}

Menu::~Menu()
{
	m_soundManager.pause(SoundType::Music);
}

void MenuStack::push(Menu * const menu)
{
	if (!m_menuStack.empty())
		m_menuStack.top()->returnedFocus = true;
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
