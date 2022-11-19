#include "ParticleLink.h"

float ParticleLink::getDistance()
{
	MyVector dist = particles[0]->position - particles[1]->position;
	return dist.magnitude();
}