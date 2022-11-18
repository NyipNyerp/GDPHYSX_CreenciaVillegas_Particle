#include "ParticleSpring.h"

float ParticleSpring::getDistance()
{
	MyVector dist = particles[0]->position - particles[1]->position;
	return dist.magnitude();
}