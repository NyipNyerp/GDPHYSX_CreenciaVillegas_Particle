#include "ParticleSpring.h"

void ParticleSpring::UpdateForce(particle* particle, float time)
{
	glm::vec3 pos = particle->pData.pos;

	glm::vec3 force = pos - otherParticle->pData.pos;

	/*
	float mag = force.getMagnitude();

	float springForce = -springConstant * abs(mag - restLength);

	force.Normalize();
	force = force * springForce;

	particle->AddForce(force);
	*/
}