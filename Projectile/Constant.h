#include <math.h>
#include <SFML/Graphics.hpp>

namespace Constant {
	//Earth
	const float EarthMass = 5.972 * pow(10, 24);
	const float EarthRadie = 6.741 * pow(10, 6);

	//Physics
	const float GravityConstant = 6.676 * pow(10, -11);

	float calcGravity(float mass, sf::Vector3f pos1, sf::Vector3f pos2)
	{
		float M = GravityConstant * mass;
		float distance = sqrt(pow(pos1.x - pos2.x, 2) + pow(pos1.y - pos2.y, 2)) + pow(pos1.z - pos2.z, 2);
		return M / distance;
	}


}