#include "Rocket.h"

Rocket::Rocket(Vector2d realPosition, sf::Vector2f position, sf::Vector2f velocity, float angle) : Object(position, velocity)
{
	this->forward = -sf::Vector2f(cos(angle * 3.14/180), sin(angle* 3.14 / 180));
	this->mass = 100;
	this->realPosition = realPosition;

	this->stages[0].emptyMass = 130000;
	this->stages[1].emptyMass = 40100;
	this->stages[2].emptyMass = 13500;

	this->stages[0].fuelMass = 2290000;
	this->stages[1].fuelMass = 496200;
	this->stages[2].fuelMass = 123000;

	this->stages[0].fuelSpeed = 2560;
	this->stages[1].fuelSpeed = 4130;
	this->stages[2].fuelSpeed = 4130;

	this->stages[0].fuelConsumption = 2290000 / 168;
	this->stages[1].fuelConsumption = 496200 / 360;
	this->stages[2].fuelConsumption = 123000 / 500;

	this->currentStage = 0;
	this->maxStage = 3;

	for (int i = 0; i < maxStage; i++) {
		stages[i].shape.setSize(sf::Vector2f(5, 10));
		stages[i].shape.setPosition(this->position - sf::Vector2f(0, 10 * i));
		stages[i].shape.setFillColor(sf::Color::White);
		stages[i].shape.setRotation(angle - 270);
	}

	font.loadFromFile("arial.ttf");
	text.setFont(font);
	text.setCharacterSize(24);
	text.setPosition(this->position + sf::Vector2f(10, 0));
}

void Rocket::update(float dt, Vector2d gravity)
{
	float speed = sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
	sf::Vector2f drag(0,0);
	double height = sqrt(pow(realPosition.x, 2) + pow(realPosition.y, 2)) - Constant::EarthRadie;
	float d = Constant::calcAirDensity(height);

	if(speed != 0 && d != 0)
	drag = -sf::Vector2f(velocity.x / speed, velocity.y / speed)*(float)(0.5 * d * 0.5 * 1*1*3.14 * speed * speed) / mass;
	
	//Rocket update
	mass = 0;
	for (int i = currentStage; i < maxStage; i++) {
		mass += stages[i].fuelMass + stages[i].emptyMass;
	}

	if (height > 0 || engine) {
		float f = 0;
		if (stages[currentStage].fuelMass > 0) {
			float vMass = stages[currentStage].fuelConsumption * dt;
			if (vMass > stages[currentStage].fuelMass)
				vMass = stages[currentStage].fuelMass;

			stages[currentStage].fuelMass -= vMass;

			f = stages[currentStage].fuelSpeed * vMass;
		}
		else {
			if(currentStage < maxStage - 1)
			currentStage++;
		}
		accelleration = forward * f / mass + (-sf::Vector2f(gravity) + drag) * dt;
		velocity += accelleration * dt;
		realPosition += Vector2d(velocity) * (double)dt;
	}

	//Text update
	text.setPosition(position + sf::Vector2f(10, 0));
	text.setString(
		"v: " + std::to_string(speed) + "\n" +
		"a: " + std::to_string((sqrt(accelleration.x * accelleration.x + accelleration.y * accelleration.y)))
	);
}

void Rocket::draw(sf::RenderWindow & window)
{
	for(int i = currentStage; i < maxStage; i++)
	window.draw(stages[i].shape);

	window.draw(text);
}

void Rocket::toggleEngine()
{
	engine = !engine;
}
