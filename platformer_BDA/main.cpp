#include "map.h"//заголовочный файл карты
#include <SFML/Graphics.hpp>
#include "view.h"//заголовочный файл камеры
#include <sstream>
#include <iostream>
#include "mission.h"//заголовочный файл миссии
using namespace sf;
#pragma warning(disable : 4996)//убираем ошибку об устаревшей функции
////////////////////////////////////////////////////КЛАСС ИГРОКА////////////////////////
class Player { // класс Игрока
public:
	float w, h, dx, dy, x, y, speed = 0; //координаты игрока х и у, высота ширина, ускорение (по х и по у), сама скорость
	enum stateObject { left, right, jump, stay, djump };//добавляем тип перечисления-состояние объекта
	stateObject state;//создали экземпляр типа перечисления stateObject
	int playerScore;//переменная хранящая очки игрока
	int health;//переменная хранящая жизни игрока
	bool life;//переменная жизнь, логическая
	bool onGround;//логическая на земле ли персонаж?
	bool djumpAV;//возможность второго прыжка
	bool dir;
	String File; //файл с расширением
	Image image;//сфмл изображение
	Texture texture;//сфмл текстура
	Sprite sprite;//сфмл спрайт
	Player(String F, float X, float Y, float W, float H) { //Конструктор с параметрами(формальными) для класса Player. При создании объекта класса мы будем задавать имя файла, координату Х и У, ширину и высоту
		playerScore = 0;
		life = true;
		health = 100;
		speed = 0;
		dx = 0;
		dy = 0;
		onGround = false;
		state = stay;
		djumpAV = false;
		bool dir = true;
		float currentframe = 0;
		File = F;//имя файла+расширение
		w = W; h = H;//высота и ширина
		image.loadFromFile("images/" + File);//запихиваем в image наше изображение вместо File мы передадим то, что пропишем при создании объекта. В нашем случае "hero.png" и получится запись идентичная image.loadFromFile("images/hero/png");
		texture.loadFromImage(image);//закидываем наше изображение в текстуру
		sprite.setTexture(texture);//заливаем спрайт текстурой
		sprite.setScale(0.4, 0.4);
		x = X; y = Y;//координата появления спрайта
		sprite.setTextureRect(IntRect(6, 26, w, h)); //Задаем спрайту один прямоугольник для вывода персонажа, а не кучи сразу. IntRect - приведение типов
		sprite.setOrigin(w / 2, h);// рисуем спрайта грубо говоря с центра w/2 h/2
	}
	////////////////Управление персонажем////////////////
	void control()
	{
		if ((Keyboard::isKeyPressed(Keyboard::Left)))
		{ //если нажата клавиша стрелка влево
			state = left;
			speed = 0.1;
		}
		if ((Keyboard::isKeyPressed(Keyboard::Right)))
		{ //если нажата клавиша стрелка вправо
			state = right;
			speed = 0.1;
		}
		if ((Keyboard::isKeyPressed(Keyboard::Up)) && (onGround))
		{
			state = jump;
			dy = -0.22;
			onGround = false;
			djumpAV = true;
			//то состояние равно прыжок,прыгнули и сообщили, что мы не на земле
		}
		if ((Keyboard::isKeyPressed(Keyboard::Space)) && (!onGround) && (djumpAV))
		{
			state = djump;
			dy = -0.22;
			djumpAV = false;
			//то состояние равно прыжок,прыгнули и сообщили, что мы не на земле
		}
	}
	void update(float time) //функция "оживления" объекта класса. update - обновление. принимает в себя время SFML , вследствие чего работает бесконечно, давая персонажу движение.
	{
		dx = 0;
		control(); //функция управления персонажем
		switch (state)//тут делаются различные действия в зависимости от состояния
		{
		case right:
		{
			dir = true;
			dx = speed;
			break;//состояние идти вправо
		}
		case left:
		{
			dir = false;
			dx = -speed;
			break;//состояние идти вправо
		}
		case jump:
		{
			djumpAV = true;
			break;
		}
		case stay:
		{
			break;
		}
		case djump:
		{
			break;
		}
		}
		if (onGround)
		{
			djumpAV = false;
		}
		x += dx*time;//наше ускорение на время получаем смещение координат и как следствие движение
		checkCollisionWithMap(dx, 0);//обрабатываем столкновение по X
		y += dy*time;//аналогично по игреку
		checkCollisionWithMap(0, dy);//обрабатываем столкновение по Y
		speed = 0;
		sprite.setPosition(x + w / 4, y + h *0.6);//задаём позицию спрайта в местро кго центраx + w / 2, y + h / 2
		//w/4 чтобы было одинаковое взаимодействие с хитбоксами карты
		if (health <= 0) {
			life = false;
			health = 0;
		}//если жизней меньше либо равно 0, то игрок умирает
		dy = dy + 0.0005*time;
		//делаем притяжение к земле постоянным, так как если сделать только когда переменная ground
		//ложна, то при уходе с холма мы будем "ливитировать" на высоте этого холма пока не прыгнем		

	}
	float GetPlayerCoordinateX() {	//этим методом берём координату Х	
		return x;
	}
	float GetPlayerCoordinateY() {	//этим методом берём координату Y 	
		return y;
	}

	void checkCollisionWithMap(float Dx, float Dy)//ф ция проверки столкновений с картой
	{
		for (int i = y / 32; i < (y + h) / 32; i++)//проходимся по элементам карты
			for (int j = x / 32; j < (x + w) / 32; j++)
			{
				if (TileMap[i][j] == '0')//если элемент наш тайлик земли? то
				{
					if (Dy > 0) {
						y = i * 32 - h;  dy = 0; onGround = true;
					}//по Y вниз=>идем в пол(стоим на месте) или падаем. В этот момент надо вытолкнуть персонажа и поставить его на землю, при этом говорим что мы на земле тем самым снова можем прыгать
					if (Dy < 0) { y = i * 32 + 32;  dy = 0; }//столкновение с верхними краями карты(может и не пригодиться)
					if (Dx > 0) { x = j * 32 - w; }//с правым краем карты
					if (Dx < 0) { x = j * 32 + 32; }// с левым краем карты
				}
				//взаимодействие с картой
				if (TileMap[i][j] == 'c') {
					playerScore++;//если взяли монетку, переменная playerScore=playerScore+1;
					TileMap[i][j] = ' ';
				}
				if (TileMap[i][j] == 'f') {
					health -= 40;//если наступили на колья,то переменная health=health-40;
					TileMap[i][j] = ' ';//убрали колья
				}
				if (TileMap[i][j] == 'j') {
					health -= 40;//если наступили на колья,то переменная health=health-40;
					TileMap[i][j] = ' ';//убрали колья
				}
				if (TileMap[i][j] == 'h') {
					health += 20;//если взяли бочку с водой,то переменная health=health+20;
					TileMap[i][j] = ' ';//убрали сердечко
				}
				if (TileMap[i][j] == 'E') {
					life = false;//закончили игру

				}
			}
	}
};
bool startGame()
{
//настройка миссии
	RenderWindow window(sf::VideoMode(1024, 768), "Jojo");
	view.reset(FloatRect(0, 0, 768, 576)); //размер "вида" камеры при создании объекта вида камеры (чтото вроде инициализации)
	//задаём настройки текста для текста по выводу хп игрока
	Font font_viv_hp;//шрифт 
	font_viv_hp.loadFromFile("CyrilicOld.ttf");//передаем нашему шрифту файл шрифта
	Text text_vivod_hp("", font_viv_hp, 20);//создаем объект текст. закидываем в объект текст строку, шрифт, размер шрифта(в пикселях)//сам объект текст (не строка)
	text_vivod_hp.setColor(Color::Red);//покрасили текст в красный. по умолчанию он белый
	text_vivod_hp.setStyle(Text::Bold);
	//задаём настройки текста по выводу миссии количества собранных монет
	Font font_mis;//шрифт 
	font_mis.loadFromFile("CyrilicOld.ttf");//передаем нашему шрифту файл шрифта
	Text text_mis("", font_mis, 15);//создаем объект текст. закидываем в объект текст строку, шрифт, размер шрифта(в пикселях)//сам объект текст (не строка)
	text_mis.setColor(Color::Black);//покрасили текст в красный. по умолчанию он белый
	text_vivod_hp.setStyle(Text::Bold);
	Image map_image;//объект изображения для карты
	map_image.loadFromFile("images/Castle2.png");//загружаем файл для карты
	Texture map;//текстура карты
	map.loadFromImage(map_image);//загружаем текстуру картинкой
	Sprite s_map;//создаём спрайт для карты
	s_map.setTexture(map);//заливаем текстуру спрайтом
	Map m("mapbgBezk.png", 32, 32, 1280, 928);//делаем фон для карты: начиная с 1 куба, а не 0
	//чтобы мы могли вставить границу чтобы персонаж не падал в пропасть
	Image quest_image;
	quest_image.loadFromFile("images/missionbg.jpg");
	quest_image.createMaskFromColor(Color(0, 0, 0));
	Texture quest_texture;
	quest_texture.loadFromImage(quest_image);
	Sprite s_quest;
	s_quest.setTexture(quest_texture);
	s_quest.setTextureRect(IntRect(0, 0, 340, 510));  //приведение типов, размеры картинки исходные
	s_quest.setScale(0.7f, 0.7f);//чуть уменьшили картинку, => размер стал меньше
	Player p("hero.png", 160, 830, 30, 59);
	//68 118
	//34 59
	bool showMissionText = true;//логическая переменная, отвечающая за появление текста миссии на экране
	float  CurrentFrame = 0;//хранит текущий кадр
	Clock clock; //создаём переменную времени. т.о. привязка по времени (а не загруженность\мощности процессора)
	Clock gameTimeClock;//переменная игрового времени, будем здесь хранить время игры
	int gameTime = 0;
	while (window.isOpen())
	{
		if (Keyboard::isKeyPressed(Keyboard::R)) { return true; }//если таб, то перезагружаем игру
		if (Keyboard::isKeyPressed(Keyboard::Escape)) { return false; }//если эскейп, то выходим из игры
		float time = clock.getElapsedTime().asMicroseconds(); //дать продшее время в микросекундах
		if (p.life)
		{
			gameTime = gameTimeClock.getElapsedTime().asSeconds();
		}		//игровое время в секундах идёт вперёд пока жив игрок
		else
		{
			if (p.health > 0)
			{

			}
			else
			{
				p.sprite.setOrigin(50, -30);
				if (!p.dir)
				{
					p.sprite.setTextureRect(IntRect(1950, 4050, 160, 40));
				}
				else
				{
					p.sprite.setTextureRect(IntRect(2110, 4050, -160, 40));
				}
			}
		}
		clock.restart();
		time = time / 800; //задаём таймер, в зависимости от того насколько делим увеличивается производельность игры (чем больше то на что делим, тем лучше производительность игры), но и потребляемые ресурсы
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == Event::KeyPressed)//событие нажатия клавиши
				if ((event.key.code == Keyboard::Tab)) {//если клавиша ТАБ
					switch (showMissionText) {//переключатель, реагирующий на логическую переменную showMissionText
					case true: {
						showMissionText = false;//эта строка позволяет убрать все что мы вывели на экране
						break;//выходим , чтобы не выполнить условие "false" (которое ниже)
					}
					case false: {
						text_mis.setString("");//если не нажата клавиша таб, то весь этот текст пустой
						showMissionText = true;// а эта строка позволяет снова нажать клавишу таб и получить вывод на экран
						break;
					}
					}
				}
		}
		////////////////Анимация////////////////					
		if (p.life)
		{						
			if (p.onGround)
			{
				if ((Keyboard::isKeyPressed(Keyboard::Left)) || (Keyboard::isKeyPressed(Keyboard::Right)))
				{
					if ((Keyboard::isKeyPressed(Keyboard::Left)))
					{
						//если нажата клавиша стрелка влево
						p.dir = false;
						CurrentFrame += 0.005*time; //служит для прохождения по "кадрам". переменная доходит до 10 суммируя произведение времени и скорости. изменив 0.005 можно изменить скорость анимации
						if (CurrentFrame > 10) CurrentFrame -= 10; //проходимся по кадрам с первого по последний включительно. если пришли к последнему кадру - откидываемся назад.
						p.sprite.setTextureRect(IntRect(115 * int(CurrentFrame) + 30, 540, 112, 118)); //проходимся по координатам 
					   //+30 добавляем чтобы не появлялся кусок следующего тайла
					}
					if ((Keyboard::isKeyPressed(Keyboard::Right)))
					{//если нажата клавиша стрелка вправо
						p.dir = true;
						CurrentFrame += 0.005*time;
						if (CurrentFrame > 10) CurrentFrame -= 10;
						p.sprite.setTextureRect(IntRect(115 * int(CurrentFrame) + 115, 540, -112, 118)); //проходимся по координатам, так как надо отразить тайлы-рисуем справа налево
					}
				}
				else
				{
					if (!p.dir)
					{
						CurrentFrame = 0;
						p.sprite.setTextureRect(IntRect(6, 26, 68, 118));
					}
					else
					{
						CurrentFrame = 0;
						p.sprite.setTextureRect(IntRect(74, 26, -68, 118));
					}
				}
			}
			if ((Keyboard::isKeyPressed(Keyboard::Up)))
			{
				if (p.dir)
				{
					CurrentFrame = 0;
					p.sprite.setTextureRect(IntRect(580, 1075, -110, 120));
				}
				else
				{
					CurrentFrame = 0;
					p.sprite.setTextureRect(IntRect(470, 1075, 110, 120));
				}
			}
			p.update(time);//оживляем объект p класса Player с помощью времени sfml, передавая время в качестве параметра функции update. благодаря этому персонаж может двигаться
		}
		getplayercoordinateforview(p.GetPlayerCoordinateX(), p.GetPlayerCoordinateY());// передаём координаты игрока в функцию управления камерой
		window.setView(view);//"оживляем" камеру в окне sfml
		window.clear();
		window.draw(m.sprite);//рисуем фон
				/////////////////////////////Рисуем подбираемые или взаимодействующие с игроком объекты/////////////////////
		for (int i = 0; i < HEIGHT_MAP; i++)
			for (int j = 0; j < WIDTH_MAP; j++)
			{
				if (TileMap[i][j] == 'c')  s_map.setTextureRect(IntRect(34, 258, 27, 27));//монетка
				if ((TileMap[i][j] == ' ') || (TileMap[i][j] == '0'))  s_map.setTextureRect(IntRect(0, 0, 0, 0));
				if (TileMap[i][j] == 'f') s_map.setTextureRect(IntRect(351, 195, 32, -32));//добавили колья наверх торчащие
				if (TileMap[i][j] == 'j') s_map.setTextureRect(IntRect(351, 163, 32, 32));//добавили колья вниз торчащие
				if (TileMap[i][j] == 'h') s_map.setTextureRect(IntRect(291, 452, 27, 28));//и водичку
				s_map.setPosition(j * 32, i * 32);
				window.draw(s_map);
			}
window.draw(p.sprite);//рисуем спрайт объекта p класса player
		std::ostringstream playerHealthString;//переменная здоровья игрока
		std::ostringstream gameTimeString;//переменная времени игры
		playerHealthString << p.health; //занесли в неё число здоровья, то есть формируем строку
		gameTimeString << gameTime; //занесли в неё время, то есть формируем строку
		text_vivod_hp.setString("Здоровье: " + playerHealthString.str() + "   " + "Время игры: " + gameTimeString.str()); //задаёт строку текста
		text_vivod_hp.setPosition(view.getCenter().x - 350, view.getCenter().y - 250);//задаёт позицию текста
		window.draw(text_vivod_hp);//рисуем текст
		//вывод количества монет и миссии
		if (!showMissionText) {
			std::ostringstream playerScoreString;//строка здоровья игрока
			playerScoreString << p.playerScore; //заносим в строку здоровье 
			std::ostringstream task;//строка текста миссии
			task << getTextMission(getCurrentMission(p.GetPlayerCoordinateX(), p.GetPlayerCoordinateY()));//вызывается функция getTextMission (она возвращает текст миссии), которая принимает в качестве аргумента функцию getCurrentMission(возвращающую номер миссии), а уже эта ф-ция принимает в качестве аргумента функцию p.getplayercoordinateX() (эта ф-ция возвращает Икс координату игрока)
			text_mis.setString("Количество собранных монет: " + playerScoreString.str() + "\n" + task.str());//задаем
			text_mis.setPosition(view.getCenter().x + 140, view.getCenter().y - 120);//позиция всего этого текстового блока
			s_quest.setPosition(view.getCenter().x + 130, view.getCenter().y - 130);//позиция фона для блока			
			window.draw(s_quest); window.draw(text_mis); //рисуем спрайт свитка (фон для текста миссии). а затем и текст. все это завязано на логическую переменную, которая меняет свое состояние от нажатия клавиши ТАБ
		}		
		window.display();
	}
}
void gameRunning() {//ф-ция перезагружает игру , если это необходимо
	if (startGame()) { gameRunning(); }////если startGame() == true, то вызываем занова ф-цию isGameRunning, которая в свою очередь опять вызывает startGame() 
}
int main()
{
	gameRunning();//запускаем процесс игры
	return 0;
}