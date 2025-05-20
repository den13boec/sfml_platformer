#include <SFML\Graphics.hpp>
const int HEIGHT_MAP = 31;//размер карты высота
const int WIDTH_MAP = 41;//размер карты ширина 
using namespace sf;
/////////////////////////////МЕТОДЫ КАРТЫ////////////////////////////
struct Map {

	Image image;
	Texture texture;
	Sprite sprite;
	String File;
	int x, y, w, h;

	//////////////////////////КОНСТРУКТОР КАРТЫ///////////////////////
	Map(String file, int X, int Y, int Width, int Height) {
		File = file;
		x = X; y = Y; w = Width; h = Height;
		image.loadFromFile("images/" + File);
		texture.loadFromImage(image);
		sprite.setTexture(texture);
		sprite.setTextureRect(IntRect(0, 0, w, h));
		sprite.setPosition(x, y);
	}


};

sf::String TileMap[HEIGHT_MAP] = {
	"000000000000000000000000000000000000000000",
	"0                                        0",
	"0                                        0",
	"0                                        0",
	"0                                        0",
	"0                                        0",
	"0                                        0",
	"0                                        0",
	"0                                        0",
	"0                                        0",
	"0  0 cc0                                 0",
	"0   000  0ff0                            0",
	"0              0                         0",
	"0             0 0           f0000        0",
	"0            0   00        00            0",
	"0            0            0 0 c          0",
	"0  0000  h 000           0j00000000      0",
	"0      0000  0                   00      0",
	"00           0c    h           c 00     E0",
	"0             0000000000    00000  0f00000",
	"0               jj    0 00   0      000  0",
	"00cc00                0  0   0000000   0 0",
	"0000  0     h         0  00            0 0",
	"0     0 c0  0        c0  0             0 0",
	"0     0000ff0f0000   00  0            c0 0",
	"0         00 0      0    0  0 h   c   0  0",
	"0                  0     0   000000000   0",
	"0                 0      00  0           0",
	"0000000000000000000      0  c0           0",
	"                          000             ",
	"                                          ",
};