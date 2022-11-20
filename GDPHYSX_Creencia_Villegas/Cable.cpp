#include "Cable.h"

ParticleContact* Cable::getContact()
{
	float curr_len = getDistance();

	if (curr_len == length)
	{
		return nullptr;
	}

	ParticleContact* ret = new ParticleContact();
	ret->particles[0] = particles[0];
	ret->particles[1] = particles[1];

	MyVector dir = particles[1]->position - particles[0]->position;
	dir.normalize();

	if (curr_len > length)
	{
		ret->collisonNormal = dir;
		ret->depth = curr_len - length;
	}

	ret->restitution = restitution;

	return ret;
}