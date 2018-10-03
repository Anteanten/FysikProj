#ifndef ROCKET_H
#define ROCKET_H
#include "Object.h"
#include <math.h>
#include <iostream>

class Rocket : public Object {
private:
	float thrust;
	sf::Vector2f forward;

	sf::RectangleShape shape;
	sf::Text text;
	sf::Font font;
public:
	Rocket(sf::Vector3f realPosition, sf::Vector2f velocity, float thrust, sf::Vector2f forward);

	// (Delta Time, g)
	void update(float dt, float gravity);
	void draw(sf::RenderWindow &window);
};

#endif // !ROCKET_H
