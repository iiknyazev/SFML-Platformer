#ifndef NEWLEVLEL_H
#define NEWLEVLEL_H

#include<SFML/Graphics.hpp>

using namespace sf;

class newLevel
{
public:
	Level level;
	newLevel(int numberLevel) {
		std::ostringstream numberLevelString;//строка номера текущего уровня
		numberLevelString << numberLevel; //заносим в строку значение номера
		level.LoadFromFile("files/Level" + numberLevelString.str() + ".tmx");//загрузка файла с картой нового уровня
	}
};

#endif NEWLEVLEL_H