#pragma once
#include <SFML\Graphics\VertexArray.hpp>

#include "Object.h"
#include "ReflectionTile.h"

#include <iostream>

class Emitter : public Object
{
private:
	const enum Direction
	{
		Up = 10,
		Down,
		Left,
		Right
	}m_direction;

	bool m_active{ false };

	// Compares this to the new tiles, if one tile moved, recalculate laser
	std::vector<float> m_oldTiles;
	sf::VertexArray m_vertices;

	struct Collided
	{
		Object * object{ nullptr };
		const Tile * tile{ nullptr };
		double percentage{ 1 };
		sf::Vector2f point{ 0,0 };
	};
	// Used to get the precise position of intersection, also used to check if collision really happened (if 1 > t > 0)
	// Template defines which type of object to check
	template <class T = Object*> Collided raycastIntersection(const sf::Vector2f &begin, const sf::Vector2f &end, const bool checkTiles, const bool checkObjects) const;

	// Checks if the two ranges intersect eachother
	bool intersects(const double left1, const double right1, const double left2, const double right2) const;

public:
	virtual void update(const float elapsedTime)override final;

	// This has to be loaded into the game menu class
	sf::VertexArray &getVertices();
	Emitter(ObjectManager &objectManager, const int id, const sf::Vector2f &position);
};

template<class T>
inline Emitter::Collided Emitter::raycastIntersection(const sf::Vector2f & begin, const sf::Vector2f & end, const bool checkTiles, const bool checkObjects) const
{
	// It is faster to check all objects instead of checking which objects are close and then check those objects
	Collided collided;
	const sf::Vector2f movement{ end - begin };
	
	if(checkObjects)
	for (const auto &object : m_objectManager.getObjects<T>())
	{
		std::vector<sf::Vector2f> vertices;
			if (dynamic_cast<ReflectionTile*>(object))
			{
				for (const auto &iter : dynamic_cast<ReflectionTile*>(object)->getVertices())
				vertices.push_back(iter);
			}
			else
			{
				vertices.push_back({ object->getHitbox().left, object->getHitbox().top });
				vertices.push_back({ object->getHitbox().left + object->getHitbox().width, object->getHitbox().top });
				vertices.push_back({ object->getHitbox().left + object->getHitbox().width, object->getHitbox().top + object->getHitbox().height });
				vertices.push_back({ object->getHitbox().left, object->getHitbox().top + object->getHitbox().height });
			}
		for (size_t i = 0; i < vertices.size(); ++i)
		{
			const sf::Vector2f begin2{ vertices[i] }; // Line begin
			const sf::Vector2f end2{ vertices[(i + 1) % vertices.size()]}; // Movement of the line
			const sf::Vector2f movement2{ end2 - begin2 };

			// Calculating collision
			// A: object starting point
			// B: object vector 
			// C: object starting point
			// D: object vector
			//
			// Because this collision system is based on rays, we can write each ray like this:
			// A.x + B.x*T1 = C.x + D.x*T2
			// A.y + B.y*T1 = C.y + D.y*T2
			//
			// The T values are percentages. We first isolate T1 out of the formula
			// T1 = (C.x + D.x*T2 - A.x) / B.x = (C.y + D.y*T2 - A.y) / B.y
			//
			// We then need to calculate T2
			// B.y*C.x + T2*D.x*B.y - B.y*A.x = B.x*C.y + T2*D.y*B.x - B.x*A.y
			// T2*D.x*B.y - T2*D.y*B.x = B.x(C.y-A.y) + B.y(A.x - C.x)
			// T2(D.x*B.y - D.y*B.x) = ...
			// T2 = (B.x(C.y-A.y) + B.y(A.x - C.x)) / (D.x*B.y - D.y*B.x)
			//
			// If T1 is larger then 0, collision could happened
			// If T2 is in between 0-1, collision happened (because the object ray couldn't reach it's end)

			// The float data type has some precision issues
			// This should fix that
			const double t2{ (movement.x*(begin2.y - begin.y) + movement.y*(begin.x - begin2.x)) / (movement2.x*movement.y - movement2.y*movement.x) };
			double t1{ (begin2.x + movement2.x*t2 - begin.x) / movement.x };

			// If the x component doesn't change, the equation will divide by zero, so we use the second equation
			if (t1 != t1)
				t1 = (begin2.y + movement2.y*t2 - begin.y) / movement.y;

			// Because off float not being accurate, we use rounded values, values cant be out of these bounds by level design
			if ((t1 > 0.001 && t2 > 0.001 && t2 < 0.999 && t1 < collided.percentage))
			{
				collided.percentage = t1;
				collided.object = object;
				collided.point = sf::Vector2f(begin.x + movement.x * static_cast<float>(t1), begin.y + movement.y * static_cast<float>(t1));
			}
		}
	}

	if(checkTiles)
		for (const auto &i: m_objectManager.getTiles())
			for (const auto &object : i)
	{
		const std::vector<sf::Vector2f> vertices{
			{ object.getHitbox().left,object.getHitbox().top },
			{ object.getHitbox().left + object.getHitbox().width,object.getHitbox().top },
			{ object.getHitbox().left + object.getHitbox().width,object.getHitbox().top + object.getHitbox().height },
			{ object.getHitbox().left,object.getHitbox().top +   object.getHitbox().height }
		};
		for (size_t i = 0; i < vertices.size(); ++i)
		{
			const sf::Vector2f begin2{ vertices[i]}; // Line begin
			const sf::Vector2f end2{ vertices[(i + 1) % vertices.size()]}; // Movement of the line
			const sf::Vector2f movement2{ end2 - begin2 };

			const double t2{ (movement.x*(begin2.y - begin.y) + movement.y*(begin.x - begin2.x)) / (movement2.x*movement.y - movement2.y*movement.x) };
			double t1{ (begin2.x + movement2.x*t2 - begin.x) / movement.x };
			
			if (t1 != t1)
				t1 = (begin2.y + movement2.y*t2 - begin.y) / movement.y;

			if ((t1 > 0.001 && t2 > 0.001 && t2 < 0.999 && t1 < collided.percentage))
			{
				collided.percentage = t1;
				collided.point = sf::Vector2f(begin.x + movement.x * static_cast<float>(t1), begin.y + movement.y * static_cast<float>(t1));
				collided.object = nullptr;
				collided.tile = &object;
			}
		}
	}
	return collided;
}
