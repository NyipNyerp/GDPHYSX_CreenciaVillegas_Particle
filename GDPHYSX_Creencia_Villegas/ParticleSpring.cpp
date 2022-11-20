#include "ParticleSpring.h"

void ParticleSpring::updateForce(MyParticle* particle, float time)
{
	MyVector pos = particle->position;

	MyVector force = pos - otherParticle->position;

	float mag = force.magnitude();

	float springForce = -springConstant * abs(mag - restLength);

	force.normalize();
	force = force * springForce;

	particle->addForce(force);
}