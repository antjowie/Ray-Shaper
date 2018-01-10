#include <SFML\Graphics\RenderWindow.hpp>

#include "menus\Menu.h"
#include "menus\GameMenu.h"

int main()
{
	sf::RenderWindow window;
	MenuStack menuStack;
	sf::Clock clock;
	float elapsedTime;

	window.create(sf::VideoMode(1280, 720), "Ray Shaper", sf::Style::Default);
	window.setFramerateLimit(60);
	
	menuStack.push(new GameMenu(menuStack, "tilemap.tmx"));
	clock.restart();

	while (window.isOpen())
	{
		elapsedTime = clock.restart().asSeconds();

		if (!menuStack.peek())
			continue;

		if (menuStack.peek()->returnedFocus)
			menuStack.peek()->gainedFocus(window);

		menuStack.peek()->input(window);
		menuStack.peek()->update(elapsedTime);

		window.clear();
		menuStack.peek()->draw(window);
		window.display();
		
		menuStack.checkPop();
	}

	return 0;
}