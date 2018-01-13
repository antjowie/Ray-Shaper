#include <SFML\Window\Event.hpp>

#include "Menu.h"

void Menu::push(Menu * const menu)
{
	if (m_wantToPush) return;
	m_newMenu = menu;
	m_wantToPush = true;
}

void Menu::pop()
{
	m_wantToPop = true;
}

bool Menu::shouldPop() const
{
	return (m_wantToPop && m_fade.getProgress() == 0.f);
}

void Menu::updateFade(sf::RenderWindow &window, const float elapsedTime)
{
	if(m_wantToPop || m_wantToPush)
		m_fade.update(-elapsedTime);
	else
		m_fade.update(elapsedTime);

	sf::Uint8 newColor{ static_cast<sf::Uint8>(255.f / 100.f * m_fade.getProgress()) };
	m_darkOverlay.setFillColor({ newColor,newColor,newColor });

	m_soundManager.setTargetVolume(m_fade.getProgress(),0,SoundType::Music);
	m_soundManager.update(0);

	if (m_wantToPush && m_fade.getProgress() == 0.f)
	{
		m_soundManager.pause(SoundType::Music);
		m_menuStack.push(m_newMenu);
	}
}

void Menu::drawFade(sf::RenderWindow & window)
{
	// Makes sure the dark overlay takes the whole screen
	m_darkOverlay.setPosition({ window.getView().getCenter().x - window.getView().getSize().x / 2.f,window.getView().getCenter().y - window.getView().getSize().y / 2.f });
	m_darkOverlay.setSize(window.getView().getSize());
	window.draw(m_darkOverlay, sf::BlendMultiply);
}

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
				pop();
		}

	m_objectManager.input(window);
}

void Menu::update(const float elapsedTime)
{
	m_objectManager.update(elapsedTime);
	m_soundManager.update(elapsedTime);
}

void Menu::draw(sf::RenderWindow & window)
{
	window.setView(window.getDefaultView());
	window.draw(m_objectManager);

	drawFade(window);
}

void Menu::gainedFocus(sf::RenderWindow& renderWindow)
{
	renderWindow.setTitle(m_title);

	m_soundManager.play(SoundType::Music);
	returnedFocus = false;
	m_wantToPush = false;
	m_newMenu = nullptr;
}

Menu::Menu(MenuStack &menuStack, const std::string &title):
	m_title(title),m_menuStack(menuStack)
{
	m_darkOverlay.setFillColor({ 0,0,0 });
	m_fade.setCap(0.1f);

	// Makes sound fade in
	m_soundManager.setTargetVolume(0, 0, SoundType::Music);
	m_soundManager.update(0);
	m_soundManager.play(SoundType::Music);
}

Menu::~Menu()
{
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
	if (peek()->shouldPop())
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
