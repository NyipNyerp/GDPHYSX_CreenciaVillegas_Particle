#include "GravityForceGenerator.h"

void GravityForceGenerator::UpdateForce(MyParticle* particle, float time)
{
	if (particle->mass <= 0) return;
	MyVector Force = Gravity * particle->mass;
	particle->AddForce(Force);
}