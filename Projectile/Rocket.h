#ifndef ROCKET_H
#define ROCKET_H
#include "Object.h"
#include <math.h>
#include <iostream>
#include "Constant.h"

class Rocket : public Object {
private:
	struct Stage {
		double emptyMass;
		double fuelMass;
		float fuelConsumption;
		double fVac;
		double fSea;
		sf::RectangleShape shape;
	};
	bool engine = false;

	sf::Vector2f forward;

	sf::Text text;
	sf::Font font;
	
	sf::Transform transform;

	Stage stages[4];
	int currentStage;
	int maxStage;

	sf::Vector2f c;

	double apoapsis, periapsis;
	double orbitalPeriod;

	void predictOrbit(Vector2d gravity);

public:
	Rocket(Vector2d realPosition, sf::Vector2f winPosition, sf::Vector2f velocity, float angle);

	// (Delta Time, g)
	void update(float dt, Vector2d gravity);
	void draw(sf::RenderWindow &window);

	float getVelocityUp(Vector2d gravity) const;
	float getVelocitySide(Vector2d gravity) const;

	float getApoapasis() const;
	float getPeriapsis() const;
	float getOrbitalPeriod() const;

	float getRotation() const;
	void setRotation(float angle);

	void toggleEngine();
};

#endif // !ROCKET_H
