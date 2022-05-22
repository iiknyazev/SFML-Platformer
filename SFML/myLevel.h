#ifndef MYLEVLEL_H
#define MYLEVLEL_H

#include<SFML/Graphics.hpp>

using namespace sf;

class myLevel
{
public:
	Level level;
	myLevel(int numberLevel) {
		std::ostringstream numberLevelString;//строка номера текущего уровня
		numberLevelString << numberLevel; //заносим в строку значение номера
		level.LoadFromFile("files/Level" + numberLevelString.str() + ".tmx");//загрузка файла с картой нового уровня
	}
};

#endif MYLEVLEL_H