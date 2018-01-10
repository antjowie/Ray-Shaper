//////////////////////////////////
//
// The MenuBackground class
//
// This is a very simple class that gives a menu a background
// I could make more background but that isn't neccessary, hence
// why I made the class always use the same texture
//
//////////////////////////////////

#pragma once
#include "Object.h"

class MenuBackground : public Object
{
private:
	// The original center
	sf::Vector2f m_center;
	// The mouse position
	sf::Vector2f m_mousePos;
	// Target position to move towards
	sf::Vector2f m_targetPos;
public:
	virtual void input(sf::RenderWindow &window) override final;
	virtual void update(const float elapsedTime) override final;

	MenuBackground(ObjectManager &objectManager);
};