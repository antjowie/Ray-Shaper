#include "KeyButton.h"
#include "InputNames.h"

void KeyButton::setButtonText(const int value)
{
	if (Config::getInstance().getData(m_key).type == Config::Data::Type::Keyboard)
		m_buttonText->setString(InputNames::toString(static_cast<sf::Keyboard::Key>(value)));
	else if (Config::getInstance().getData(m_key).type == Config::Data::Type::Mouse)
		m_buttonText->setString(InputNames::toString(static_cast<sf::Mouse::Button>(value)));
	else if (Config::getInstance().getData(m_key).type == Config::Data::Type::Integer)
		m_buttonText->setString(std::to_string(value));
}

void KeyButton::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(*m_text, states);
	target.draw(*m_buttonText, states);
}

void KeyButton::keyPressed(Config::Data data)
{
	if (!m_isSelected)
		return;

	Config::getInstance().pushData(m_key, data);
	setButtonText(data.code);
	m_isSelected = false;
}

void KeyButton::input(sf::RenderWindow & window)
{
	// When the button is selected
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && getHitbox().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window))))
	{
		m_cooldown.setTimeline(0);
		m_isSelected = true;
	}

	// Deselect if pressed outside off hitbox
	if (m_isSelected && m_cooldown.getProgress() == 100.f)
	{
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !getHitbox().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window))))
			m_isSelected = false;
	}
}

void KeyButton::update(const float elapsedTime)
{
	m_cooldown.update(elapsedTime);

	m_text->setColor(m_isSelected ? green : sf::Color::White);
	m_buttonText->setColor(m_isSelected ? green : sf::Color::White);
}

sf::FloatRect KeyButton::getHitbox() const
{
	return { -0,m_text->getHitbox().top - 25,1280, 75};
}

KeyButton::KeyButton(ObjectManager & objectManager, const sf::Vector2f & textPosition, const sf::Vector2f & buttonPos, std::string & text, const std::string & key):
	Object(objectManager),m_key(key)
{
	m_buttonText = new Text(objectManager, buttonPos, std::string("Undefined"));
	m_text = new Text(objectManager, textPosition, text);

	setButtonText(Config::getInstance().getData(m_key).code);
	m_text->setOrigin({ 0,0 });
	m_buttonText->setOrigin({ 0,0 });

	m_text->setPosition(textPosition);
	m_buttonText->setPosition(buttonPos);

	setButtonText(Config::getInstance().getData(m_key).code);

	m_cooldown.setCap(.5f);
}


