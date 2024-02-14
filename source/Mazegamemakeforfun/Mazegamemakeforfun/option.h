#pragma once
#include <SFML/Graphics.hpp>
#define MAX_OPTION 3

class option
{
private:
	int optionSelected;
	sf::Font font;
	sf::Text Textin[MAX_OPTION];
	sf::Text subject;

public:
	option(float width, float hight);
	void draw(sf::RenderWindow& window);
	void moveUp();
	void moveDown();
	int optionpreesed();

	~option();


};

