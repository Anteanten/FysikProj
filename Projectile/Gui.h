#ifndef GUI_H
#define GUI_H

#include <SFML/Graphics.hpp>
#include <iomanip>
#include <sstream>

#define ELEMENT_COLUMNS 6

class Gui {
private:

	struct Element {
		float *data;
		std::string descString;
		sf::Text text;
		sf::RectangleShape eWindow;
	};

	sf::RectangleShape gWindow;
	std::vector<Element> elements;

public:
	Gui(sf::RenderWindow &window);

	void update(sf::View view);

	void addElement(float *data, std::string descString, sf::Font *font);

	void draw(sf::RenderWindow &window);
};


#endif