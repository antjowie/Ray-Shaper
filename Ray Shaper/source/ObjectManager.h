#pragma once
#include <SFML\Graphics\Drawable.hpp>
#include <SFML\Graphics\RenderWindow.hpp>
#include <vector>

#include "Tilemap.h" 
// The object class needs the CollisionManager class to be created so we declare that 
// because object also needs CollisionManager
class Object;
struct HitboxObject;

class ObjectManager: public sf::Drawable
{
private:

	std::vector<Object *> m_objects;
	std::vector<std::vector<Tile>> m_tiles;
	// Used for collision detection
	std::vector<std::reference_wrapper<Tile>> m_innerTiles;
	std::string m_levelName;	

	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
public:
	// These functions are used to load and save the object positions
	void saveObjects();
	// If objects save data is not found, this returns false 
	// It also loads all the object so tilemap doesn't have to do it anymore
	bool loadObjects(SoundManager &soundManager);
	// The level name is used to save for the correct level
	void setLevelName(const std::string &levelName);

	// Pushes a new object into the list
	void push(Object * const object);
	// Used when loading another level
	void clear();

	// Updates all objects and delete those which are supposed to be killed
	void input(sf::RenderWindow &window);
	void update(const float elapsedTime);

	// Leaving the template uninitialize will check all objects
	// Blacklist means do not check for this
	template <class T = Object*> Object* fixMovement(const Object *const thisObject, sf::Vector2f &movement, const bool blacklist = false, const bool checkTiles = true);
	
	// Used to load tilemap
	std::vector<std::vector<Tile>> &getTiles();
	
	// Used to load inner tiles, has to be called before the get function
	void loadInnerTiles();
	std::vector<std::reference_wrapper<Tile>> &getInnerTiles();

	// Template is used to get certain type, leave empty if you want all objects
	template <class T = Object*> std::vector<T> getObjects();
	~ObjectManager();
};

template<class T>
inline Object* ObjectManager::fixMovement(const Object *const thisObject, sf::Vector2f & movement, const bool blacklist, const bool checkTiles)
{
	sf::FloatRect h{ thisObject->getHitbox() };
	sf::FloatRect hh{ h };
	sf::FloatRect vh{ h };
	sf::Vector2f &m{ movement };
	Object* returner{ nullptr };

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
		for (auto &j : m_innerTiles)
			{
				const sf::FloatRect &b{ j.get().getHitbox() };

				if (hh.intersects(b))
				{
					if (m.x > 0 && b.left - (h.left + h.width) < m.x)
						m.x = b.left - (h.left + h.width);
					else if (m.x < 0 && (b.left + b.width) - h.left > m.x)
						m.x = (b.left + b.width) - h.left;
				}

				if (vh.intersects(b))
				{
					if (m.y > 0 && b.top - (h.top + h.height) < m.y)
						m.y = b.top - (h.top + h.height);
					else if (m.y < 0 && b.top + b.height - h.top > m.y)
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
		if (iter == thisObject || !iter->isSolid)
			continue;
		if (blacklist && dynamic_cast<T>(iter))
			continue;
		const sf::FloatRect b{ iter->getHitbox() };

		bool intersect{ false };
		if (hh.intersects(b))
		{
			if (m.x > 0 && b.left - (h.left + h.width) < m.x)
			{
				m.x = b.left - (h.left + h.width);
			intersect = true;
			}
		else if (m.x < 0 && (b.left + b.width) - h.left > m.x)
			{
			m.x = (b.left + b.width) - h.left;
			intersect = true;
			}
		}

		if (vh.intersects(b))
		{
			if (m.y > 0 && b.top - (h.top + h.height) < m.y)
			{
				m.y = b.top - (h.top + h.height);
				intersect = true;
			}
		else if (m.y < 0 && b.top + b.height - h.top > m.y)
			{
				m.y = b.top + b.height - h.top;
				intersect = true;
			}
		}

		if (intersect)
		{
			if (!returner)
				returner = iter;
			// Check which one is closet
			else if (std::powf((thisObject->getHitbox().left + thisObject->getHitbox().width / 2.f)	- (iter->getHitbox().left + iter->getHitbox().width / 2.f), 2.f) +
					 std::powf((thisObject->getHitbox().top + thisObject->getHitbox().height / 2.f)	- (iter->getHitbox().top + iter->getHitbox().height / 2.f), 2.f) <
					 std::powf((returner->getHitbox().left + returner->getHitbox().width / 2.f)		- (iter->getHitbox().left + iter->getHitbox().width / 2.f), 2.f) +
					 std::powf((returner->getHitbox().top + returner->getHitbox().height / 2.f)		- (iter->getHitbox().top + iter->getHitbox().height / 2.f), 2.f))
						returner = iter;
		}
	}
	return returner;
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

