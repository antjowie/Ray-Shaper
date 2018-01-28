//////////////////////////////////
//
// The Text class
//
// A simple wrapper to turn a text object into 
// a Object object
//
//////////////////////////////////

#pragma once
#include <SFML\Graphics\Text.hpp>

#include "Object.h"

class Text : public Object
{
private:
	sf::Text m_text;

	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override final;

public:
	Text(ObjectManager &objectManager, const sf::Vector2f &position, std::string &text, const unsigned int characterSize = 30);

	void setColor(const sf::Color &color);
	void setString(const std::string &string);
	void setOrigin(const sf::Vector2f &origin);
};