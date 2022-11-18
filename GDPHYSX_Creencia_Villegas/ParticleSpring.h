#pragma once
#include "ParticleContact.h"
#include "MyParticle.h"

class ParticleSpring
{
public:
	MyParticle* particles[2];
	virtual ParticleContact* getContact() { return nullptr; }

protected:
	float getDistance();
};