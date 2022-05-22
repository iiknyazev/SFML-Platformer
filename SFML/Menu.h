#include <SFML/Graphics.hpp>
using namespace sf;

void RunMenu(RenderWindow& window)
{
	Image Button;
	Button.loadFromFile("files/images/Menu.jpg");

	Texture Button_t;
	Button_t.loadFromImage(Button);

	Sprite start, exit;
	start.setTexture(Button_t);
	exit.setTexture(Button_t);
	start.setTextureRect(IntRect(370, 360, 1210, 260));
	exit.setTextureRect(IntRect(370, 1490, 1210, 260));
	start.setPosition(20, 20);
	exit.setPosition(20, 300);

	bool isMenu = 1;
	int menuNum = 0;

	while (isMenu)
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
		}

		start.setColor(Color::White);
		exit.setColor(Color::White);
		menuNum = 0;

		if (IntRect(20, 20, 1210, 260).contains(Mouse::getPosition(window))) { start.setColor(Color::Green); menuNum = 1; }
		if (IntRect(20, 300, 1210, 260).contains(Mouse::getPosition(window))) { exit.setColor(Color::Green); menuNum = 2; }

		if (Mouse::isButtonPressed(Mouse::Left))
		{
			if (menuNum == 1) isMenu = false;
			if (menuNum == 2) { window.close(); isMenu = false; }
		}

		window.draw(start);
		window.draw(exit);

		window.display();
	}
}
