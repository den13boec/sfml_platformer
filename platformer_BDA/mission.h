///////////////////////////////////����� ������//////////////////////////////////
int getCurrentMission(int x,int y)//�-��� ������ ������, ������� ������ ����� ������, � ����������� �� ���������� ������ � (���� ����� ���������� ��� ����������)
{
	int mission = 0;
	if ((x>0) && (x<500) && (y> 760) && (y< 900)) { mission = 0; } //�������� ������ � �����
	if ((x>0) && (x<680) && (y> 540) && (y< 780)) { mission = 1; } //����� �� ������ ������
	if ((x>0) && (x<480) &&  (y< 595)) { mission = 2; }//2��
	if ((x>490) && ((y< 420))) { mission = 3; }//� ��
	return mission;//�-��� ���������� ����� ������
}
/////////////////////////////////////����� ������/////////////////////////////////
std::string getTextMission(int currentMission) {
	std::string missionText = "";//����� ������ � ��� �������������
	switch (currentMission)//����������� ����� ������ � � ����������� �� ���� ���������� missionText ������������� ��������� �����
	{
	case 0: missionText = "\n\n������� �� ����������\n�������� �� �����������,\n�� ����� ������\n������� ������,\n����� ������� ������� ������\nR-������������� ����\nEsc-�����"; break;
	case 1: missionText = "\nMission 1\n������ ������������\n �������.\n�� �������, ���\n �� �����������������\n������� � ����� \n� ����������\n�������� ������"; break;
	case 2: missionText = "\nMission 2\n�������� ��������� �� �����\n��� ���� �������"; break;
	case 3: missionText = "\nMission 3\n��� ����������\n����� ���� �������"; break;
	}
	return missionText;//�-��� ���������� �����
};