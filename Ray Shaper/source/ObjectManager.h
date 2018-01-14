#pragma once
#include <SFML\Graphics\Drawable.hpp>
#include <SFML\Graphics\RenderWindow.hpp>
#include <vector>

#include "Tilemap.h" 
// The object class needs the CollisionManager class to be created so we declare that 
// because object also needs CollisionManager
class Object;

class ObjectManager: public sf::Drawable
{
private:
	std::vector<Object *> m_objects;
	std::vector<std::vector<Tile>> m_tiles;
	
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

public:
	// Pushes a new object into the list
	void push(Object * const object);

	// Updates all objects and delete those which are supposed to be killed
	void input(sf::RenderWindow &window);
	void update(const float elapsedTime);

	// Leaving the template uninitialize will check all objects
	// Blacklist means do not check for this
	template <class T = Object*> void fixMovement(const Object *const thisObject, sf::Vector2f &movement, const bool blacklist = false, const bool checkTiles = true);

	// Used to load tilemap
	std::vector<std::vector<Tile>> &getTileVector();
	
	// Template is used to get certain type, leave empty if you want all objects
	template <class T = Object*> std::vector<T> getObjects();
	~ObjectManager();
};

template<class T>
inline void ObjectManager::fixMovement(const Object *const thisObject, sf::Vector2f & movement, const bool blacklist, const bool checkTiles)
{
	sf::FloatRect h{ thisObject->getHitbox() };
	sf::FloatRect hh{ h };
	sf::FloatRect vh{ h };
	sf::Vector2f &m{ movement };

	// This section updates the hitbox, the hitbox is a rectangle which contains the whole movement from last to next frame
	// This rectangle is used to check nearby tiles whether or not they will collide with the hitbox
	if (m.x > 0)
		hh = { hh.left,hh.top,hh.width + m.x, hh.height };

	else if (m.x < 0)
		hh = { hh.left + m.x,hh.top,hh.width - m.x, hh.height };

	if (m.y > 0)
		vh = { vh.left,vh.top, vh.width, vh.height + m.y };

	else if (m.y < 0)
		vh = { vh.left, vh.top + m.y, vh.width, vh.height - m.y };

	// Iterate though all tiles and check whether or not they collided
	if(checkTiles)
		for (auto &i : m_tiles)
			for (auto &j : i)
			{
				const sf::FloatRect &b{ j.getHitbox() };

				if (hh.intersects(b))
				{
					if (m.x > 0)
						m.x = b.left - (h.left + h.width);
					else if (m.x < 0)
						m.x = (b.left + b.width) - h.left;
				}

				if (vh.intersects(b))
				{
					if (m.y > 0)
						m.y = b.top - (h.top + h.height);
					else if (m.y < 0)
						m.y = b.top + b.height - h.top;
				}
			}
	std::vector<Object*> objects;
	if (blacklist)
		objects.swap(getObjects<>());
	else
		// This doesn't work if vector type isn't the same as the normal type, the type to swap with probably has to be the same
		//objects.swap(getObjects<T>());
		for (auto &iter : getObjects<T>())
			objects.push_back(iter);

	for (auto iter: objects)
	{
		if (iter == thisObject)
			continue;
		if (blacklist && dynamic_cast<T>(iter))
			continue;
		const sf::FloatRect b{ iter->getHitbox() };

		if (hh.intersects(b))
		{
			if (m.x > 0)
				m.x = b.left - (h.left + h.width);
			else if (m.x < 0)
				m.x = (b.left + b.width) - h.left;
		}

		if (vh.intersects(b))
		{
			if (m.y > 0)
				m.y = b.top - (h.top + h.height);
			else if (m.y < 0)
				m.y = b.top + b.height - h.top;
		}
	}
}

template<class T>
inline std::vector<T> ObjectManager::getObjects()
{
	std::vector<T> returner;
	for (auto &iter : m_objects)
		if (dynamic_cast<T>(iter))
			returner.push_back(static_cast<T>(iter));

	return returner;
}

