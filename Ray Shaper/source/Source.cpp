#include <SFML\Graphics\Sprite.hpp>

#include "menus\MainMenu.h"

int main()
{
	sf::RenderWindow window;
	MenuStack menuStack;
	sf::Clock clock;
	float elapsedTime;
	
	sf::Texture tex;
	tex.loadFromFile("Ray Shaper/data/textures/splashscreen.png");
	sf::Sprite splashscreen(tex);

	window.create(sf::VideoMode(250,375), "Ray Shaper - Loading Data", sf::Style::Titlebar);
	window.clear(sf::Color(5,5,5));
	window.draw(splashscreen);
	window.display();

	DataManager::getInstance();
	window.close();

	window.create(sf::VideoMode(1280, 720), "Ray Shaper", sf::Style::Default);
	window.setFramerateLimit(60);
	
	menuStack.push(new MainMenu(menuStack));
	clock.restart();

	while (window.isOpen())
	{
		elapsedTime = clock.restart().asSeconds();

		if (!menuStack.peek())
			break;
			
		if (menuStack.peek()->returnedFocus)
			menuStack.peek()->gainedFocus(window);

		menuStack.peek()->input(window);
		menuStack.peek()->update(elapsedTime);

		window.clear(sf::Color(50,50,50));
		menuStack.peek()->draw(window);
		window.display();
		
		menuStack.checkPop();
	}

	return 0;
}