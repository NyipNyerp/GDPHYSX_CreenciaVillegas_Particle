#include "CableSpring.h"

void CableSpring::updateForce(MyParticle* particle, float time)
{
	MyVector pos = particle->position;

	MyVector force = pos - anchorPoint;

	float mag = force.magnitude();

	float springForce = -springConstant * abs(mag - restLength);

	force.normalize();
	force = force * springForce;

	particle->addForce(force);
}