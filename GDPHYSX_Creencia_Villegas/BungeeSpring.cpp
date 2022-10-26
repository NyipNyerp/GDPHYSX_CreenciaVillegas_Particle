#include "BungeeSpring.h"
#include <iostream>

void BungeeSpring::UpdateForce(particle* particle, float time)
{
	glm::vec3 pos = particle->pData.pos;

	glm::vec3 force = pos - anchorPoint;

	/*
	float mag = force.getMagnitude();

	if (mag <= bungeeLength)
	{
		std::cout << mag << std::endl;
		particle->AddForce(glm::vec3(0, 0));
	}
	else
	{
		float springForce = -springConstant * abs(mag - bungeeLength);

		force.Normalize();
		force = force * springForce;


		particle->AddForce(force);
	}
	*/
}