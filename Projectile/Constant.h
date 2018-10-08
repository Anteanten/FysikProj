#ifndef CONSTANT_H
#define CONSTANT_H

#include <math.h>
#include <SFML/Graphics.hpp>

namespace Constant {
	//Earth
	const float EarthMass = 5.972 * pow(10, 24);
	const float EarthRadie = 6.371 * pow(10, 6);

	const int airHeight[] = { 0, 1000, 5000, 10000, 20000, 30000, 50000, 80000, 200000 };
	const float airDensity[] = { 1.225, 1.112, 0.7364, 0.4135, 0.08891, 0.01841, 0.001027, 0.00001846, 0};

	//Physics
	const float GravityConstant = 6.676 * pow(10, -11);

	static float calcGravity(float mass, sf::Vector3f pos1, sf::Vector3f pos2)
	{
		float M = GravityConstant * mass;
		float distance = abs(pos1.x - pos2.x) + abs(pos1.y - pos2.y) + abs(pos1.z - pos2.z);
		return M / pow(distance, 2);
	};

	static float calcAirDensity(float height)
	{
		float d = airDensity[0];
		for (int i = 1; i < 9; i++) {
			if (height > airHeight[i - 1] && height < airHeight[i]) {
				d = (airDensity[i - 1] * (airHeight[i] - height) + airDensity[i] * (height - airHeight[i - 1])) / (airHeight[i] - airHeight[i - 1]);
				break;
			}
		}
		return d;
	};
}

#endif