#include <SFML/Graphics.hpp>
#include "Rocket.h"
#include <iostream>
#include "constant.h"

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "SFML works!");
	sf::View view = window.getView();
	view.setCenter(sf::Vector2f(0, 0));
	window.setView(view);

	sf::Clock clock;
	float deltaTime;
	float timeCount = 0;
	float secCount = 0;
	bool lockToRocket = false;

	sf::Text gravityText;
	sf::Font font;
	font.loadFromFile("arial.tff");
	gravityText.setFont(font);
	gravityText.setPosition(sf::Vector2f(10, 10));

	Rocket rocket(sf::Vector3f(0, 0, Constant::EarthRadie), sf::Vector2f(0, 0), 10000, sf::Vector2f(0, -1));
	float gravity = Constant::calcGravity(Constant::EarthMass, rocket.getRealPosition(), sf::Vector3f(0,0,0));
	while (window.isOpen())
	{
		window.clear();
		
		//INPUT

		sf::Event event;
		while (window.pollEvent(event))
		{
			switch(event.type) {
				//Window X Button, cloase window
				case sf::Event::Closed:
					window.close();
					break;
				//MouseScroll, Zoom window
				case sf::Event::MouseWheelScrolled:
					if (event.mouseWheel.x > 0) {
						view.setSize(view.getSize() * (float)1.01);
					}
					else {
						view.setSize(view.getSize() * (float)0.99);
					};
					window.setView(view);
					break;
				//WASD, Move camera
				case sf::Event::KeyPressed:
					if (event.key.code == sf::Keyboard::W) {
						view.setCenter(view.getCenter() + sf::Vector2f(0, -5));
					}
					else if (event.key.code == sf::Keyboard::S) {
						view.setCenter(view.getCenter() + sf::Vector2f(0, 5));
					}
					else if (event.key.code == sf::Keyboard::A) {
						view.setCenter(view.getCenter() + sf::Vector2f(-5, 0));
					}
					else if (event.key.code == sf::Keyboard::D) {
						view.setCenter(view.getCenter() + sf::Vector2f(5, 0));
					}
					else if (event.key.code == sf::Keyboard::L) {
						lockToRocket = !lockToRocket;
					}
					window.setView(view);
					break;
			}
		}

		//Main Loop
		deltaTime = clock.restart().asSeconds();
		timeCount += deltaTime;
		secCount += deltaTime;

		//Update
		rocket.update(deltaTime, gravity);
		if (lockToRocket) {
			view.setCenter(rocket.getPosition());
			gravityText.setPosition(sf::Vector2f(window.getPosition() - sf::Vector2i(10, 10)));
			window.setView(view);
		}

		//Time Update
		if (secCount >= 1) {
			gravity = Constant::calcGravity(Constant::EarthMass, rocket.getRealPosition(), sf::Vector3f(0, 0, 0));
			gravityText.setString("Gravity: " + std::to_string(gravity));
		}

		//Render
		rocket.draw(window);

		window.display();
	}

	return 0;
}