#include <SFML/Graphics.hpp>
using namespace sf;
View view; //�������� smfl ������ "���", ������� � �������� �������

View getplayercoordinateforview(float x, float y) { //������� ��� ���������� ��������� ������
	float tempX = x;
	float tempY = y; //��������� ���������� ������ � ��������� ��, ����� ������ ����	
	if (x < 416) tempX = 416;//������� �� ���� ����� ������� "�������"
	if (x > 925) tempX = 925;//������� �� ���� ������ ������� "�������" 
	if (y < 340) tempY = 340;//�������
	if (y > 650) tempY = 650;//������
	view.setCenter(tempX, tempY);//������ �� �������, ��������� ��� ����������
	//������ ��� �������� ���� ���������, ����� �� ��� �������� �����-����� ��� � ������������
	return view;
}