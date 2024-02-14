#include "Menu.h"

Menu::Menu(float width, float hight)
{
	if (!font.loadFromFile("arial.ttf"))
	{

	}
	//play
	mainMenu[0].setFont(font);
	mainMenu[0].setFillColor(sf::Color::Red);
	mainMenu[0].setString("Play");
	mainMenu[0].setCharacterSize(30);
	mainMenu[0].setPosition(sf::Vector2f(width / 2 -250, hight / (MAX_MAIN_MENU + 1)+120));
	//infor
	mainMenu[1].setFont(font);
	mainMenu[1].setFillColor(sf::Color::White);
	mainMenu[1].setString("Options");
	mainMenu[1].setCharacterSize(30);
	mainMenu[1].setPosition(sf::Vector2f(width / 2 - 250, hight / (MAX_MAIN_MENU + 1)*2+60));
	//options
	mainMenu[2].setFont(font);
	mainMenu[2].setFillColor(sf::Color::White);
	mainMenu[2].setString("Infor");
	mainMenu[2].setCharacterSize(30);
	mainMenu[2].setPosition(sf::Vector2f(width / 2 - 250, hight / (MAX_MAIN_MENU + 1) * 3));
	//exit
	mainMenu[3].setFont(font);
	mainMenu[3].setFillColor(sf::Color::White);
	mainMenu[3].setString("Exit");
	mainMenu[3].setCharacterSize(30);
	mainMenu[3].setPosition(sf::Vector2f(width / 2 - 250, hight / (MAX_MAIN_MENU + 1) * 4-50));

	mainMenuSelected = 0;
}
//Funtion to draw the Menu
void Menu::draw(sf::RenderWindow& window)
{
	for (int i = 0; i < MAX_MAIN_MENU; i++)
	{
		window.draw(mainMenu[i]);
	}
}
void Menu::moveUp()
{
	if (mainMenuSelected - 1 >= -1)  // check it is not the frist item(play)
	{
		mainMenu[mainMenuSelected].setFillColor(sf::Color::White);  // change the previous item's Color

		mainMenuSelected--;  // move to the upper item
		if (mainMenuSelected == -1)
		{
			mainMenuSelected = 3;
		}
		mainMenu[mainMenuSelected].setFillColor(sf::Color::Red);   // Change the new item's color
	}
}
//Move Down
void Menu::moveDown()
{
	if (mainMenuSelected + 1 <= MAX_MAIN_MENU)  // check it is not the last item(exit)
	{
		mainMenu[mainMenuSelected].setFillColor(sf::Color::White);  // change the previous item's Color

		mainMenuSelected++;  // move to the lower item
		if (mainMenuSelected == MAX_MAIN_MENU)
		{
			mainMenuSelected = 0;
		}
		mainMenu[mainMenuSelected].setFillColor(sf::Color::Red);   // Change the new item's color
	}
}

int Menu::Mainmenupreesed()
{
	return mainMenuSelected;
}



Menu::~Menu()
{
}
