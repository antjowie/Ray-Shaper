//
// Used for option menu volume scrollers
// 
#pragma once
#include <SFML\Graphics\RectangleShape.hpp>

#include "Object.h"
#include "Text.h"

class Scroller :public Object
{
private:
	Text *m_text;
	Sound *m_sound;
	bool m_shouldPlaySound;
	const std::string m_key;

	class Scroll : public sf::Drawable
	{
	private:
		float m_percentage;
		sf::RectangleShape m_leftBorder;
		sf::RectangleShape m_rightBorder;
		sf::RectangleShape m_slider;
		sf::RectangleShape m_progress;

		virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override final;

	public:
		// Used for sound to check if was being hold
		bool hasBeenSelected{ false };
		sf::FloatRect getHitbox() const;
		
		float input(sf::RenderWindow &window);
		Scroll(const sf::FloatRect size, const float sliderPercentage);
	}m_scroll;

	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override final;

public:

	virtual void input(sf::RenderWindow &window)override final;
	
	Scroller(ObjectManager &objectManager, SoundManager &soundManager, std::string &string, sf::Vector2f &textPosition, const sf::FloatRect &scrollerPosition, const std::string &key,const bool shouldPlaySound);
};