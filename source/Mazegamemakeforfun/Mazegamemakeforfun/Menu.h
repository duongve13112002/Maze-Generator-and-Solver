#include <SFML/Graphics.hpp>
#define MAX_MAIN_MENU 4

class Menu
{
private:
	int mainMenuSelected;
	sf::Font font;
	sf::Text mainMenu[MAX_MAIN_MENU];
public:
	Menu(float width , float hight);
	void draw(sf::RenderWindow &window);
	void moveUp();
	void moveDown();
	int Mainmenupreesed();
	~Menu();
};

