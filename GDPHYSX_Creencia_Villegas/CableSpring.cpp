#include "CableSpring.h"

void CableSpring::UpdateForce(particle* particle, float time)
{
	glm::vec3 pos = particle->pData.pos;

	glm::vec3 force = pos - anchorPoint;

	/*
	float mag = force.getMagnitude();

	float springForce = -springConstant * abs(mag - restLength);

	force.Normalize();
	force *= springForce;

	particle->AddForce(force);
	*/
}