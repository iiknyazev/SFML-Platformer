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
	// инициализация ///
	View view(FloatRect(0, 0, 350, 220));

	const int allLevels = 2;// кол-во всех уровней
	int numberLevel = 1/*номер текущего уровня(первый уровень с номером 1)*/, counter = 1, i;
	bool pause = false, showMissionText = true, isGame = true;

	Font font;//шрифт 
	font.loadFromFile("files/10468.ttf");//передаем нашему шрифту файл шрифта
	Text text("", font, 25);//создаем объект текст. закидываем в объект текст строку, шрифт, размер шрифта(в пикселях);сам объект текст (не строка)
	text.setFillColor(Color::Blue);//покрасили текст в красный; если убрать эту строку, то по умолчанию он белый
	//text.setColor(Color::Blue); старая версия функции из SFML
	text.setStyle(Text::Bold);//жирный текст

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
	gameOver.setTexture(endOfGame);//загрузка спрайта из инициализированной текстуры
	gameOver.setTextureRect(IntRect(350, 320, 590, 80));//выделение конкретного прямоуольного участока из всего изображения
	gameOver.setScale(0.6f, 0.6f);//размер картинки
	gamePause.setTexture(pause_t);
	gamePause.setTextureRect(IntRect(380, 410, 400, 100));
	gamePause.setScale(0.6f, 0.6f);
	missionText.setTexture(missionbg);
	missionText.setScale(0.9f, 0.9f);

	// метод анимации через файл .xml
	AnimationManager anim;
	anim.loadFromXML("files/anim_megaman.xml", player_t);

	// метод ручной анимации
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

	list<newLevel*>  levels;// список указателей на уровни
	list<newLevel*>::iterator itlvl;//итератор(т.е. указатель) на каждый элемент списка

	// создание списка уровней
	for (i = 1; i <= allLevels; i++)
		levels.push_back(new newLevel(i));

	itlvl = levels.begin();//указываем на первый элемент списка уровней

	list<Entity*>  entities;// список указателей на динамические объекты(появляющихся и исчезающих)
	list<Entity*>::iterator it;//итератор(т.е. указатель) на каждый элемент списка

	// поиск всех одноименных объектов и передача переменных в соответствующие классы //
	vector<Object> e = (*itlvl)->level.GetObjects("enemy");//поиск по карте всех объектов с именем "enemy"
	for (i = 0; i < e.size(); i++)
		entities.push_back(new ENEMY(anim3, (*itlvl)->level, e[i].rect.left, e[i].rect.top));

	e = (*itlvl)->level.GetObjects("MovingPlatform");//поиск по карте всех объектов с именем "MovingPlatform"
	for (int i = 0; i < e.size(); i++)
		entities.push_back(new MovingPlatform(anim4, (*itlvl)->level, e[i].rect.left, e[i].rect.top));

	// поиск координат спавна персонажа и передача переменных в класс игрока //
	Object pl = (*itlvl)->level.GetObject("player");
	PLAYER player(anim, (*itlvl)->level, pl.rect.left, pl.rect.top);
	HealthBar healthBar;

	Clock clock;

	/////////////////// основной цикл  /////////////////////
	while (window.isOpen() && isGame)
	{
		window.clear();
		// зарузка уровней после первого //
		if (numberLevel != counter) {
			// очищение контейнеров для нового уровня
			entities.clear();
			e.clear();

			itlvl = levels.begin();//сначала указываем на начала списка
			advance(itlvl, numberLevel - 1);//затем сдвигаем итератор до элемента с новым уровнем

			// поиск всех одноименных объектов и передача переменных в соответствующие классы //
			e = (*itlvl)->level.GetObjects("enemy");//поиск по карте нового уровня всех объектов с именем "enemy"
			for (i = 0; i < e.size(); i++)
				entities.push_back(new ENEMY(anim3, (*itlvl)->level, e[i].rect.left, e[i].rect.top));

			e = (*itlvl)->level.GetObjects("MovingPlatform");//поиск по карте нового уровня всех объектов с именем "MovingPlatform"
			for (int i = 0; i < e.size(); i++)
				entities.push_back(new MovingPlatform(anim4, (*itlvl)->level, e[i].rect.left, e[i].rect.top));

			pl = (*itlvl)->level.GetObject("player");// поиск по карте нового уровня объекта с именем "player"
			player.start(pl.rect.left, pl.rect.top, (*itlvl)->level);// передаем игроку координаты старта на карте нового уровня

			counter++;// увеличение значения счетчика на один, чтобы условие загрузки нового уровня не выполнялось до следущего перехода на новый уровень
		}

		// регулируем скорость игры //
		float time = clock.getElapsedTime().asMicroseconds();
		clock.restart();
		time = time / 500;
		if (time > 40) time = 40;

		// тело событий //
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();

			// однократное нажатие правой клавиши мыши для стрельбы (при зажиме пробела нераздельный поток пуль) // 
			if (event.type == Event::MouseButtonPressed)
				if (event.key.code == Mouse::Left)
					entities.push_back(new Bullet(anim2, (*itlvl)->level, player.x + 18, player.y + 20, player.dir));
			//стрельба потоком
			if (event.type == Event::KeyPressed)
				if (event.key.code == Keyboard::Space)
					entities.push_back(new Bullet(anim2, (*itlvl)->level, player.x + 18, player.y + 20, player.dir));
			//пауза
			if (event.type == Event::KeyPressed)
				if (event.key.code == Keyboard::Escape)
					pause = true;
			//вывод миссии
			if (event.type == Event::KeyPressed)//событие нажатия клавиши
				if ((event.key.code == Keyboard::Tab)) {//если клавиша ТАБ

					switch (showMissionText) {//переключатель, реагирующий на логическую переменную showMissionText

					case true: {
						setlocale(LC_ALL, "rus"); // корректное отображение Кириллицы
						char buff[1000]; // буфер промежуточного хранения считываемого из файла текста
						std::ostringstream numberLevelString;//строка номера текущего уровня
						numberLevelString << numberLevel; //заносим в строку значение номера
						std::ifstream fout("files/Mission" + numberLevelString.str() + ".txt"); // создаём объект класса ofstream для записи и связываем его с файлом текста миссии
						fout >> buff;
						std::ostringstream textMission;
						textMission << buff;
						text.setString(textMission.str());
						fout.close(); // закрываем файл
						showMissionText = false;//эта строка позволяет убрать все что мы вывели на экране
						break;//выходим , чтобы не выполнить условие "false" (которое ниже)
					}
					case false: {
						text.setString("");//если не нажата клавиша таб, то весь этот текст пустой
						showMissionText = true;// а эта строка позволяет снова нажать клавишу таб и получить вывод на экран
						break;
					}
					}
				}
		}

		// если игра не поставлена на паузу
		if (!pause) {
			// проверка нажатий клавиш //
			if (Keyboard::isKeyPressed(Keyboard::Left)) player.key["L"] = true;
			if (Keyboard::isKeyPressed(Keyboard::Right)) player.key["R"] = true;
			if (Keyboard::isKeyPressed(Keyboard::Up)) player.key["Up"] = true;
			if (Keyboard::isKeyPressed(Keyboard::Down)) player.key["Down"] = true;
			if (Keyboard::isKeyPressed(Keyboard::Space)) player.key["Space"] = true;
			if (Mouse::isButtonPressed(Mouse::Left)) player.key["Mouse_Left"] = true;
			// анимация динамических объектов и проверка их существования //
			for (it = entities.begin(); it != entities.end();)
			{
				Entity* b = *it;
				b->update(time, 0);
				if (b->life == false) { it = entities.erase(it); delete b; }
				else it++;
			}

			// проверка состояния персонажа и его передвижение //
			if (player.life)
				player.update(time, numberLevel);
			numberLevel = player.getLevel(numbLvl);
			healthBar.update(player.Health);

			// проверка взаимодействий объектов друг с другом //
			for (it = entities.begin(); it != entities.end(); it++)
			{
				//1. враги
				if ((*it)->Name == "Enemy")
				{
					Entity* enemy = *it;

					if (enemy->Health <= 0)/*если враг мертв, то пропуск*/ continue;

					if (player.getRect().intersects(enemy->getRect()))
						if (!player.hit) {
							player.Health -= 5; player.hit = true;/*при столкновении игрока и врага у игрока отнимается здоровье и запускается анимация ранения */
							if (player.dir) player.x += 10; else player.x -= 10;
						}


					for (list<Entity*>::iterator it2 = entities.begin(); it2 != entities.end(); it2++)// проверка столкновений пуль с врагами
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

				//2. движущиеся платформы
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

				//3.. и т.д.
			}


			// отображение на экран //
			view.setCenter(player.x, player.y);// центрирование камеры вида
			window.setView(view);// передача данных о камере в рабочее окно

			background.setPosition(view.getCenter());// центрирование заднего фона (неподвижен)
			window.draw(background);// прорисовка заднего фона

			(*itlvl)->level.Draw(window);// прорисовка текстур карты

			for (it = entities.begin(); it != entities.end(); it++)// прорисовка динамических объектв
				(*it)->draw(window);

			if (player.life) {// продолжать рисовать персонажа пока он жив
				player.draw(window);// прорисовка персонажа
				healthBar.draw(window);// прорисовка уровня жизни

				////////////////////////ВЫВОД ЛИСТА МИССИИ//////////////////////////////
				if (!showMissionText) {
					Vector2f center = window.getView().getCenter();//координаты центра
					Vector2f size = window.getView().getSize();//координаты всего размера

					missionText.setPosition(center.x + size.x / 2 - 330, center.y - size.y / 2 + 30);//позиция фона для блока	
					text.setPosition(center.x + size.x / 2 - 320, center.y - size.y / 2 + 40);//позиция всего текстового блока

					window.draw(missionText); window.draw(text);//рисуем спрайт свитка (фон для текста миссии), а затем и текст. все это завязано на логической переменной, которая меняет свое состояние от нажатия клавиши ТАБ*/
				}
			}
			else {// конец игры
				int g = 0;//переменная для того, чтобы надпись GAMEOVER работала как кнопка, т.е. только при нажатии в её области происходило действие
				Vector2f size = window.getView().getSize();
				gameOver.setPosition(player.x - 170, player.y - 50);
				if (IntRect(size.x / 2 - 210, size.y / 2 - 30, 420, 60).contains(Mouse::getPosition(window))) { gameOver.setColor(Color::Green); g = 1; }
				else  gameOver.setColor(Color::White);
				if (Mouse::isButtonPressed(Mouse::Left)) { if (g = 1) window.close(); isGame = false; }
				window.draw(gameOver);
			}

			window.display();
		}

		//если игра поставлена на паузу
		else {
			if (Keyboard::isKeyPressed(Keyboard::Space)) pause = false;
			gamePause.setPosition(player.x - 170, player.y - 50);
			window.draw(gamePause);
			window.display();
		}
	}


}

#endif GAME_H