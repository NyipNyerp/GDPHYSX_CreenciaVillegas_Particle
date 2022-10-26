#include "DragForceGenerator.h"

void DragForceGenerator::UpdateForce(particle* particle, float time)
{
	glm::vec3 Force = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 currV = particle->pData.velo;

	/*
	float mag = currV.getMagnitude();
	float DragF = (k1 * mag) + (k2 * mag * mag);

	currV.Normalize();
	glm::vec3 dir = currV;

	particle->AddForce(dir * (-DragF));
	*/
}