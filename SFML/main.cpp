#include <SFML/Graphics.hpp> 
#include "Game.h"
#include "Menu.h"

using namespace sf;

int main()
{
	RenderWindow window(sf::VideoMode(1280, 720), "Game!"/*, Style::Fullscreen*/);
	while (window.isOpen())
	{
		RunMenu(window);
		RunGame(window);
	}
	return 0;
}
