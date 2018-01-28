#include <SFML\Window\Event.hpp>

#include "OptionMenu.h"
#include "objects/Button.h"
#include "objects/KeyButton.h"
#include "objects/Scroller.h"
#include "objects/MenuBackground.h"


#include <iostream>
void OptionMenu::input(sf::RenderWindow & window)
{
	sf::Event event;
	while (window.pollEvent(event))
		switch (event.type)
		{
		case sf::Event::Closed:
			window.close();
			break;

		case sf::Event::KeyPressed:
			if (event.key.code == sf::Keyboard::Escape)
				pop();
			else
				for (auto &iter : m_objectManager.getObjects<KeyButton*>())
					iter->keyPressed(Config::Data(event.key.code, Config::Data::Type::Keyboard));
			break;

		case sf::Event::MouseButtonPressed:
			for (auto &iter : m_objectManager.getObjects<KeyButton*>())
				iter->keyPressed(Config::Data(event.key.code, Config::Data::Type::Mouse));
			break;
		}

	if (window.hasFocus())
		m_objectManager.input(window);
}

void OptionMenu::update(const float elapsedTime)
{
	m_soundManager.setTargetVolume(Config::getInstance().getData("musicVolume").code, 0, SoundType::Music);
	m_soundManager.setTargetVolume(Config::getInstance().getData("soundVolume").code, 0, SoundType::Sound);
	m_objectManager.update(elapsedTime);
	m_soundManager.update(elapsedTime);

//	std::cout << Config::getInstance().getData("musicVolume").code << '\t'
//			  << Config::getInstance().getData("soundVolume").code << '\n';
}

OptionMenu::OptionMenu(MenuStack & menuStack):
	Menu(menuStack,"Ray Shaper - Option menu"),
	m_music(m_soundManager,"gameMusic",SoundType::Music)
{
	new MenuBackground(m_objectManager);
	new Scroller(m_objectManager, m_soundManager, std::string("Music"), sf::Vector2f{ 58,25 }, sf::FloatRect{ 300,15,880,50 }, "musicVolume", false);
	new Scroller(m_objectManager, m_soundManager, std::string("Sound"), sf::Vector2f{ 58,100 }, sf::FloatRect{ 300,90,880,50 }, "soundVolume", true);
	new KeyButton(m_objectManager, { 58,200 }, { 880,200 }, std::string("Jump"), "jump");
	new KeyButton(m_objectManager, { 58,275 }, { 880,275 }, std::string("Left"), "left");
	new KeyButton(m_objectManager, { 58,350 }, { 880,350 }, std::string("Right"), "right");
	new KeyButton(m_objectManager, { 58,425 }, { 880,425 }, std::string("Crouch"), "crouch");
	new KeyButton(m_objectManager, { 58,500 }, { 880,500 }, std::string("Grab"), "grab");
	new KeyButton(m_objectManager, { 58,575 }, { 880,575 }, std::string("Reflector"), "toggleReflector");
	new Text(m_objectManager, { 640,650 }, std::string("Press escape to return"), 20);
}