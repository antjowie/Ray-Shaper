#pragma once
#include <SFML\Graphics\Drawable.hpp>
#include <SFML\Graphics\RenderWindow.hpp>
#include <list>

#include "Tilemap.h" 

// The object class needs the CollisionManager class to be created so we declare that 
// because object also needs CollisionManager
class Object;

class ObjectManager: public sf::Drawable
{
private:
	std::list<Object *> m_objects;
	std::vector<std::vector<Tile>> m_tiles;
	
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

public:
	// Pushes a new object into the list
	void push(Object * const object);

	// Updates all objects and delete those which are supposed to be killed
	void input(sf::RenderWindow &window);
	void update(const float elapsedTime);

	void fixMovement(const Object * const thisObject, sf::Vector2f &movement);

	// Used to load tilemap
	std::vector<std::vector<Tile>> &getTileVector();
	std::list<Object*> &getObjects();
	~ObjectManager();
};