#include <SFML\Window\Mouse.hpp>

#include "Button.h"
#include "DataManager.h"

void Button::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(m_button, states);
	target.draw(m_text, states);
}

void Button::input(sf::RenderWindow & window)
{
	const sf::Vector2f mousePos{ window.mapPixelToCoords(sf::Mouse::getPosition(window)) };
	if (m_button.getGlobalBounds().contains(mousePos))
	{
		if (!m_hover)
			m_entered = true;
		m_hover = true;
	}
	else
	{
		m_hover = false;
		m_entered = false;
	}

	if (m_entered)
	{
		m_sound.play();
		m_entered = false;
	}

	// Check if originalview has been initialized
	if (m_originalView == sf::FloatRect(0, 0, 0, 0))
		m_originalView = { window.getView().getCenter(),window.getView().getSize() };
}

void Button::update(const float elapsedTime)
{
	m_button.setOutlineColor(m_hover ? green : red);
}

const int Button::getAction()
{
	if (m_hover)
	{
		m_sound.play();
		return m_action;
	}
	return -1;
}

Button::Button(ObjectManager &objectManager, SoundManager &soundManager, Meta & meta, const float textSize, const sf::FloatRect & defaultViewSize):
	Object(objectManager),
	m_originalView(defaultViewSize),
	m_action(meta.action),
	m_sound(soundManager,"button",SoundType::Sound)
{
	m_button.setFillColor(sf::Color(10,10,10));
	m_button.setPosition(meta.button.left, meta.button.top);
	m_button.setSize({ meta.button.width,meta.button.height });
	m_button.setOutlineColor(red);
	m_button.setOutlineThickness(3);
	
	m_text.setFont(DataManager::getInstance().getData("pixeled").meta.font);
	std::transform(meta.text.begin(), meta.text.end(), meta.text.begin(), toupper);
	m_text.setString(meta.text);
	m_text.setPosition(m_button.getPosition() + sf::Vector2f(10, 20)); // Font has offset by 3 pixels
	
	if(textSize != 0)
		m_text.setCharacterSize(textSize);
	else // Calculate based on height
	{
		const float maxHeight{ m_button.getGlobalBounds().height - 6 }; // -6 top 6 bottom for outline and top
		const float textHeight{ m_text.getGlobalBounds().height +3 };
		m_text.setScale({ maxHeight / textHeight }, { maxHeight / textHeight });
		m_text.scale(0.8f, 0.8f);
	}
}

Button::Meta::Meta(const sf::FloatRect & button, const std::string & string, const int action):
	button(button),
	text(string),
	action(action)
{
}
