#include <SFML/Graphics.hpp>
#include "Rocket.h"
#include <iostream>
#include "constant.h"
#include "Gui.h"

//Use this for calculations as it is double instead of float
typedef sf::Vector2<double> Vector2d;

int main()
{
	sf::RenderWindow window(sf::VideoMode(1024, 768), "Projectiles!");
	sf::View view = window.getView();
	Gui gui(window);

	sf::Clock clock;
	float deltaTime;
	float timeCount = 0;
	float secCount = 0;
	int fps = 0;
	bool lockToRocket = false;

	sf::Font font;
	font.loadFromFile("arial.ttf");

	//RealPosition, Window Position, Velocity, Thrust, Rotation
	Rocket rocket(Vector2d((double)0, Constant::EarthRadie), sf::Vector2f(300, 300), sf::Vector2f(0, 0), 50000, sf::Vector2f(1/sqrt(2), 1/sqrt(2)));
	Vector2d gravity = Constant::calcGravity(Constant::EarthMass, rocket.getRealPosition(), Vector2d(0, 0));


	//Map Shapes
	sf::CircleShape earth;
	earth.setPosition(600, 200);
	earth.setRadius(Constant::EarthRadie / pow(10, 5));
	earth.setFillColor(sf::Color::Blue);

	sf::CircleShape mapRocket;
	mapRocket.setPosition((earth.getPosition().x + earth.getRadius()) - (rocket.getRealPosition().x / pow(10, 5)), (earth.getPosition().y + earth.getRadius()) - (rocket.getRealPosition().y / pow(10, 5)));
	mapRocket.setRadius(5);
	mapRocket.setFillColor(sf::Color::Red);

	sf::RectangleShape line;
	line.setPosition(window.getSize().x / 2, 0);
	line.setFillColor(sf::Color(128, 128, 128, 255));
	line.setSize(sf::Vector2f(5, window.getSize().y * 2 / 3));

	//GUI
	gui.addElement(&timeCount, "Time: ", &font);

	float gravityStr = Constant::calcDistance(gravity, Vector2d(0, 0));
	gui.addElement(&gravityStr, "Gravity: ", &font);

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
				else if (event.key.code == sf::Keyboard::Space) {
					rocket.toggleEngine();
				}
				window.setView(view);
				break;
			}
		}

		//Main Loop
		float time = clock.restart().asSeconds();
		deltaTime = time;
		timeCount += deltaTime ;
		secCount += time;

		//Update
		rocket.update(deltaTime, gravity);

		gravity = Constant::calcGravity(Constant::EarthMass, rocket.getRealPosition(), Vector2d(0, 0));

		//Map
		mapRocket.setPosition((earth.getPosition().x + earth.getRadius()) + (rocket.getRealPosition().x / pow(10, 5)) - mapRocket.getRadius() / 2, (earth.getPosition().y + earth.getRadius()) + (rocket.getRealPosition().y / pow(10, 5)) - mapRocket.getRadius() / 2);
		window.draw(earth);
		window.draw(mapRocket);
		window.draw(line);

		//GUI
		gravityStr = Constant::calcDistance(gravity, Vector2d(0, 0));
		h = Constant::calcDistance(rocket.getRealPosition(), Vector2d(0, 0)) - Constant::EarthRadie;

		gui.update(window.getView());
		//Time Update
		fps++;
		if (secCount >= 1) {
			window.setTitle("Projectile " + std::to_string(fps));
			fps = 0;
			secCount = 0;
		}

		//Render
		rocket.draw(window);
		gui.draw(window);

		window.display();
	}

	return 0;
}