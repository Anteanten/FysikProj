#include <SFML/Graphics.hpp>
#include "Rocket.h"
#include <iostream>
#include "constant.h"
#include "Gui.h"

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "SFML works!");
	sf::View view = window.getView();
	Gui gui(window);

	sf::Clock clock;
	float deltaTime;
	float timeCount = 0;
	float secCount = 0;
	bool lockToRocket = false;

	sf::Font font;
	font.loadFromFile("arial.ttf");

	Rocket rocket(sf::Vector3f(0, 0, Constant::EarthRadie), sf::Vector2f(0, 0), 5000, sf::Vector2f(0, -1));
	float gravity = Constant::calcGravity(Constant::EarthMass, rocket.getRealPosition(), sf::Vector3f(0, 0, 0));

	gui.addElement(&gravity, "Gravity: ", &font);
	float h = 0;
	gui.addElement(&h, "Height: ", &font);
	while (window.isOpen())
	{
		window.clear();

		//INPUT

		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type) {
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
		deltaTime = clock.restart().asSeconds() * 5;
		timeCount += deltaTime;
		secCount += deltaTime;

		//Update
		rocket.update(deltaTime, gravity);

		h = rocket.getRealPosition().z - Constant::EarthRadie;

		if (lockToRocket) {
			view.setCenter(rocket.getPosition());
			window.setView(view);
			lockToRocket = false;
		}
		gui.update(window.getView());
		timeCount = 0;

		//Time Update
		if (secCount >= 1) {
			gravity = Constant::calcGravity(Constant::EarthMass, rocket.getRealPosition(), sf::Vector3f(0, 0, 0));
		}

		//Render
		rocket.draw(window);
		gui.draw(window);

		window.display();
	}

	return 0;
}