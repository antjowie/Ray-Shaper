#pragma once
#include <SFML\Graphics\RectangleShape.hpp>

#include "Object.h"

class Gate : public Object
{
private:
	const int m_id;
	sf::RectangleShape m_upperSprite;
	sf::RectangleShape m_lowerSprite;

	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override final;

public:
	bool isCollided{ false };

	virtual void update(const float elapsedTime) override final;

	virtual sf::FloatRect getHitbox() const override final;

	Gate(ObjectManager &objectManager, const int id, sf::Vector2f &position);
};