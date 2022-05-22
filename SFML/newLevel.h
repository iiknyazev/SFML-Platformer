#ifndef NEWLEVLEL_H
#define NEWLEVLEL_H

#include<SFML/Graphics.hpp>

using namespace sf;

class newLevel
{
public:
	Level level;
	newLevel(int numberLevel) {
		std::ostringstream numberLevelString;//������ ������ �������� ������
		numberLevelString << numberLevel; //������� � ������ �������� ������
		level.LoadFromFile("files/Level" + numberLevelString.str() + ".tmx");//�������� ����� � ������ ������ ������
	}
};

#endif NEWLEVLEL_H