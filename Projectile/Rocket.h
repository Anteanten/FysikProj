#ifndef ROCKET_H
#define ROCKET_H
#include "Object.h"
#include <math.h>
#include <iostream>
#include "Constant.h"

class Rocket : public Object {
private:
	bool engine = true;

	float thrust;
	sf::Vector2f forward;

	sf::RectangleShape shape;
	sf::Text text;
	sf::Font font;

	sf::Vector2f c;
public:
	Rocket(Vector2d realPosition, sf::Vector2f winPosition, sf::Vector2f velocity, float thrust, sf::Vector2f forward);

	// (Delta Time, g)
	void update(float dt, Vector2d gravity);
	void draw(sf::RenderWindow &window);

	void toggleEngine();
};

#endif // !ROCKET_H
