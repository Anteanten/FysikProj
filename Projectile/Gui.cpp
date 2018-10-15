#include "Gui.h"

Gui::Gui(sf::RenderWindow &window)
{
	gWindow.setPosition(sf::Vector2f(window.getView().getCenter().x - window.getSize().x/2, window.getSize().y * 2 / 3));
	gWindow.setSize(sf::Vector2f(window.getSize().x, window.getSize().y/3));
	gWindow.setFillColor(sf::Color(128, 128, 128, 255));
}

void Gui::update(sf::View view)
{
	for (int i = 0; i < elements.size(); i++) {
		std::stringstream str;
		str << elements[i].descString << std::setprecision(5) << *elements[i].data;
		elements[i].text.setString(str.str());
	}
}

void Gui::addElement(float *data, std::string descString, sf::Font *font)
{
	Element tmp;
	tmp.data = data;
	tmp.descString = descString;
	sf::Vector2f pos = gWindow.getPosition() - sf::Vector2f(-20 - gWindow.getSize().x * (elements.size() % ELEMENT_COLUMNS) / ELEMENT_COLUMNS, -20 - gWindow.getSize().y * floor(elements.size() / ELEMENT_COLUMNS) / ELEMENT_COLUMNS);
	tmp.eWindow.setPosition(pos);
	tmp.eWindow.setFillColor(sf::Color(0, 0, 0, 255));
	tmp.eWindow.setSize(sf::Vector2f(130, 20));
	tmp.text.setPosition(tmp.eWindow.getPosition());
	tmp.text.setFont(*font);
	tmp.text.setCharacterSize(15);
	std::stringstream str;
	str << descString << std::setprecision(2) << *data;
	tmp.text.setString(str.str());

	elements.push_back(tmp);
}

void Gui::draw(sf::RenderWindow & window)
{
	window.draw(gWindow);
	for (int i = 0; i < elements.size(); i++) {
		window.draw(elements[i].eWindow);
		window.draw(elements[i].text);
	}
}
