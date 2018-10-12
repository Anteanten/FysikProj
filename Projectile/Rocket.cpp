#include "Rocket.h"

Rocket::Rocket(Vector2d realPosition, sf::Vector2f position, sf::Vector2f velocity, float thrust, sf::Vector2f forward) : Object(position, velocity)
{
	this->thrust = thrust;
	this->forward = forward;
	this->mass = 100;
	this->realPosition = realPosition;

	shape.setPosition(this->position);
	shape.setSize(sf::Vector2f(5, 10));
	shape.setFillColor(sf::Color::White);

	font.loadFromFile("arial.ttf");
	text.setFont(font);
	text.setCharacterSize(24);
	text.setPosition(this->position + sf::Vector2f(10, 0));
}

void Rocket::update(float dt, Vector2d gravity)
{
	float speed = sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
	sf::Vector2f drag(0,0);
	float d = Constant::calcAirDensity(sqrt(pow(realPosition.x, 2) + pow(realPosition.y, 2)) - Constant::EarthRadie);
	if(speed != 0 && d != 0)
	drag = -sf::Vector2f(velocity.x / speed, velocity.y / speed)*(float)(0.5 * d * 0.5 * 1*1*3.14 * speed * speed) / mass;
	
	//Rocket update
	float f = thrust;
	if (!engine)
		f = 0;

	accelleration = forward * f / mass - sf::Vector2f(gravity) + drag;
	velocity += accelleration * dt;
	realPosition += Vector2d(velocity) * (double)dt;

	//Text update
	text.setPosition(position + sf::Vector2f(10, 0));
	text.setString(
		"v: " + std::to_string(speed) + "\n" +
		"a: " + std::to_string((sqrt(accelleration.x * accelleration.x + accelleration.y * accelleration.y)))
	);
}

void Rocket::draw(sf::RenderWindow & window)
{
	window.draw(shape);
	window.draw(text);
}

void Rocket::toggleEngine()
{
	engine = !engine;
}
