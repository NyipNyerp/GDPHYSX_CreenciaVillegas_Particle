#include "AnchoredSpring.h"

void AnchoredSpring::UpdateForce(particle* particle, float time)
{
	MyVector pos = particle->pos;

	MyVector force = pos - anchorPoint;

	float mag = force.getMagnitude();

	float springForce = -springConstant * abs(mag - restLength);

	force.Normalize();
	force *= springForce;

	particle->AddForce(force);
}