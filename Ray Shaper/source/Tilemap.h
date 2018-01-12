//////////////////////////////////
//
// The Tilemap class
// The tilemap loads the tiles
// In this game, tilemaps are made with Tiled
// and to read the xml files made by Tiled
// the xml parser pugixml is used
//
//////////////////////////////////

#pragma once
#include <SFML\Graphics\Drawable.hpp>
#include <SFML\Graphics\Sprite.hpp>

#include "DataManager.h"
#include "ObjectManager.h"

struct Spawn
{
	sf::Vector2f spawn;
	int id;
	
	Spawn(const int id, const sf::Vector2f &position);
};

struct Area
{
	sf::FloatRect area;
	int id;
	
	Area(const int id, const sf::FloatRect &area);
};

class Tile : public sf::Drawable
{
private:
	sf::Sprite m_sprite;
	bool m_shouldDraw;

	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override final;

public:
	bool m_isSolid{ false };

	// Changes tile color to red or green
	void setState(const bool isGreen);
	const sf::FloatRect getHitbox() const;

	Tile(const sf::Vector2f &position, const bool shouldDraw = false, const bool isGreen = false);

	static Tile getTile(const int id, const sf::Vector2f &position);
};

class Tilemap
{
private:
	std::vector<Spawn> m_spawns;
	std::vector<Area>  m_areas;

public:
	// Error codes
	// -1 Invalid level path
	// -2 Tile layer not found
	// -3 Object layer not found
	// -4 PCData not added
	int load(const std::string &levelPath, std::vector<std::vector<Tile>> &tilemap, ObjectManager &objectManager);

	// id == -1 means not found
	const Spawn getSpawn(const int id) const;
	// id == -1 means not found
	const Area getArea(const int id) const;
};