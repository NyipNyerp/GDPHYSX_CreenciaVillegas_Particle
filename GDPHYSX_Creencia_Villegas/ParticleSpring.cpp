#include "ParticleSpring.h"

void ParticleSpring::UpdateForce(particle* particle, float time)
{
	MyVector pos = particle->pos;

	MyVector force = pos - otherParticle->pos;

	float mag = force.getMagnitude();

	float springForce = -springConstant * abs(mag - restLength);

	force.Normalize();
	force = force * springForce;

	particle->AddForce(force);
}