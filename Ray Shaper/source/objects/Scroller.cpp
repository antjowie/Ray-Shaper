#include <SFML\Window\Mouse.hpp>

#include "Scroller.h"
#include "Config.h"

#include <iostream>

void Scroller::Scroll::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(m_slider, states);
	target.draw(m_leftBorder, states);
	target.draw(m_rightBorder, states);
	target.draw(m_progress, states);
}

sf::FloatRect Scroller::Scroll::getHitbox() const
{
	return sf::FloatRect(m_leftBorder.getGlobalBounds().left,m_leftBorder.getGlobalBounds().top,m_slider.getGlobalBounds().width,m_leftBorder.getGlobalBounds().height);
}

float Scroller::Scroll::input(sf::RenderWindow & window)
{
	const sf::Vector2f mousePos{ window.mapPixelToCoords(sf::Mouse::getPosition(window)) };
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && getHitbox().contains(mousePos))
	{
		hasBeenSelected = true;
		m_percentage = (mousePos.x - m_leftBorder.getGlobalBounds().left) / m_slider.getGlobalBounds().width * 100.f;
		m_progress.setPosition(m_leftBorder.getGlobalBounds().left + m_progress.getGlobalBounds().width * 0.5f 
			+ m_slider.getGlobalBounds().width * (m_percentage * 0.01f), m_progress.getPosition().y);
	}
	else
		hasBeenSelected = false;
	return m_percentage;
}

Scroller::Scroll::Scroll(const sf::FloatRect size, const float sliderPercentage):
	m_percentage(sliderPercentage)
{
	m_leftBorder.setSize({ size.height *0.25f,size.height });
	m_rightBorder.setSize({ size.height *0.25f,size.height });
	m_slider.setSize({ size.width + size.height *0.25f ,size.height * 0.25f });
	m_progress.setSize({ size.height * 0.25f,size.height});

	m_leftBorder.setPosition(size.left, size.top);
	m_rightBorder.setPosition(size.left + size.width + size.height *0.25f, size.top);
	m_slider.setOrigin(0, m_slider.getGlobalBounds().height * 0.5f);
	m_slider.setPosition(size.left, size.top + size.height * 0.5f);
	m_progress.setOrigin(m_progress.getLocalBounds().width * 0.5f, m_progress.getLocalBounds().height * .5f);
	m_progress.setPosition(size.left + size.width * sliderPercentage * 0.01f + size.height * 0.25f * 0.5f, size.top + size.height *0.5f);
}

void Scroller::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(*m_text, states);
	target.draw(m_scroll, states);
}

void Scroller::input(sf::RenderWindow &window)
{
	bool oldState{ m_scroll.hasBeenSelected };
	Config::getInstance().pushData(m_key,Config::Data(m_scroll.input(window),Config::Data::Type::Integer));
	if (m_shouldPlaySound && oldState && !m_scroll.hasBeenSelected)
		m_sound->play();
}

Scroller::Scroller(ObjectManager & objectManager,SoundManager &soundManager,  std::string & string, sf::Vector2f & textPosition, const sf::FloatRect & scrollerPosition, const std::string &key, const bool shouldPlaySound):
	Object(objectManager),m_shouldPlaySound(shouldPlaySound),
	m_sound(new Sound(soundManager,"hit2",SoundType::Sound)),
	m_scroll(scrollerPosition,Config::getInstance().getData(key).code),
	m_key(key)
{
	m_text = new Text(objectManager, textPosition, string);
	m_text->setOrigin({ 0,0 });
	m_text->setPosition(textPosition);
}
