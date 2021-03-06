#include <SFML\Window\Event.hpp>

#include "MainMenu.h"
#include "GameMenu.h"
#include "OptionMenu.h"

#include "objects\MenuBackground.h"
#include "objects\Button.h"
#include "objects\Text.h"

void MainMenu::input(sf::RenderWindow & window)
{
	if (window.hasFocus())
		m_objectManager.input(window);

	sf::Event event;

	// Check if buttons have been pressed
	while(window.pollEvent(event))
		switch (event.type)
		{
		case sf::Event::MouseButtonPressed:
			for (auto &iter : m_objectManager.getObjects<Button*>())
			{
				switch (iter->getAction())
				{
				case NewGame:
					push(new GameMenu(m_menuStack,"test.tmx",true));
					break;
				case Continue:
					push(new GameMenu(m_menuStack, "test.tmx",false));
					break;
				case Options:
					push(new OptionMenu(m_menuStack));
					break;
				case Exit:
					pop();
					break;
				}
			}
			break;

		case sf::Event::Closed:
			window.close();
			break;

		case sf::Event::KeyPressed:
			if (event.key.code == sf::Keyboard::Key::Escape)
				pop();
		}
}

MainMenu::MainMenu(MenuStack & menuStack):
	Menu(menuStack,"Ray Shaper - Main Menu"),
	m_music(m_soundManager,"mainMenuMusic",SoundType::Music)
{
	new MenuBackground(m_objectManager);
	new Text(m_objectManager, { 640.f,140.f}, std::string("Ray Shaper"), 100);
	new Button(m_objectManager,m_soundManager, Button::Meta({ 320,238,600,80 }, "new game", Action::NewGame));
	new Button(m_objectManager,m_soundManager, Button::Meta({ 320,356,600,80 }, "continue", Action::Continue));
	new Button(m_objectManager,m_soundManager, Button::Meta({ 320,474,600,80 }, "options", Action::Options));
	new Button(m_objectManager,m_soundManager, Button::Meta({ 320,592,600,80 }, "exit", Action::Exit));
}