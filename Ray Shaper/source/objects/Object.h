//////////////////////////////////
//
// The Object class
// An object is an interactable object in the program.
// This regards all collideables and also buttons in menu's.
// The object class serves as a class that keeps these objects
// together and a foundation for them to build on, making
// management of these objects easier because they share the 
// same functions
//
// All objects will be held in class called objectManager, all object
// have a reference to this class as member variable, for their collision 
// checks and other certain functions
//
///////////////////////////////////

#pragma once
#include <SFML\Graphics\Drawable.hpp>
#include <SFML\Graphics\Sprite.hpp>
#include <SFML\Graphics\RenderWindow.hpp>
#include <map>

#include "ObjectManager.h"
#include "AnimationHandler.h"

// These go in pairs
struct Sprite
{
	AnimationHandler animHandler;
	sf::Sprite sprite;

	Sprite(const int frameWidth, const int frameHeight,sf::Texture &texture);
};

// Should not be instantiated
class Object:
	public sf::Drawable
{
private:
	const int m_spriteWidth;
	const int m_spriteHeight;

	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

protected:
	// Assumes that the first sprite is the body
	// All following sprites are other things like eyes or other cosmetics
	std::map<std::string, Sprite> m_sprites;

	// Call in constructor after all variables are loaded
	void init();
	
	// Simple function to make it easier to get values
	Sprite &getSprite(const std::string &name);

	ObjectManager &m_objectManager;

public:
	// Quite bad design but used to check if the object needs to update setFocus
	const bool m_needPlayerPos;
	// Checks whether object should be deleted
	bool m_isDead{ false };

	// This is used to check for input, player and button instances need this function, enemies don't
	virtual void input(sf::RenderWindow &window);
	
	// This function is used to update all values, it is not used to check if keys are pressed
	// In that case use the input function
	virtual void update(const float elapsedTime);

	sf::FloatRect getHitbox() const;
	const sf::Vector2f &getPosition() const;

	void setPosition(const sf::Vector2f &pos);

	// The sprite size is the width and height of the sprite, not the frame
	// This value is used to caluculate a more precise hitbox
	Object(const int spriteWidth, const int spriteHeight, ObjectManager &objectManager, const bool needPlayerPos = false);
};