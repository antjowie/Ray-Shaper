#include <SFML\Graphics\RenderTarget.hpp>
#include <queue>

#include "Tilemap.h"
#include "pugixml.hpp"
#include "DataManager.h"

void Tile::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	if(m_shouldDraw)
		target.draw(m_sprite, states);
}

void Tile::setState(const bool isGreen)
{
	sf::IntRect textureRect{ m_sprite.getTextureRect() };
	if (isGreen)
		textureRect.left = textureRect.width;
	else
		textureRect.left = 0;
	m_sprite.setTextureRect(textureRect);
}

const sf::FloatRect Tile::getHitbox() const
{
	return m_sprite.getGlobalBounds();
}

Tile::Tile(const sf::Vector2f &position, const bool shouldDraw, const bool isGreen):
	m_shouldDraw(shouldDraw)
{
	if (!m_shouldDraw) return;

	m_sprite.setTexture(DataManager::getInstance().getData("tile").meta.texture);
	m_sprite.setTextureRect({ 0,0,16,16 });
	m_sprite.setPosition(position);
	setState(isGreen);
}

Tile Tile::getTile(const int id, const sf::Vector2f & position)
{
	if(id == 1)
		return  Tile(position, true, false);
	return  Tile(position, false, false);
}

int Tilemap::load(const std::string &levelPath, std::vector<std::vector<Tile>> &tilemap, ObjectManager &objectManager)
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(std::string(levelPath).c_str());

	if (!doc)
		return -1;

	pugi::xml_node map = doc.first_child();
	
	const int tilemapWidth = map.find_attribute([]( const pugi::xml_attribute &attr) {return std::string(attr.name()) == "width"; }).as_int();
	const int tilemapHeight = map.find_attribute([](const pugi::xml_attribute &attr) {return std::string(attr.name()) == "height"; }).as_int();

	pugi::xml_node data = map.find_node([](const pugi::xml_node &node) {return std::string(node.name()) == "layer"; }).first_child();
	if (!data)
		return -2;

	std::string buffer{ data.child_value() };
	if (buffer.empty())
		return -4;

	std::queue<int> fixedBuffer;
	buffer.erase(std::remove(buffer.begin(), buffer.end(), '\n'), buffer.end());

	// This gets all tile id's
	while(!buffer.empty())
	{ 
		auto iter{ std::find(buffer.begin(),buffer.end(),',') };
		std::string value{ buffer.begin(),iter  };
		if (iter != buffer.end())
			buffer.erase(buffer.begin(), iter + 1);
		else
			buffer.clear();

		fixedBuffer.push(std::stoi(value));
	}
	
	// Some tiles are loaded as objects, this map converts them
	tilemap.resize(tilemapHeight);
	for (int vertic{ 0 }; vertic < tilemapHeight; vertic++)
	{
		tilemap[vertic].reserve(tilemapWidth);
		for (int horiz{ 0 }; horiz < tilemapWidth; horiz++)
		{
			int id{ fixedBuffer.front() };
			fixedBuffer.pop();
			
			// Load into object 
			if (id > 1)
			{
				switch (id)
				{
				default:
					break;
				}
				id = 0;
			}
			// Load tile into this
			tilemap[vertic].push_back(Tile::getTile(id, { horiz*16.f,vertic*16.f }));
		}
	}

	pugi::xml_node objectgroup = map.find_node([](const pugi::xml_node &node) {return std::string(node.name()) == "objectgroup"; });
	if (!objectgroup)
		return -3;

	// Load all objects
	for(pugi::xml_node object = objectgroup.first_child();object;object = object.next_sibling())
	{
		const std::string type{ object.attribute("type").as_string() };
		if (type == "spawn")
			m_spawns.push_back(Spawn(object.first_child().first_child().attribute("value").as_int(),{ object.attribute("x").as_float(), object.attribute("y").as_float() }));
		else if (type == "area")
			m_areas.push_back(Area(object.first_child().first_child().attribute("value").as_int(), { object.attribute("x").as_float() ,object.attribute("y").as_float() ,object.attribute("width").as_float() ,object.attribute("height").as_float() }));
	}

	return 0;
}

const Spawn Tilemap::getSpawn(const int id) const
{
	for (auto &iter : m_spawns)
		if (iter.id == id)
			return iter;
	return Spawn(-1, { 0,0 });
}

const Area Tilemap::getArea(const int id) const
{
	for (auto &iter : m_areas)
		if (iter.id == id)
			return iter;
	return Area(-1, { 0,0 ,0,0});
}

Spawn::Spawn(const int id, const sf::Vector2f & position):
	id(id),spawn(position)
{
}

Area::Area(const int id, const sf::FloatRect & area):
	id(id),area(area)
{
}
