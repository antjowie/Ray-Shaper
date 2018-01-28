#include <SFML/Window/Event.hpp>

#include "CreditMenu.h"
#include "objects/Text.h"

void CreditMenu::input(sf::RenderWindow & window)
{
	sf::Event event;
	while (window.pollEvent(event))
		switch (event.type)
		{
		case sf::Event::Closed:
			window.close();
			break;

		case sf::Event::KeyPressed:
		case sf::Event::MouseButtonPressed:
			m_hasClicked = true;
		}

	if (m_canClose)
		window.close();
}

void CreditMenu::update(const float elapsedTime)
{
	m_readTime.update(elapsedTime);
	if (m_readTime.getProgress() == 100 && m_hasClicked)
		m_canClose = true;
	m_hasClicked = false;
}

CreditMenu::CreditMenu(MenuStack & menuStack):
	Menu(menuStack,"Ray Shaper - Thanks for playing")
{
	new Text(m_objectManager, { 640,169 },std::string("Thank you!")				, 80);
	new Text(m_objectManager, { 640,300 }, std::string("For playing my game")	, 70);
	new Text(m_objectManager, { 640,550 }, std::string("Press any key to close"), 40);

	m_readTime.setCap(3.f);
}
