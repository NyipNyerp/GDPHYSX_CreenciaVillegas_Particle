#pragma once
#include "MyParticle.h"
class ParticleContact
{
public:
	MyParticle* particles[2];
	float restitution;
	MyVector collisonNormal = MyVector(0, 0, 0);
	float depth = 0;

public:
	ParticleContact();
	void resolve(float time);
	float getSeparatingSpeed();
	void resolveInterpenetration(float time);

protected:
	void resolveVelocity(float time);
};