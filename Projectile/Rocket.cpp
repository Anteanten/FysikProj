#include "Rocket.h"

Rocket::Rocket(Vector2d realPosition, sf::Vector2f position, sf::Vector2f velocity, float angle) : Object(position, velocity)
{
	this->forward = sf::Vector2f(cos(angle * 3.14159f / 180.0f), sin(angle * 3.14159f / 180.0f));
	this->mass = 100;
	this->realPosition = realPosition;

	this->stages[0].emptyMass = 130000;
	this->stages[1].emptyMass = 40100;
	this->stages[2].emptyMass = 13500;

	this->stages[0].fuelMass = 2290000;
	this->stages[1].fuelMass = 496200;
	this->stages[2].fuelMass = 123000;

	this->stages[0].fuelSpeed = 2560 * 10;
	this->stages[1].fuelSpeed = 4130 * 10;
	this->stages[2].fuelSpeed = 4130 * 10;

	this->stages[0].fuelConsumption = 2290000 / 168;
	this->stages[1].fuelConsumption = 496200 / 360;
	this->stages[2].fuelConsumption = 123000 / 500;

	this->stages[0].shape.setSize(sf::Vector2f(10.1, 42.1));
	this->stages[1].shape.setSize(sf::Vector2f(10.1, 24.8));
	this->stages[2].shape.setSize(sf::Vector2f(6.6, 18.8));

	this->currentStage = 0;
	this->maxStage = 3;

	for (int i = 0; i < maxStage; i++) {
		stages[i].shape.setPosition(this->position - sf::Vector2f(stages[i].shape.getSize().x/2, stages[i].shape.getSize().y * i));
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
	float r = stages[currentStage].shape.getSize().x;
	double c = 0;

	if (speed < 343)
		c = (((-1) * (0.1 / 343)) * speed) + 0.6;
	else if (speed >= 343 && speed <= 1029)
		c = (((-1) * (0.1 / 686)) * (speed - 343)) + 0.6;
	else
		c = 0.5;
	
	//Rocket update
	mass = 0;
	for (int i = currentStage; i < maxStage; i++) {
		mass += stages[i].fuelMass + stages[i].emptyMass;
	}

	if (speed != 0 && d != 0)
		drag = -sf::Vector2f(velocity.x / speed, velocity.y / speed)*(float)(0.5 * (d * c * r * r * 3.14 * speed * speed)) / mass;

	if (height > 0 || engine) {
		float f = 0;
		if (stages[currentStage].fuelMass > 0) {
			if (engine) {
				float vMass = stages[currentStage].fuelConsumption * dt;
				if (vMass > stages[currentStage].fuelMass)
					vMass = stages[currentStage].fuelMass;

				stages[currentStage].fuelMass -= vMass;

				f = stages[currentStage].fuelSpeed * vMass;
			}
		}
		else {
			if(currentStage < maxStage - 1)
			currentStage++;
		}
		accelleration = forward * f / mass / dt + (-sf::Vector2f(gravity) + drag);
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

float Rocket::getRotation() const
{
	float a = 0;
	if (forward.y > 0) {
		a = acos(forward.x) * 180.0f / 3.141592f;
	}
	else {
		a = -acos(forward.x) * 180.0f / 3.141592f;
	}
	return a;
}

void Rocket::setRotation(float angle)
{
	this->forward = sf::Vector2f(cos(angle * 3.14159f / 180.0f), sin(angle * 3.14159f / 180.0f));
	for (int i = currentStage; i < maxStage; i++) {
		stages[i].shape.setRotation(angle - 270);
	}
}

void Rocket::toggleEngine()
{
	engine = !engine;
}
