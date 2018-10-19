#ifndef CONSTANT_H
#define CONSTANT_H

#include <math.h>
#include <SFML/Graphics.hpp>

namespace Constant {
	typedef sf::Vector2<double> Vector2d;


	//Earth Constants
	const double EarthMass = 5.972 * pow(10, 24);
	const double EarthRadie = 6.371 * pow(10, 6);

	const float pSea = 101325;
	const float pAltitude[] = { 101325, 54054, 26506, 5531, 1198, 287, 0 };
	const float pHeight[] = { 0, 5000, 10000, 20000, 30000, 40000, 45000 };

	const int airHeight[] = { 0, 1000, 5000, 10000, 20000, 30000, 50000, 80000, 200000 };
	const float airDensity[] = { 1.225, 1.112, 0.7364, 0.4135, 0.08891, 0.01841, 0.001027, 0.00001846, 0};

	//Physics Constans
	const double GravityConstant = 6.676 * pow(10, -11);

	//Calculation Functions
	static double calcDistance(Vector2d pos1, Vector2d pos2) {
		Vector2d vec = pos1 - pos2;
		return sqrt(pow(vec.x, 2) + pow(vec.y, 2));
	}

	static Vector2d calcGravity(double mass, Vector2d pos1, Vector2d pos2)
	{
		Vector2d vec = pos1 - pos2;
		double distance = pow(vec.x, 2) + pow(vec.y, 2); //We Square it later so doing square root is uncessary
		vec /= sqrt(distance);
		double M = GravityConstant * mass;
		return vec * (M / distance);
	};

	static double calcAirDensity(double height)
	{
		float d = 0;
		for (int i = 1; i < 9; i++) {
			if (height > airHeight[i - 1] && height < airHeight[i]) {
				d = (airDensity[i - 1] * (airHeight[i] - height) + airDensity[i] * (height - airHeight[i - 1])) / (airHeight[i] - airHeight[i - 1]);
				break;
			}
		}
		return d;
	};

	static double calcPressure(double height)
	{
		float p = 0;
		for (int i = 1; i < 9; i++) {
			if (height > pHeight[i - 1] && height < pHeight[i]) {
				p = (pAltitude[i - 1] * (pHeight[i] - height) + pAltitude[i] * (height - pHeight[i - 1])) / (pHeight[i] - pHeight[i - 1]);
				break;
			}
		}
		return p;
	};
}

#endif