#include "ObjectManager.h"
#include "objects\Object.h"
#include "Tilemap.h" 

#define SHOW_HITBOX false
#include <SFML\Graphics\RectangleShape.hpp>

void ObjectManager::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
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

std::list<Object*>& ObjectManager::getObjects()
{
	return m_objects;
}

ObjectManager::~ObjectManager()
{
	for (auto &deleter : m_objects)
		delete deleter;
	m_objects.clear(); // I like to be explicit
}
