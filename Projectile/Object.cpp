#include "Object.h"

Object::Object(sf::Vector3f position, sf::Vector2f velocity)
{
	this->velocity = velocity;
	realPosition = position;

}

sf::Vector2f Object::getVelocity() const
{
	return velocity;
}

sf::Vector2f Object::getPosition() const
{
	return position;
}

sf::Vector3f Object::getRealPosition() const
{
	return realPosition;
}

float Object::getMass() const
{
	return mass;
}

void Object::setVelocity(sf::Vector2f velocity)
{
	this->velocity = velocity;
}

void Object::setPosition(sf::Vector2f position)
{
	this->position = position;
}
