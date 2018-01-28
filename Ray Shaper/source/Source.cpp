#include <SFML\Graphics\Sprite.hpp>

#include "menus\MainMenu.h"
#include "Config.h"

#include "menus\CreditMenu.h"

int main()
{
	std::srand(static_cast<unsigned int>(std::time(NULL)));
	std::rand();

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
	Config::getInstance();
	window.close();

	window.create(sf::VideoMode(1280, 720), "Ray Shaper", sf::Style::Default);
	window.setFramerateLimit(Config::getInstance().getData("frameLimit").code);
	//window.setVerticalSyncEnabled(true);

	//menuStack.push(new MainMenu(menuStack));
	menuStack.push(new CreditMenu(menuStack));
	clock.restart();

	while (window.isOpen())
	{
		elapsedTime = clock.restart().asSeconds();

		// Happens when below 10 fps, game becomes unstable, should actually wait instead of changing the elapsedTime
		if (elapsedTime > 0.1f)
			elapsedTime = 0.1f;

		if (!menuStack.peek())
			break;
			
		if (menuStack.peek()->returnedFocus)
			menuStack.peek()->gainedFocus(window);

		// NOTE: Unsure but apperantly this line has a bug where the buttons in the main menu wont appear
		// if the window does not have focus
		//if(window.hasFocus())
		menuStack.peek()->input(window);
		menuStack.peek()->update(elapsedTime);
		menuStack.peek()->updateFade(window,elapsedTime);

		window.clear(sf::Color(50,50,50));
		menuStack.peek()->draw(window);
		window.display();
		
		menuStack.checkPop();
	}

	return 0;
}