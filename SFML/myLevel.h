#ifndef MYLEVLEL_H
#define MYLEVLEL_H

#include<SFML/Graphics.hpp>

using namespace sf;

class myLevel
{
public:
	Level level;
	myLevel(int numberLevel) {
		std::ostringstream numberLevelString;//������ ������ �������� ������
		numberLevelString << numberLevel; //������� � ������ �������� ������
		level.LoadFromFile("files/Level" + numberLevelString.str() + ".tmx");//�������� ����� � ������ ������ ������
	}
};

#endif MYLEVLEL_H