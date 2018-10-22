#include "Rocket.h"

void Rocket::predictOrbit(Vector2d gravity)
{
	float h = realPosition.x * velocity.y - realPosition.y * velocity.x;
	float speed = sqrt(pow(velocity.x, 2) + pow(velocity.y, 2));
	double mu = Constant::EarthMass * Constant::GravityConstant;
	double height = Constant::calcDistance(realPosition, Vector2d(0, 0));
	Vector2d e = ((pow(speed, 2) - mu / height) * realPosition - (realPosition.x * velocity.x + realPosition.y * velocity.y) * Vector2d(velocity)) / mu;
	double ee = sqrt(pow(e.x, 2) + pow(e.y, 2));
	double energy = pow(speed, 2) / 2 - mu / height;
	double a = -mu / (2 * energy);
	orbitalPeriod = 2 * 3.1415 * sqrt(pow(a, 3) / mu);
	apoapsis = a * (1 + ee);
	periapsis = a * (1 - ee);
}

Rocket::Rocket(Vector2d realPosition, sf::Vector2f position, sf::Vector2f velocity, float angle) : Object(position, velocity)
{
	this->forward = sf::Vector2f(cos(angle * 3.14159f / 180.0f), sin(angle * 3.14159f / 180.0f));
	this->mass = 0;
	this->realPosition = realPosition;

	this->stages[0].emptyMass = 130000;
	this->stages[1].emptyMass = 40100;
	this->stages[2].emptyMass = 13500;
	this->stages[3].emptyMass = 10000;
	

	this->stages[0].fuelMass = 2990000;
	this->stages[1].fuelMass = 496200;
	this->stages[2].fuelMass = 133000;
	this->stages[3].fuelMass = 0;

	this->stages[0].fuelConsumption = 2578 * 6;
	this->stages[1].fuelConsumption = 276 * 5;
	this->stages[2].fuelConsumption = 276;
	this->stages[3].fuelConsumption = 0;
	
	this->stages[0].fVac = 7770 * 1000 * 6;
	this->stages[1].fVac = 1033.1 * 1000 * 5;
	this->stages[2].fVac = 1033.1 * 1000 * 1;
	this->stages[3].fVac = 0;

	this->stages[0].fSea = 6770 * 1000 * 6;
	this->stages[1].fSea = 486.2 * 1000 * 5;
	this->stages[2].fSea = 486.2 * 1000 * 1;
	this->stages[3].fSea = 0;

	this->stages[0].shape.setSize(sf::Vector2f(10.1, 42.1));
	this->stages[1].shape.setSize(sf::Vector2f(10.1, 24.8));
	this->stages[2].shape.setSize(sf::Vector2f(6.6, 18.8));
	this->stages[3].shape.setSize(sf::Vector2f(3.3, 6.0));

	this->currentStage = 0;
	this->maxStage = 4;

	transform.rotate(angle + 90, position);

	stages[0].shape.setPosition(position);

	for (int i = 1; i < maxStage; i++) {
		stages[i].shape.setPosition(stages[i-1].shape.getPosition() - sf::Vector2f((stages[i].shape.getSize().x - stages[i-1].shape.getSize().x)/2, stages[i].shape.getSize().y));
		stages[i].shape.setFillColor(sf::Color(255, 255 * i / maxStage, 255 * i / maxStage, 255));
	}



	font.loadFromFile("arial.ttf");
	text.setFont(font);
	text.setCharacterSize(24);
	text.setPosition(this->position + sf::Vector2f(10, 0));
}

void Rocket::update(float dt, Vector2d gravity)
{
	speed = sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
	double height = sqrt(pow(realPosition.x, 2) + pow(realPosition.y, 2)) - Constant::EarthRadie;

	//Air Resitance Values
	sf::Vector2f drag(0, 0);
	float d = Constant::calcAirDensity(height);
	float r = stages[currentStage].shape.getSize().x;
	double c = 0;

	//Drag Coefficent
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

	//Drag
	if (speed != 0 && d != 0)
		drag = -sf::Vector2f(velocity.x / speed, velocity.y / speed)*(float)(0.5 * (d * c * r * r * 3.14 * speed * speed)) / mass;

	if (height > 0 || engine) {
		float f = 0;
		float p = Constant::calcPressure(height);

		if (stages[currentStage].fuelMass > 0) {
			if (engine) {
				float vMass = stages[currentStage].fuelConsumption * dt;
				if (vMass > stages[currentStage].fuelMass)
					vMass = stages[currentStage].fuelMass;

				stages[currentStage].fuelMass -= vMass;

				/*f = stages[currentStage].fuelSpeed * vMass;*/
				f = stages[currentStage].fVac - (stages[currentStage].fVac - stages[currentStage].fSea) * (p / Constant::pSea);
			}
		}
		else {
			if(currentStage < maxStage - 1)
			currentStage++;
		}
		accelleration = forward * f / mass + (-sf::Vector2f(gravity) + drag);
		velocity += accelleration * dt;
		realPosition += Vector2d(velocity) * (double)dt;

		predictOrbit(gravity);
	}
}

void Rocket::draw(sf::RenderWindow & window)
{
	for(int i = currentStage; i < maxStage; i++)
	window.draw(stages[i].shape, transform);

	window.draw(text);
}

float Rocket::getVelocityUp(Vector2d gravity) const
{
	return velocity.x * gravity.x + velocity.y * gravity.y;
}

float Rocket::getVelocitySide(Vector2d gravity) const
{
	sf::Vector2f g = sf::Vector2f(gravity.y, -gravity.x);
	return abs(velocity.x * g.x + velocity.y * g.y);
}

float Rocket::getApoapasis() const
{
	return apoapsis;
}

float Rocket::getPeriapsis() const
{
	return periapsis;
}

float Rocket::getOrbitalPeriod() const
{
	return orbitalPeriod;
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
	transform = sf::Transform::Identity;
	transform.rotate(angle + 90, position);
}

void Rocket::toggleEngine()
{
	engine = !engine;
}
