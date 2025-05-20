#include "map.h"//������������ ���� �����
#include <SFML/Graphics.hpp>
#include "view.h"//������������ ���� ������
#include <sstream>
#include <iostream>
#include "mission.h"//������������ ���� ������
using namespace sf;
#pragma warning(disable : 4996)//������� ������ �� ���������� �������
////////////////////////////////////////////////////����� ������////////////////////////
class Player { // ����� ������
public:
	float w, h, dx, dy, x, y, speed = 0; //���������� ������ � � �, ������ ������, ��������� (�� � � �� �), ���� ��������
	enum stateObject { left, right, jump, stay, djump };//��������� ��� ������������-��������� �������
	stateObject state;//������� ��������� ���� ������������ stateObject
	int playerScore;//���������� �������� ���� ������
	int health;//���������� �������� ����� ������
	bool life;//���������� �����, ����������
	bool onGround;//���������� �� ����� �� ��������?
	bool djumpAV;//����������� ������� ������
	bool dir;
	String File; //���� � �����������
	Image image;//���� �����������
	Texture texture;//���� ��������
	Sprite sprite;//���� ������
	Player(String F, float X, float Y, float W, float H) { //����������� � �����������(�����������) ��� ������ Player. ��� �������� ������� ������ �� ����� �������� ��� �����, ���������� � � �, ������ � ������
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
		File = F;//��� �����+����������
		w = W; h = H;//������ � ������
		image.loadFromFile("images/" + File);//���������� � image ���� ����������� ������ File �� ��������� ��, ��� �������� ��� �������� �������. � ����� ������ "hero.png" � ��������� ������ ���������� image.loadFromFile("images/hero/png");
		texture.loadFromImage(image);//���������� ���� ����������� � ��������
		sprite.setTexture(texture);//�������� ������ ���������
		sprite.setScale(0.4, 0.4);
		x = X; y = Y;//���������� ��������� �������
		sprite.setTextureRect(IntRect(6, 26, w, h)); //������ ������� ���� ������������� ��� ������ ���������, � �� ���� �����. IntRect - ���������� �����
		sprite.setOrigin(w / 2, h);// ������ ������� ����� ������ � ������ w/2 h/2
	}
	////////////////���������� ����������////////////////
	void control()
	{
		if ((Keyboard::isKeyPressed(Keyboard::Left)))
		{ //���� ������ ������� ������� �����
			state = left;
			speed = 0.1;
		}
		if ((Keyboard::isKeyPressed(Keyboard::Right)))
		{ //���� ������ ������� ������� ������
			state = right;
			speed = 0.1;
		}
		if ((Keyboard::isKeyPressed(Keyboard::Up)) && (onGround))
		{
			state = jump;
			dy = -0.22;
			onGround = false;
			djumpAV = true;
			//�� ��������� ����� ������,�������� � ��������, ��� �� �� �� �����
		}
		if ((Keyboard::isKeyPressed(Keyboard::Space)) && (!onGround) && (djumpAV))
		{
			state = djump;
			dy = -0.22;
			djumpAV = false;
			//�� ��������� ����� ������,�������� � ��������, ��� �� �� �� �����
		}
	}
	void update(float time) //������� "���������" ������� ������. update - ����������. ��������� � ���� ����� SFML , ���������� ���� �������� ����������, ����� ��������� ��������.
	{
		dx = 0;
		control(); //������� ���������� ����������
		switch (state)//��� �������� ��������� �������� � ����������� �� ���������
		{
		case right:
		{
			dir = true;
			dx = speed;
			break;//��������� ���� ������
		}
		case left:
		{
			dir = false;
			dx = -speed;
			break;//��������� ���� ������
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
		x += dx*time;//���� ��������� �� ����� �������� �������� ��������� � ��� ��������� ��������
		checkCollisionWithMap(dx, 0);//������������ ������������ �� X
		y += dy*time;//���������� �� ������
		checkCollisionWithMap(0, dy);//������������ ������������ �� Y
		speed = 0;
		sprite.setPosition(x + w / 4, y + h *0.6);//����� ������� ������� � ������ ��� ������x + w / 2, y + h / 2
		//w/4 ����� ���� ���������� �������������� � ���������� �����
		if (health <= 0) {
			life = false;
			health = 0;
		}//���� ������ ������ ���� ����� 0, �� ����� �������
		dy = dy + 0.0005*time;
		//������ ���������� � ����� ����������, ��� ��� ���� ������� ������ ����� ���������� ground
		//�����, �� ��� ����� � ����� �� ����� "������������" �� ������ ����� ����� ���� �� �������		

	}
	float GetPlayerCoordinateX() {	//���� ������� ���� ���������� �	
		return x;
	}
	float GetPlayerCoordinateY() {	//���� ������� ���� ���������� Y 	
		return y;
	}

	void checkCollisionWithMap(float Dx, float Dy)//� ��� �������� ������������ � ������
	{
		for (int i = y / 32; i < (y + h) / 32; i++)//���������� �� ��������� �����
			for (int j = x / 32; j < (x + w) / 32; j++)
			{
				if (TileMap[i][j] == '0')//���� ������� ��� ������ �����? ��
				{
					if (Dy > 0) {
						y = i * 32 - h;  dy = 0; onGround = true;
					}//�� Y ����=>���� � ���(����� �� �����) ��� ������. � ���� ������ ���� ���������� ��������� � ��������� ��� �� �����, ��� ���� ������� ��� �� �� ����� ��� ����� ����� ����� �������
					if (Dy < 0) { y = i * 32 + 32;  dy = 0; }//������������ � �������� ������ �����(����� � �� �����������)
					if (Dx > 0) { x = j * 32 - w; }//� ������ ����� �����
					if (Dx < 0) { x = j * 32 + 32; }// � ����� ����� �����
				}
				//�������������� � ������
				if (TileMap[i][j] == 'c') {
					playerScore++;//���� ����� �������, ���������� playerScore=playerScore+1;
					TileMap[i][j] = ' ';
				}
				if (TileMap[i][j] == 'f') {
					health -= 40;//���� ��������� �� �����,�� ���������� health=health-40;
					TileMap[i][j] = ' ';//������ �����
				}
				if (TileMap[i][j] == 'j') {
					health -= 40;//���� ��������� �� �����,�� ���������� health=health-40;
					TileMap[i][j] = ' ';//������ �����
				}
				if (TileMap[i][j] == 'h') {
					health += 20;//���� ����� ����� � �����,�� ���������� health=health+20;
					TileMap[i][j] = ' ';//������ ��������
				}
				if (TileMap[i][j] == 'E') {
					life = false;//��������� ����

				}
			}
	}
};
bool startGame()
{
//��������� ������
	RenderWindow window(sf::VideoMode(1024, 768), "Jojo");
	view.reset(FloatRect(0, 0, 768, 576)); //������ "����" ������ ��� �������� ������� ���� ������ (����� ����� �������������)
	//����� ��������� ������ ��� ������ �� ������ �� ������
	Font font_viv_hp;//����� 
	font_viv_hp.loadFromFile("CyrilicOld.ttf");//�������� ������ ������ ���� ������
	Text text_vivod_hp("", font_viv_hp, 20);//������� ������ �����. ���������� � ������ ����� ������, �����, ������ ������(� ��������)//��� ������ ����� (�� ������)
	text_vivod_hp.setColor(Color::Red);//��������� ����� � �������. �� ��������� �� �����
	text_vivod_hp.setStyle(Text::Bold);
	//����� ��������� ������ �� ������ ������ ���������� ��������� �����
	Font font_mis;//����� 
	font_mis.loadFromFile("CyrilicOld.ttf");//�������� ������ ������ ���� ������
	Text text_mis("", font_mis, 15);//������� ������ �����. ���������� � ������ ����� ������, �����, ������ ������(� ��������)//��� ������ ����� (�� ������)
	text_mis.setColor(Color::Black);//��������� ����� � �������. �� ��������� �� �����
	text_vivod_hp.setStyle(Text::Bold);
	Image map_image;//������ ����������� ��� �����
	map_image.loadFromFile("images/Castle2.png");//��������� ���� ��� �����
	Texture map;//�������� �����
	map.loadFromImage(map_image);//��������� �������� ���������
	Sprite s_map;//������ ������ ��� �����
	s_map.setTexture(map);//�������� �������� ��������
	Map m("mapbgBezk.png", 32, 32, 1280, 928);//������ ��� ��� �����: ������� � 1 ����, � �� 0
	//����� �� ����� �������� ������� ����� �������� �� ����� � ��������
	Image quest_image;
	quest_image.loadFromFile("images/missionbg.jpg");
	quest_image.createMaskFromColor(Color(0, 0, 0));
	Texture quest_texture;
	quest_texture.loadFromImage(quest_image);
	Sprite s_quest;
	s_quest.setTexture(quest_texture);
	s_quest.setTextureRect(IntRect(0, 0, 340, 510));  //���������� �����, ������� �������� ��������
	s_quest.setScale(0.7f, 0.7f);//���� ��������� ��������, => ������ ���� ������
	Player p("hero.png", 160, 830, 30, 59);
	//68 118
	//34 59
	bool showMissionText = true;//���������� ����������, ���������� �� ��������� ������ ������ �� ������
	float  CurrentFrame = 0;//������ ������� ����
	Clock clock; //������ ���������� �������. �.�. �������� �� ������� (� �� �������������\�������� ����������)
	Clock gameTimeClock;//���������� �������� �������, ����� ����� ������� ����� ����
	int gameTime = 0;
	while (window.isOpen())
	{
		if (Keyboard::isKeyPressed(Keyboard::R)) { return true; }//���� ���, �� ������������� ����
		if (Keyboard::isKeyPressed(Keyboard::Escape)) { return false; }//���� ������, �� ������� �� ����
		float time = clock.getElapsedTime().asMicroseconds(); //���� ������� ����� � �������������
		if (p.life)
		{
			gameTime = gameTimeClock.getElapsedTime().asSeconds();
		}		//������� ����� � �������� ��� ����� ���� ��� �����
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
		time = time / 800; //����� ������, � ����������� �� ���� ��������� ����� ������������� ���������������� ���� (��� ������ �� �� ��� �����, ��� ����� ������������������ ����), �� � ������������ �������
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == Event::KeyPressed)//������� ������� �������
				if ((event.key.code == Keyboard::Tab)) {//���� ������� ���
					switch (showMissionText) {//�������������, ����������� �� ���������� ���������� showMissionText
					case true: {
						showMissionText = false;//��� ������ ��������� ������ ��� ��� �� ������ �� ������
						break;//������� , ����� �� ��������� ������� "false" (������� ����)
					}
					case false: {
						text_mis.setString("");//���� �� ������ ������� ���, �� ���� ���� ����� ������
						showMissionText = true;// � ��� ������ ��������� ����� ������ ������� ��� � �������� ����� �� �����
						break;
					}
					}
				}
		}
		////////////////��������////////////////					
		if (p.life)
		{						
			if (p.onGround)
			{
				if ((Keyboard::isKeyPressed(Keyboard::Left)) || (Keyboard::isKeyPressed(Keyboard::Right)))
				{
					if ((Keyboard::isKeyPressed(Keyboard::Left)))
					{
						//���� ������ ������� ������� �����
						p.dir = false;
						CurrentFrame += 0.005*time; //������ ��� ����������� �� "������". ���������� ������� �� 10 �������� ������������ ������� � ��������. ������� 0.005 ����� �������� �������� ��������
						if (CurrentFrame > 10) CurrentFrame -= 10; //���������� �� ������ � ������� �� ��������� ������������. ���� ������ � ���������� ����� - ������������ �����.
						p.sprite.setTextureRect(IntRect(115 * int(CurrentFrame) + 30, 540, 112, 118)); //���������� �� ����������� 
					   //+30 ��������� ����� �� ��������� ����� ���������� �����
					}
					if ((Keyboard::isKeyPressed(Keyboard::Right)))
					{//���� ������ ������� ������� ������
						p.dir = true;
						CurrentFrame += 0.005*time;
						if (CurrentFrame > 10) CurrentFrame -= 10;
						p.sprite.setTextureRect(IntRect(115 * int(CurrentFrame) + 115, 540, -112, 118)); //���������� �� �����������, ��� ��� ���� �������� �����-������ ������ ������
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
			p.update(time);//�������� ������ p ������ Player � ������� ������� sfml, ��������� ����� � �������� ��������� ������� update. ��������� ����� �������� ����� ���������
		}
		getplayercoordinateforview(p.GetPlayerCoordinateX(), p.GetPlayerCoordinateY());// ������� ���������� ������ � ������� ���������� �������
		window.setView(view);//"��������" ������ � ���� sfml
		window.clear();
		window.draw(m.sprite);//������ ���
				/////////////////////////////������ ����������� ��� ����������������� � ������� �������/////////////////////
		for (int i = 0; i < HEIGHT_MAP; i++)
			for (int j = 0; j < WIDTH_MAP; j++)
			{
				if (TileMap[i][j] == 'c')  s_map.setTextureRect(IntRect(34, 258, 27, 27));//�������
				if ((TileMap[i][j] == ' ') || (TileMap[i][j] == '0'))  s_map.setTextureRect(IntRect(0, 0, 0, 0));
				if (TileMap[i][j] == 'f') s_map.setTextureRect(IntRect(351, 195, 32, -32));//�������� ����� ������ ��������
				if (TileMap[i][j] == 'j') s_map.setTextureRect(IntRect(351, 163, 32, 32));//�������� ����� ���� ��������
				if (TileMap[i][j] == 'h') s_map.setTextureRect(IntRect(291, 452, 27, 28));//� �������
				s_map.setPosition(j * 32, i * 32);
				window.draw(s_map);
			}
window.draw(p.sprite);//������ ������ ������� p ������ player
		std::ostringstream playerHealthString;//���������� �������� ������
		std::ostringstream gameTimeString;//���������� ������� ����
		playerHealthString << p.health; //������� � �� ����� ��������, �� ���� ��������� ������
		gameTimeString << gameTime; //������� � �� �����, �� ���� ��������� ������
		text_vivod_hp.setString("��������: " + playerHealthString.str() + "   " + "����� ����: " + gameTimeString.str()); //����� ������ ������
		text_vivod_hp.setPosition(view.getCenter().x - 350, view.getCenter().y - 250);//����� ������� ������
		window.draw(text_vivod_hp);//������ �����
		//����� ���������� ����� � ������
		if (!showMissionText) {
			std::ostringstream playerScoreString;//������ �������� ������
			playerScoreString << p.playerScore; //������� � ������ �������� 
			std::ostringstream task;//������ ������ ������
			task << getTextMission(getCurrentMission(p.GetPlayerCoordinateX(), p.GetPlayerCoordinateY()));//���������� ������� getTextMission (��� ���������� ����� ������), ������� ��������� � �������� ��������� ������� getCurrentMission(������������ ����� ������), � ��� ��� �-��� ��������� � �������� ��������� ������� p.getplayercoordinateX() (��� �-��� ���������� ��� ���������� ������)
			text_mis.setString("���������� ��������� �����: " + playerScoreString.str() + "\n" + task.str());//������
			text_mis.setPosition(view.getCenter().x + 140, view.getCenter().y - 120);//������� ����� ����� ���������� �����
			s_quest.setPosition(view.getCenter().x + 130, view.getCenter().y - 130);//������� ���� ��� �����			
			window.draw(s_quest); window.draw(text_mis); //������ ������ ������ (��� ��� ������ ������). � ����� � �����. ��� ��� �������� �� ���������� ����������, ������� ������ ���� ��������� �� ������� ������� ���
		}		
		window.display();
	}
}
void gameRunning() {//�-��� ������������� ���� , ���� ��� ����������
	if (startGame()) { gameRunning(); }////���� startGame() == true, �� �������� ������ �-��� isGameRunning, ������� � ���� ������� ����� �������� startGame() 
}
int main()
{
	gameRunning();//��������� ������� ����
	return 0;
}