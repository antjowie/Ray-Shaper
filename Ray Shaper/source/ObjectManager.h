#pragma once
#include <SFML\Graphics\Drawable.hpp>
#include <SFML\Graphics\RenderWindow.hpp>
#include <list>

// The object class needs the CollisionManager class to be created so we declare that 
// because object also needs CollisionManager
class Object;

class Tilemap;

class ObjectManager: public sf::Drawable
{
private:
	std::list<Object *> m_objects;

	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

public:
	// Pushes a new object into the list
	void push(Object * const object);

	// Updates all objects and delete those which are supposed to be killed
	void input(sf::RenderWindow &window);
	void update(const float elapsedTime);

	std::list<Object*> &getObjects();
	~ObjectManager();
};