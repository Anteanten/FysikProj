#ifndef OBJECT_H
#define OBJECT_H

#include <SFML\Graphics.hpp>
#include "Constant.h"

typedef sf::Vector2<double> Vector2d;

class Object {
protected:
	float speed;

	sf::Vector2f velocity;
	sf::Vector2f position;
	Vector2d realPosition;
	sf::Vector2f accelleration;
	float mass;


public:
	Object(sf::Vector2f position, sf::Vector2f velocity);

	virtual void update(float dt, Vector2d gravity) = 0;
	virtual void draw(sf::RenderWindow &window) = 0;

	sf::Vector2f getVelocity() const;
	sf::Vector2f getAcceleration() const;
	sf::Vector2f getPosition() const;
	Vector2d getRealPosition() const;
	float getMass() const;



	void setVelocity(sf::Vector2f velocity);
	void setPosition(sf::Vector2f position);
	

};

#endif // !OBJECT_H
