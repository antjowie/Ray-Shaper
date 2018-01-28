#pragma once
#include "Config.h"
#include "Text.h"
#include "Timeline.h"

class KeyButton : public Object
{
private:
	bool m_isSelected;
	std::string m_key;
	Text *m_text;
	Text *m_buttonText;
	Timeline m_cooldown;

	const sf::Color green{ 58,166,62,255 };

	void setButtonText(const int value);
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override final;

public:
	// Because we handle key pressed in a event loop, the event loop will push the new
	// key value to the key button that is selected
	void keyPressed(Config::Data data);

	virtual void input(sf::RenderWindow & window) override final;
	virtual void update(const float elapsedTime) override final;

	virtual sf::FloatRect getHitbox() const override final;

	KeyButton(ObjectManager &objectManager, const sf::Vector2f & textPosition, const sf::Vector2f & buttonPos, std::string &text, const std::string &key);
};
