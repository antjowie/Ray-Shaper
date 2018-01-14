#include "ObjectManager.h"
#include "objects\Object.h"

#define SHOW_HITBOX false
#include <SFML\Graphics\RectangleShape.hpp>

void ObjectManager::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	for (auto &i : m_tiles)
		for (auto&j : i)
		{
			// Only draw what's on screen
			if (sf::FloatRect(target.getView().getCenter() - target.getView().getSize() / 2.f, target.getView().getSize()).intersects(j.getHitbox()))
				target.draw(j,states);
		
			//if (!SHOW_HITBOX)
			continue;

			sf::RectangleShape pHit;
			pHit.setSize(sf::Vector2f(j.getHitbox().width, j.getHitbox().height));
			pHit.setOrigin(0, 0);
			pHit.setPosition(j.getHitbox().left, j.getHitbox().top);
			pHit.setFillColor(sf::Color(255, 255, 255, 100));
			pHit.setOutlineThickness(1);
			pHit.setOutlineColor(sf::Color::Red);
			target.draw(pHit, states);
		}


	for (auto &iter : m_objects)
	{
		target.draw(*iter, states);

		if (!SHOW_HITBOX)
			continue;
		
		sf::RectangleShape pHit;
		pHit.setSize(sf::Vector2f(iter->getHitbox().width, iter->getHitbox().height));
		pHit.setOrigin(0, 0);
		pHit.setPosition(iter->getHitbox().left, iter->getHitbox().top);
		pHit.setFillColor(sf::Color(255, 255, 255, 100));
		pHit.setOutlineThickness(1);
		pHit.setOutlineColor(sf::Color::Red);
		target.draw(pHit, states);
	}
}

void ObjectManager::push(Object * const object)
{
	m_objects.push_back(object);
}

void ObjectManager::input(sf::RenderWindow & window)
{
	for (auto &iter : m_objects)
		iter->input(window);
}

void ObjectManager::update(const float elapsedTime)
{
	for (auto &iter : m_objects)
		iter->update(elapsedTime);
}

void ObjectManager::fixMovement(const Object * const thisObject, sf::Vector2f & movement)
{
	// TODO
	// Fix corner collision

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
}

std::vector<std::vector<Tile>>& ObjectManager::getTileVector()
{
	return m_tiles;
}

ObjectManager::~ObjectManager()
{
	for (auto &deleter : m_objects)
		delete deleter;
	m_objects.clear(); // I like to be explicit
}
