#include "Infor.h"

Infor::Infor(float width, float hight)
{
	if (!font.loadFromFile("arial.ttf"))
	{

	}
	//
	Textin[0].setFont(font);
	Textin[0].setFillColor(sf::Color::White);
	Textin[0].setString("This is the first time we have made this project.\nIf it has some bugs, Please contact to us to fix them.Thanks");
	Textin[0].setCharacterSize(30);
	Textin[0].setPosition(sf::Vector2f(width / 2 - 440, hight / (MAX_INFOR + 1) -40));
	//Tutorial
	Textin[1].setFont(font);
	Textin[1].setFillColor(sf::Color::White);
	Textin[1].setString("Tutorial :Click M/m to create a random maze then click S/s to solve it\nIn Option: Chose data you want to change after that click Enter after\n                 that write the number you want and click Escape to back\n\nSometime when you click enter in option menu If window is not changed ,\nPlease choose the option you want to change and press enter again.");
	Textin[1].setCharacterSize(30);
	Textin[1].setPosition(sf::Vector2f(width / 2 - 440, hight / (MAX_INFOR + 1) * 2 -80));
	//End
	Textin[2].setFont(font);
	Textin[2].setFillColor(sf::Color::White);
	Textin[2].setString("\n\n\n\n\nThis project has been developed by :\nPHAN DONG TEAM\nUniversity of Information Technology");
	Textin[2].setCharacterSize(30);
	Textin[2].setPosition(sf::Vector2f(width / 2 - 440, hight / (MAX_INFOR + 1) * 3 - 150));
}

void Infor::draw(sf::RenderWindow& window)
{
	for (int i = 0; i < 3; i++)
	{
		window.draw(Textin[i]);
	}
}

Infor::~Infor()
{

}
