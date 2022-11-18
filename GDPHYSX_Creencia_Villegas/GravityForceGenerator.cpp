#include "GravityForceGenerator.h"

void GravityForceGenerator::updateForce(MyParticle* p, float time)
{
	if (p->mass <= 0)
	{
		return;
	}

	MyVector force = gravity * p->mass;
	p->addForce(force);
}