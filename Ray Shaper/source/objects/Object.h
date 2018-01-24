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

#include "ObjectManager.h"

// Should not be instantiated
class Object:
	public sf::Drawable
{
private:
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
	
protected:
	sf::Sprite m_sprite;

	ObjectManager &m_objectManager;

public:
	// Checks whether object should be checked for collision
	bool isSolid{ true };
	// Used to check if the object deflects the laser to know if it has special functions
	// I would normally use an enum class but there are only 2 types that are
	// different in the way it should be handled
	// This is used to check for input, player and button instances need this function, enemies don't
	virtual void input(sf::RenderWindow &window);
	
	// This function is used to update all values, it is not used to check if keys are pressed
	// In that case use the input function
	virtual void update(const float elapsedTime);

	// If a shape doesn't have a rectangular hitbox
	virtual sf::FloatRect getHitbox() const;
	const sf::Vector2f &getPosition() const;

	// Some classes use more than one sprite
	virtual void setPosition(const sf::Vector2f &pos);
	virtual void move(const sf::Vector2f &movement);

	// This function returns the data used to save it
	virtual std::map<std::string, std::string> getSaveData() const;

	Object(ObjectManager &objectManager);
};