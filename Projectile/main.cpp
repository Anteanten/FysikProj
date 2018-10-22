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
	float timeSpeed = 1;
	int fps = 0;
	bool lockToRocket = false;

	sf::Font font;
	font.loadFromFile("arial.ttf");

	//RealPosition, Window Position, Velocity, Thrust, Rotation
	Rocket rocket(Vector2d((double)0, -Constant::EarthRadie), sf::Vector2f(300, 300), sf::Vector2f(0, 0), 270);
	Vector2d gravity = Constant::calcGravity(Constant::EarthMass, rocket.getRealPosition(), Vector2d(0, 0));

	sf::RectangleShape background;
	background.setPosition(0, 0);
	background.setFillColor(sf::Color::Blue);
	background.setSize(sf::Vector2f(window.getSize().x / 2, window.getSize().y * 2 / 3));

	//Map Shapes
	double mapScale = 1 / pow(10, 5);

	sf::CircleShape earth;
	earth.setPosition(600, 200);
	earth.setRadius(Constant::EarthRadie * mapScale);
	earth.setFillColor(sf::Color::Blue);

	sf::CircleShape mapRocket;
	mapRocket.setPosition((earth.getPosition().x + earth.getRadius()) - (rocket.getRealPosition().x * mapScale), (earth.getPosition().y + earth.getRadius()) - (rocket.getRealPosition().y * mapScale));
	mapRocket.setRadius(5);
	mapRocket.setFillColor(sf::Color::Red);

	sf::RectangleShape line;
	line.setPosition(window.getSize().x / 2, 0);
	line.setFillColor(sf::Color(128, 128, 128, 255));
	line.setSize(sf::Vector2f(5, window.getSize().y * 2 / 3));

	//GUI
	gui.addElement(&timeCount, "Time: ", &font);
	gui.addElement(&timeSpeed, "TimeSpeed: ", &font);

	float gravityStr = Constant::calcDistance(gravity, Vector2d(0, 0));
	gui.addElement(&gravityStr, "Gravity: ", &font);

	float rocketMass = rocket.getMass();
	gui.addElement(&rocketMass, "Mass: ", &font);

	float h = 0;
	gui.addElement(&h, "Height: ", &font);

	float v = 0;
	gui.addElement(&v, "Speed: ", &font);

	float a = 0;
	gui.addElement(&a, "Accel: ", &font);

	float up = 0;
	gui.addElement(&up, "Up Speed: ", &font);

	float side = 0;
	gui.addElement(&side, "Side Speed: ", &font);

	float apo = 0;
	gui.addElement(&apo, "Apoapis: ", &font);

	float per = 0;
	gui.addElement(&per, "Periapsis: ", &font);

	float T = 0;
	gui.addElement(&T, "Orbit T: ", &font);

	//MISC
	sf::Text rocketText;
	rocketText.setFont(font);
	rocketText.setCharacterSize(20);
	rocketText.setPosition(10, 10);
	rocketText.setString("Toggle Engine: Space\nRotate: Q/E\nChange Time: R/T");

	sf::Text mapText;
	mapText.setFont(font);
	mapText.setCharacterSize(20);
	mapText.setPosition(window.getSize().x / 2 + 10, 10);
	mapText.setString("Lock To Rocket: L\nCamera: WASD\nZoom: MouseWheel");

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
					mapScale *= 1.05f;
					earth.setRadius(Constant::EarthRadie * mapScale);
				}
				else {
					mapScale *= 0.95f;
					earth.setRadius(Constant::EarthRadie * mapScale);
				};
				window.setView(view);
				break;
				//WASD, Move camera
			case sf::Event::KeyPressed:
				if (event.key.code == sf::Keyboard::W) {
					earth.setPosition(earth.getPosition() + sf::Vector2f(0, 5));
					mapRocket.setPosition(rocket.getPosition() + sf::Vector2f(0, 5));
				}
				else if (event.key.code == sf::Keyboard::S) {
					earth.setPosition(earth.getPosition() + sf::Vector2f(0, -5));
					mapRocket.setPosition(rocket.getPosition() + sf::Vector2f(0, -5));
				}
				else if (event.key.code == sf::Keyboard::A) {
					earth.setPosition(earth.getPosition() + sf::Vector2f(5, 0));
					mapRocket.setPosition(rocket.getPosition() + sf::Vector2f(5, 0));
				}
				else if (event.key.code == sf::Keyboard::D) {
					earth.setPosition(earth.getPosition() + sf::Vector2f(-5, 0));
					mapRocket.setPosition(rocket.getPosition() + sf::Vector2f(-5, 0));
				}
				else if (event.key.code == sf::Keyboard::Q) {
					rocket.setRotation(rocket.getRotation() + 5);
				}
				else if (event.key.code == sf::Keyboard::E) {
					rocket.setRotation(rocket.getRotation() - 5);
				}
				break;
			case sf::Event::KeyReleased:
				if (event.key.code == sf::Keyboard::R) {
					if (timeSpeed < 5) {
						timeSpeed++;
					}
					else if (timeSpeed < 20) {
						timeSpeed *= 2;
					}
					else if (timeSpeed == 20) {
						timeSpeed = 50;
					}
					else if(timeSpeed < 200){
						timeSpeed += 50;
					}
					else {
						timeSpeed += 100;
					}
				}
				else if (event.key.code == sf::Keyboard::T) {
						if (timeSpeed <= 5 && timeSpeed > 0) {
							timeSpeed--;
						}
						else if (timeSpeed <= 20) {
							timeSpeed /= 2;
						}
						else if (timeSpeed == 50) {
							timeSpeed = 20;
						}
						else if (timeSpeed <= 200) {
							timeSpeed -= 50;
						}
						else {
							timeSpeed -= 100;
						}
				}
				else if (event.key.code == sf::Keyboard::Space) {
					rocket.toggleEngine();
				}
				else if (event.key.code == sf::Keyboard::L) {
					lockToRocket = !lockToRocket;
				}
				window.setView(view);
				break;
			}
		}

		//Main Loop
		float time = clock.restart().asSeconds();
		deltaTime = time * timeSpeed;
		timeCount += deltaTime ;
		secCount += time;

		//Update
		if (timeSpeed > 0) {
			rocket.update(deltaTime, gravity);
		}

		gravity = Constant::calcGravity(Constant::EarthMass, rocket.getRealPosition(), Vector2d(0, 0));

		//Map
		if (lockToRocket) {
			mapRocket.setPosition(sf::Vector2f(window.getSize().x * 3 / 4, window.getSize().y / 3));
			earth.setPosition((mapRocket.getPosition().x - earth.getRadius()) - (rocket.getRealPosition().x * mapScale) + mapRocket.getRadius() / 2, (mapRocket.getPosition().y - earth.getRadius()) - (rocket.getRealPosition().y * mapScale) + mapRocket.getRadius() / 2);
		}
		else {
			mapRocket.setPosition((earth.getPosition().x + earth.getRadius()) + (rocket.getRealPosition().x * mapScale) - mapRocket.getRadius() / 2, (earth.getPosition().y + earth.getRadius()) + (rocket.getRealPosition().y * mapScale) - mapRocket.getRadius() / 2);
		}
		window.draw(earth);
		window.draw(mapRocket);
		window.draw(line);

		//GUI
		a = sqrt(pow(rocket.getAcceleration().x, 2) + pow(rocket.getAcceleration().y, 2));
		v = sqrt(pow(rocket.getVelocity().x, 2) + pow(rocket.getVelocity().y, 2));
		up = rocket.getVelocityUp(gravity / (double)gravityStr);
		side = rocket.getVelocitySide(gravity / (double)gravityStr);
		apo = rocket.getApoapasis() - Constant::EarthRadie;
		per = rocket.getPeriapsis() - Constant::EarthRadie;
		rocketMass = rocket.getMass();
		T = rocket.getOrbitalPeriod();
		gravityStr = Constant::calcDistance(gravity, Vector2d(0, 0));
		h = Constant::calcDistance(rocket.getRealPosition(), Vector2d(0, 0)) - Constant::EarthRadie;
		if (h < 255 * 250)
			background.setFillColor(sf::Color(0, 0, 255 - (h / 250), 255));

		gui.update(window.getView());
		//Time Update
		fps++;
		if (secCount >= 1) {
			window.setTitle("Projectile " + std::to_string(fps));
			fps = 0;
			secCount = 0;
		}

		//Render
		window.draw(background);
		rocket.draw(window);
		gui.draw(window);
		window.draw(rocketText);
		window.draw(mapText);

		window.display();
	}

	return 0;
}