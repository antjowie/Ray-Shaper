//////////////////////////////////
//
// The Button class
//
// The button class instantiates a button in the global space
// The location of the button is the same on the screen, so it
// moves with the view. This is used for overlays (pause screen)
//
//////////////////////////////////

#pragma once
#include <SFML\Graphics\RectangleShape.hpp>
#include <SFML\Graphics\Text.hpp>

#include "Object.h"

class Button : public Object
{
public:
	struct Meta
	{
		sf::FloatRect button;
		std::string text;
		// The action value is the value that the object returns. Menu's can make their own
		// enums to keep track of what it is supposed to do
		int action;

		Meta(const sf::FloatRect &button, const std::string &string, const int action);
	};

private:
	// Used for the outline change
	bool m_hover{ false };
	// Used for the sound
	bool m_entered{ false };
	// These are the two colors used for switching outline color
	// Because they may change, I have made them member variables
	const sf::Color red{ 171,41,41,200 };
	const sf::Color green{ 58,166,62,200 };

	// This is the offset from the view center, the spawn location
	const sf::Vector2f m_offset;
	// This is used to keep track of resizing. When the view is resized
	// The buttons aren't so this calculated the scale
	sf::FloatRect m_originalView;

	sf::RectangleShape m_button;
	sf::Text m_text;
	const int m_action;

	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override final;

public:
	virtual void input(sf::RenderWindow &window);
	virtual void update(const float elapsedTime);

	// Return his action if hovered over
	// else returns -1
	const int getAction() const;

	// If textSize is 0, the text size will be adjusted to fit in the button
	// If defaultViewSize is left uninitialized the input function will use the view size it first gets
	Button(ObjectManager &objectManager, Meta &meta, const float textSize = 0, const sf::FloatRect &defaultViewSize = { 0,0,0,0 });
};