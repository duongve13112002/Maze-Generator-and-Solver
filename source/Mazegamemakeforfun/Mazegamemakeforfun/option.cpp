#include "option.h"

option::option(float width, float hight)
{
	if (!font.loadFromFile("arial.ttf"))
	{

	}
	subject.setFont(font);
	subject.setFillColor(sf::Color::Green);
	subject.setString("Change data the Maze");
	subject.setCharacterSize(50);
	subject.setPosition(sf::Vector2f(width / 2 - 250, hight / (MAX_OPTION + 1)));

	Textin[0].setFont(font);
	Textin[0].setFillColor(sf::Color::Red);
	Textin[0].setString("Set Columns");
	Textin[0].setCharacterSize(30);
	Textin[0].setPosition(sf::Vector2f(width / 2 -100 , hight / (MAX_OPTION + 1)*2 ));

	Textin[1].setFont(font);
	Textin[1].setFillColor(sf::Color::White);
	Textin[1].setString("Set Rows ");
	Textin[1].setCharacterSize(30);
	Textin[1].setPosition(sf::Vector2f(width / 2 - 100, hight / (MAX_OPTION + 1) * 3 - 80));

	Textin[2].setFont(font);
	Textin[2].setFillColor(sf::Color::White);
	Textin[2].setString("Return Menu");
	Textin[2].setCharacterSize(30);
	Textin[2].setPosition(sf::Vector2f(width / 2 - 100, hight / (MAX_OPTION + 1) * 4 - 150));
	optionSelected = 0;

}

void option::draw(sf::RenderWindow& window)
{
	window.draw(subject);
	for (int i = 0; i < 3; i++)
	{
		window.draw(Textin[i]);
	}
}

void option::moveUp()
{
	if (optionSelected - 1 >= -1)  // check it is not the frist item(play)
	{
		Textin[optionSelected].setFillColor(sf::Color::White);  // change the previous item's Color

		optionSelected--;  // move to the upper item
		if (optionSelected == -1)
		{
			optionSelected = 2;
		}
		Textin[optionSelected].setFillColor(sf::Color::Red);   // Change the new item's color
	}
}

void option::moveDown()
{
	if (optionSelected + 1 <= MAX_OPTION)  // check it is not the last item(exit)
	{
		Textin[optionSelected].setFillColor(sf::Color::White);  // change the previous item's Color

		optionSelected++;  // move to the lower item
		if (optionSelected == MAX_OPTION)
		{
			optionSelected = 0;
		}
		Textin[optionSelected].setFillColor(sf::Color::Red);   // Change the new item's color
	}
}

int option::optionpreesed()
{
	return optionSelected;
}

option::~option()
{

}
