#include "AnchoredSpring.h"

void AnchoredSpring::updateForce(MyParticle* particle, float time)
{
	MyVector pos = particle->position;
	MyVector force = pos - anchorPoint;
	float mag = force.magnitude();
	float springForce = -springConstant * abs(mag - restLength);

	force.normalize();
	MyVector dir = force;

	particle->addForce(dir * springForce);
}