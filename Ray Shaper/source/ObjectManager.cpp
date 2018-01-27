#include "ObjectManager.h"
#include "objects\Player.h"
#include "objects\Gate.h"
#include "objects\Emitter.h"
#include "pugixml.hpp"

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
			pHit.setOutlineColor(sf::Color::Yellow);
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

	/*
	for (auto &iter : m_innerTiles)
	{
		
		sf::RectangleShape pHit;
		pHit.setSize(sf::Vector2f(iter.get().getHitbox().width, iter.get().getHitbox().height));
		pHit.setOrigin(0, 0);
		pHit.setPosition(iter.get().getHitbox().left, iter.get().getHitbox().top);
		pHit.setFillColor(sf::Color(255, 255, 255, 100));
		pHit.setOutlineThickness(1);
		pHit.setOutlineColor(sf::Color::Red);
		target.draw(pHit, states);
	}
	*/
}

void ObjectManager::saveObjects()
{
	// The head of the xml tree
	pugi::xml_document xmlHead;

	// Add xml decleration
	pugi::xml_node xmlDecleration{ xmlHead.append_child(pugi::node_declaration) };
	xmlDecleration.append_attribute("version")	= "1.0";
	xmlDecleration.append_attribute("encoding") = "UTF-8";

	pugi::xml_node xmlObjects{ xmlHead.append_child("objects") };
	// Make a node for all data types
	// This made me notice that I should use type instead of casting with dynamic cast
	pugi::xml_node xmlRTile{ xmlObjects.append_child("reflectionTiles") };
	pugi::xml_node xmlGate{ xmlObjects.append_child("gates") };
	pugi::xml_node xmlEmitter{ xmlObjects.append_child("emitters") };
	pugi::xml_node xmlPlayer{ xmlObjects.append_child("players") };

	for (const auto &iter : getObjects<ReflectionTile*>())
	{
		if (dynamic_cast<Player::Reflector*>(iter))
			continue;
		pugi::xml_node object{ xmlRTile.append_child("rtile") };
		object.append_attribute("x") = std::stoi(iter->getSaveData()["x"]);
		object.append_attribute("y") = std::stoi(iter->getSaveData()["y"]);
		object.append_attribute("id") = std::stoi(iter->getSaveData()["id"]);
	}
	for (const auto &iter : getObjects<Gate*>())
	{
		pugi::xml_node object{ xmlGate.append_child("gate") };
		object.append_attribute("x") = std::stoi(iter->getSaveData()["x"]);
		object.append_attribute("y") = std::stoi(iter->getSaveData()["y"]);
		object.append_attribute("state") = std::stoi(iter->getSaveData()["state"]);
		object.append_attribute("id") = std::stoi(iter->getSaveData()["id"]);
	}
	for (const auto &iter : getObjects<Emitter*>())
	{
		pugi::xml_node object{ xmlEmitter.append_child("emitter") };
		object.append_attribute("x") = std::stoi(iter->getSaveData()["x"]);
		object.append_attribute("y") = std::stoi(iter->getSaveData()["y"]);
		object.append_attribute("state") = std::stoi(iter->getSaveData()["state"]);
		object.append_attribute("id") = std::stoi(iter->getSaveData()["id"]);
	}
	// There only is one player but maybe more will be added probably not
	for (const auto &iter : getObjects<Player*>())
	{
		pugi::xml_node object{ xmlPlayer.append_child("player") };
		object.append_attribute("x") = std::stoi(iter->getSaveData()["x"]);
		object.append_attribute("y") = std::stoi(iter->getSaveData()["y"]);
	}

	xmlHead.save_file(std::string(m_levelName + ".xml").c_str());
}

bool ObjectManager::loadObjects(SoundManager &soundManager)
{
	// Check if file exists, else load normal object position and states
	pugi::xml_document xmlHead;
	if (!xmlHead.load_file(std::string(m_levelName + ".xml").c_str()))
		return false;

	pugi::xml_node xmlObject{xmlHead.first_child()};
	pugi::xml_node xmlRTile(xmlObject.find_node([&](pugi::xml_node &node)
	{	return std::string(node.name()) == "reflectionTiles";}));
	pugi::xml_node xmlGate(xmlObject.find_node([&](pugi::xml_node &node)
	{	return std::string(node.name()) == "gates";}));
	pugi::xml_node xmlEmitter(xmlObject.find_node([&](pugi::xml_node &node)
	{	return std::string(node.name()) == "emitters"; }));
	pugi::xml_node xmlPlayer(xmlObject.find_node([&](pugi::xml_node &node)
	{	return std::string(node.name()) == "players"; }));

	for (pugi::xml_node rTile = xmlRTile.first_child(); rTile; rTile = rTile.next_sibling())
		// If it is the reflector
		if(rTile.attribute("id").as_int() != 0)
			new ReflectionTile(*this, rTile.attribute("id").as_int(), { rTile.attribute("x").as_float(),rTile.attribute("y").as_float() });
	for (pugi::xml_node gate = xmlGate.first_child(); gate; gate = gate.next_sibling())
		new Gate(*this,soundManager, gate.attribute("id").as_int(), sf::Vector2f{ gate.attribute("x").as_float(),gate.attribute("y").as_float() }, gate.attribute("state").as_bool());
	for (pugi::xml_node emitter = xmlEmitter.first_child(); emitter; emitter = emitter.next_sibling())
		new Emitter(*this, emitter.attribute("id").as_int(), { emitter.attribute("x").as_float(),emitter.attribute("y").as_float() }, emitter.attribute("state").as_bool());
	for (pugi::xml_node player = xmlPlayer.first_child(); player; player = player.next_sibling())
		new Player(*this, { player.attribute("x").as_float(),player.attribute("y").as_float() });

	return true;
}

void ObjectManager::setLevelName(const std::string & levelName)
{
	m_levelName = levelName;
}

void ObjectManager::push(Object * const object)
{
	m_objects.push_back(object);
}

void ObjectManager::clear()
{
	for (auto &deleter : m_objects)
		delete deleter;
	m_objects.clear();
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

std::vector<std::vector<Tile>>& ObjectManager::getTiles()
{
	return m_tiles;
}

void ObjectManager::loadInnerTiles()
{
	for (size_t y{ 0 }; y < m_tiles.size(); y++)
		for (size_t x{ 0 }; x < m_tiles[y].size(); x++)
		{
			if (!m_tiles[y][x].m_isSolid)
				continue;
			bool m_isSurrounded{ true };

			// If it is on the border, skip it
			if (static_cast<int>(x) - 1 < 0 || 
				static_cast<int>(x) + 1 > m_tiles[y].size() || 
				static_cast<int>(y) - 1 < 0 || 
				static_cast<int>(y) + 1 > m_tiles.size())
				continue;
			
			// Top three tiles
			if (!m_tiles[y - 1][x - 1].m_isSolid)
				m_isSurrounded = false;
			else if (!m_tiles[y - 1][x].m_isSolid)
				m_isSurrounded = false;
			else if (!m_tiles[y - 1][x + 1].m_isSolid)
				m_isSurrounded = false;

			// Middle two tiles
			else if (!m_tiles[y][x - 1].m_isSolid)
				m_isSurrounded = false;
			else if (!m_tiles[y][x + 1].m_isSolid)
				m_isSurrounded = false;

			// Bottom three tiles
			else if (!m_tiles[y + 1][x - 1].m_isSolid)
				m_isSurrounded = false;
			else if (!m_tiles[y + 1][x].m_isSolid)
				m_isSurrounded = false;
			else if (!m_tiles[y + 1][x + 1].m_isSolid)
				m_isSurrounded = false;

			if (m_isSurrounded)
				continue;

			m_innerTiles.push_back(m_tiles[y][x]);
		}
}

std::vector<std::reference_wrapper<Tile>>& ObjectManager::getInnerTiles()
{
	return m_innerTiles;
}

ObjectManager::~ObjectManager()
{
	clear();
}