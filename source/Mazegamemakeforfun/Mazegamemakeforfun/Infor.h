#pragma once
#include <SFML/Graphics.hpp>
#define MAX_INFOR 3
class Infor
{
private:
	sf::Font font;
	sf::Text Textin[MAX_INFOR];
public:
	Infor(float width, float hight);
	void draw(sf::RenderWindow& window);
	
	~Infor();

};

