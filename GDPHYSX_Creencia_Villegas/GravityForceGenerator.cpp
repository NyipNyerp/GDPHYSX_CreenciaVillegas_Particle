#include "GravityForceGenerator.h"

void GravityForceGenerator::UpdateForce(particle* particle, float time)
{
	if (particle->pData.mass <= 0) return;
	glm::vec3 Force = Gravity * particle->pData.mass;
	particle->AddForce(Force);
}