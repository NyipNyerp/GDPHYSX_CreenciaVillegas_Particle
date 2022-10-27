#include "BungeeSpring.h"
#include <iostream>

void BungeeSpring::UpdateForce(particle* particle, float time)
{
	MyVector pos = particle->pos;

	MyVector force = pos - anchorPoint;

	float mag = force.getMagnitude();

	if (mag <= bungeeLength)
	{
		std::cout << mag << std::endl;
		particle->AddForce(MyVector(0, 0));
	}
	else
	{
		float springForce = -springConstant * abs(mag - bungeeLength);

		force.Normalize();
		force = force * springForce;


		particle->AddForce(force);
	}
}