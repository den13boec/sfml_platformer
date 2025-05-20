#include <SFML/Graphics.hpp>
using namespace sf;
View view; //объ€вили smfl объект "вид", который и €вл€етс€ камерой

View getplayercoordinateforview(float x, float y) { //функци€ дл€ считывани€ координат игрока
	float tempX = x;
	float tempY = y; //считываем координаты игрока и провер€ем их, чтобы убрать кра€	
	if (x < 416) tempX = 416;//убираем из вида левую сторону "пустоты"
	if (x > 925) tempX = 925;//убираем из вида правую сторону "пустоты" 
	if (y < 340) tempY = 340;//верхнюю
	if (y > 650) tempY = 650;//нижнюю
	view.setCenter(tempX, tempY);//следим за игроком, передава€ его координаты
	//сделан вид немножко выше персонажа, чтобы он был немножко снизу-такой вид в платформерах
	return view;
}