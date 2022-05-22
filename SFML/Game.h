#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <list>
#include <iterator>
#include "level.h"
#include "Animation.h"
#include "Entity.h"
#include "Bullet.h"
#include "Player.h"
#include "Enemy.h"
#include "newLevel.h"
#include "HealthBar.h"
#include "MovingPlatform.h"

using namespace sf;
using namespace std;

void RunGame(RenderWindow& window)
{
	// ������������� ///
	View view(FloatRect(0, 0, 350, 220));

	const int allLevels = 2;// ���-�� ���� �������
	int numberLevel = 1/*����� �������� ������(������ ������� � ������� 1)*/, counter = 1, i;
	bool pause = false, showMissionText = true, isGame = true;

	Font font;//����� 
	font.loadFromFile("files/10468.ttf");//�������� ������ ������ ���� ������
	Text text("", font, 25);//������� ������ �����. ���������� � ������ ����� ������, �����, ������ ������(� ��������);��� ������ ����� (�� ������)
	text.setFillColor(Color::Blue);//��������� ����� � �������; ���� ������ ��� ������, �� �� ��������� �� �����
	//text.setColor(Color::Blue); ������ ������ ������� �� SFML
	text.setStyle(Text::Bold);//������ �����

	Image end;
	end.loadFromFile("files/images/game_over.jpg");

	Texture enemy_t, pause_t, player_t, moveplatform_t, bullet_t, missionbg, bg, endOfGame;
	endOfGame.loadFromImage(end);
	bg.loadFromFile("files/images/bg.png");
	enemy_t.loadFromFile("files/images/enemy.png");
	pause_t.loadFromFile("files/images/pause.png");
	player_t.loadFromFile("files/images/megaman.png");
	moveplatform_t.loadFromFile("files/images/movingPlatform.png");
	bullet_t.loadFromFile("files/images/bullet.png");
	missionbg.loadFromFile("files/images/missionbg.jpg");

	Sprite gameOver, gamePause, missionText;
	gameOver.setTexture(endOfGame);//�������� ������� �� ������������������ ��������
	gameOver.setTextureRect(IntRect(350, 320, 590, 80));//��������� ����������� ������������� �������� �� ����� �����������
	gameOver.setScale(0.6f, 0.6f);//������ ��������
	gamePause.setTexture(pause_t);
	gamePause.setTextureRect(IntRect(380, 410, 400, 100));
	gamePause.setScale(0.6f, 0.6f);
	missionText.setTexture(missionbg);
	missionText.setScale(0.9f, 0.9f);

	// ����� �������� ����� ���� .xml
	AnimationManager anim;
	anim.loadFromXML("files/anim_megaman.xml", player_t);

	// ����� ������ ��������
	AnimationManager anim2;
	anim2.create("move", bullet_t, 7, 10, 8, 8, 1, 0);
	anim2.create("explode", bullet_t, 27, 7, 18, 18, 4, 0.01, 29, false);

	AnimationManager anim3;
	anim3.create("move", enemy_t, 0, 0, 16, 16, 2, 0.002, 18);
	anim3.create("dead", enemy_t, 58, 0, 16, 16, 1, 0);

	AnimationManager anim4;
	anim4.create("move", moveplatform_t, 0, 0, 95, 22, 1, 0);

	Sprite background(bg);
	background.setOrigin(bg.getSize().x / 2, bg.getSize().y / 2);
	background.setScale(0.7f, 0.7f);

	list<newLevel*>  levels;// ������ ���������� �� ������
	list<newLevel*>::iterator itlvl;//��������(�.�. ���������) �� ������ ������� ������

	// �������� ������ �������
	for (i = 1; i <= allLevels; i++)
		levels.push_back(new newLevel(i));

	itlvl = levels.begin();//��������� �� ������ ������� ������ �������

	list<Entity*>  entities;// ������ ���������� �� ������������ �������(������������ � ����������)
	list<Entity*>::iterator it;//��������(�.�. ���������) �� ������ ������� ������

	// ����� ���� ����������� �������� � �������� ���������� � ��������������� ������ //
	vector<Object> e = (*itlvl)->level.GetObjects("enemy");//����� �� ����� ���� �������� � ������ "enemy"
	for (i = 0; i < e.size(); i++)
		entities.push_back(new ENEMY(anim3, (*itlvl)->level, e[i].rect.left, e[i].rect.top));

	e = (*itlvl)->level.GetObjects("MovingPlatform");//����� �� ����� ���� �������� � ������ "MovingPlatform"
	for (int i = 0; i < e.size(); i++)
		entities.push_back(new MovingPlatform(anim4, (*itlvl)->level, e[i].rect.left, e[i].rect.top));

	// ����� ��������� ������ ��������� � �������� ���������� � ����� ������ //
	Object pl = (*itlvl)->level.GetObject("player");
	PLAYER player(anim, (*itlvl)->level, pl.rect.left, pl.rect.top);
	HealthBar healthBar;

	Clock clock;

	/////////////////// �������� ����  /////////////////////
	while (window.isOpen() && isGame)
	{
		window.clear();
		// ������� ������� ����� ������� //
		if (numberLevel != counter) {
			// �������� ����������� ��� ������ ������
			entities.clear();
			e.clear();

			itlvl = levels.begin();//������� ��������� �� ������ ������
			advance(itlvl, numberLevel - 1);//����� �������� �������� �� �������� � ����� �������

			// ����� ���� ����������� �������� � �������� ���������� � ��������������� ������ //
			e = (*itlvl)->level.GetObjects("enemy");//����� �� ����� ������ ������ ���� �������� � ������ "enemy"
			for (i = 0; i < e.size(); i++)
				entities.push_back(new ENEMY(anim3, (*itlvl)->level, e[i].rect.left, e[i].rect.top));

			e = (*itlvl)->level.GetObjects("MovingPlatform");//����� �� ����� ������ ������ ���� �������� � ������ "MovingPlatform"
			for (int i = 0; i < e.size(); i++)
				entities.push_back(new MovingPlatform(anim4, (*itlvl)->level, e[i].rect.left, e[i].rect.top));

			pl = (*itlvl)->level.GetObject("player");// ����� �� ����� ������ ������ ������� � ������ "player"
			player.start(pl.rect.left, pl.rect.top, (*itlvl)->level);// �������� ������ ���������� ������ �� ����� ������ ������

			counter++;// ���������� �������� �������� �� ����, ����� ������� �������� ������ ������ �� ����������� �� ��������� �������� �� ����� �������
		}

		// ���������� �������� ���� //
		float time = clock.getElapsedTime().asMicroseconds();
		clock.restart();
		time = time / 500;
		if (time > 40) time = 40;

		// ���� ������� //
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();

			// ����������� ������� ������ ������� ���� ��� �������� (��� ������ ������� ������������ ����� ����) // 
			if (event.type == Event::MouseButtonPressed)
				if (event.key.code == Mouse::Left)
					entities.push_back(new Bullet(anim2, (*itlvl)->level, player.x + 18, player.y + 20, player.dir));
			//�������� �������
			if (event.type == Event::KeyPressed)
				if (event.key.code == Keyboard::Space)
					entities.push_back(new Bullet(anim2, (*itlvl)->level, player.x + 18, player.y + 20, player.dir));
			//�����
			if (event.type == Event::KeyPressed)
				if (event.key.code == Keyboard::Escape)
					pause = true;
			//����� ������
			if (event.type == Event::KeyPressed)//������� ������� �������
				if ((event.key.code == Keyboard::Tab)) {//���� ������� ���

					switch (showMissionText) {//�������������, ����������� �� ���������� ���������� showMissionText

					case true: {
						setlocale(LC_ALL, "rus"); // ���������� ����������� ���������
						char buff[1000]; // ����� �������������� �������� ������������ �� ����� ������
						std::ostringstream numberLevelString;//������ ������ �������� ������
						numberLevelString << numberLevel; //������� � ������ �������� ������
						std::ifstream fout("files/Mission" + numberLevelString.str() + ".txt"); // ������ ������ ������ ofstream ��� ������ � ��������� ��� � ������ ������ ������
						fout >> buff;
						std::ostringstream textMission;
						textMission << buff;
						text.setString(textMission.str());
						fout.close(); // ��������� ����
						showMissionText = false;//��� ������ ��������� ������ ��� ��� �� ������ �� ������
						break;//������� , ����� �� ��������� ������� "false" (������� ����)
					}
					case false: {
						text.setString("");//���� �� ������ ������� ���, �� ���� ���� ����� ������
						showMissionText = true;// � ��� ������ ��������� ����� ������ ������� ��� � �������� ����� �� �����
						break;
					}
					}
				}
		}

		// ���� ���� �� ���������� �� �����
		if (!pause) {
			// �������� ������� ������ //
			if (Keyboard::isKeyPressed(Keyboard::Left)) player.key["L"] = true;
			if (Keyboard::isKeyPressed(Keyboard::Right)) player.key["R"] = true;
			if (Keyboard::isKeyPressed(Keyboard::Up)) player.key["Up"] = true;
			if (Keyboard::isKeyPressed(Keyboard::Down)) player.key["Down"] = true;
			if (Keyboard::isKeyPressed(Keyboard::Space)) player.key["Space"] = true;
			if (Mouse::isButtonPressed(Mouse::Left)) player.key["Mouse_Left"] = true;
			// �������� ������������ �������� � �������� �� ������������� //
			for (it = entities.begin(); it != entities.end();)
			{
				Entity* b = *it;
				b->update(time, 0);
				if (b->life == false) { it = entities.erase(it); delete b; }
				else it++;
			}

			// �������� ��������� ��������� � ��� ������������ //
			if (player.life)
				player.update(time, numberLevel);
			numberLevel = player.getLevel(numbLvl);
			healthBar.update(player.Health);

			// �������� �������������� �������� ���� � ������ //
			for (it = entities.begin(); it != entities.end(); it++)
			{
				//1. �����
				if ((*it)->Name == "Enemy")
				{
					Entity* enemy = *it;

					if (enemy->Health <= 0)/*���� ���� �����, �� �������*/ continue;

					if (player.getRect().intersects(enemy->getRect()))
						if (!player.hit) {
							player.Health -= 5; player.hit = true;/*��� ������������ ������ � ����� � ������ ���������� �������� � ����������� �������� ������� */
							if (player.dir) player.x += 10; else player.x -= 10;
						}


					for (list<Entity*>::iterator it2 = entities.begin(); it2 != entities.end(); it2++)// �������� ������������ ���� � �������
					{
						Entity* bullet = *it2;
						if (bullet->Name == "Bullet")
							if (bullet->Health > 0)
								if (bullet->getRect().intersects(enemy->getRect()))
								{
									bullet->Health = 0; enemy->Health -= 5;
								}
					}
				}

				//2. ���������� ���������
				if ((*it)->Name == "MovingPlatform")
				{
					Entity* movPlat = *it;
					if (player.getRect().intersects(movPlat->getRect()))
						if (player.dy > 0)
							if (player.y + player.h < movPlat->y + movPlat->h)
							{
								player.y = movPlat->y - player.h + 3; player.x += movPlat->dx * time; player.dy = 0; player.STATE = PLAYER::stay;
							}
				}

				//3.. � �.�.
			}


			// ����������� �� ����� //
			view.setCenter(player.x, player.y);// ������������� ������ ����
			window.setView(view);// �������� ������ � ������ � ������� ����

			background.setPosition(view.getCenter());// ������������� ������� ���� (����������)
			window.draw(background);// ���������� ������� ����

			(*itlvl)->level.Draw(window);// ���������� ������� �����

			for (it = entities.begin(); it != entities.end(); it++)// ���������� ������������ �������
				(*it)->draw(window);

			if (player.life) {// ���������� �������� ��������� ���� �� ���
				player.draw(window);// ���������� ���������
				healthBar.draw(window);// ���������� ������ �����

				////////////////////////����� ����� ������//////////////////////////////
				if (!showMissionText) {
					Vector2f center = window.getView().getCenter();//���������� ������
					Vector2f size = window.getView().getSize();//���������� ����� �������

					missionText.setPosition(center.x + size.x / 2 - 330, center.y - size.y / 2 + 30);//������� ���� ��� �����	
					text.setPosition(center.x + size.x / 2 - 320, center.y - size.y / 2 + 40);//������� ����� ���������� �����

					window.draw(missionText); window.draw(text);//������ ������ ������ (��� ��� ������ ������), � ����� � �����. ��� ��� �������� �� ���������� ����������, ������� ������ ���� ��������� �� ������� ������� ���*/
				}
			}
			else {// ����� ����
				int g = 0;//���������� ��� ����, ����� ������� GAMEOVER �������� ��� ������, �.�. ������ ��� ������� � � ������� ����������� ��������
				Vector2f size = window.getView().getSize();
				gameOver.setPosition(player.x - 170, player.y - 50);
				if (IntRect(size.x / 2 - 210, size.y / 2 - 30, 420, 60).contains(Mouse::getPosition(window))) { gameOver.setColor(Color::Green); g = 1; }
				else  gameOver.setColor(Color::White);
				if (Mouse::isButtonPressed(Mouse::Left)) { if (g = 1) window.close(); isGame = false; }
				window.draw(gameOver);
			}

			window.display();
		}

		//���� ���� ���������� �� �����
		else {
			if (Keyboard::isKeyPressed(Keyboard::Space)) pause = false;
			gamePause.setPosition(player.x - 170, player.y - 50);
			window.draw(gamePause);
			window.display();
		}
	}


}

#endif GAME_H