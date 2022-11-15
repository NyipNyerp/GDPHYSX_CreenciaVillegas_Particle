#include "ParticleContact.h"
#include <iostream>

ParticleContact::ParticleContact()
{
}

void ParticleContact::resolve(float time)
{
	resolveVelocity(time);
	resolveInterpenetration(time);
}

float ParticleContact::getSeparatingSpeed()
{
	MyVector resu(0, 0, 0);


	MyVector velocityDiff = particles[0]->velocity;
	if (particles[1])
		velocityDiff = velocityDiff - particles[1]->velocity;



	//std::cout << "velocity diff x: " << velocityDiff.x << "velocity diff y: " << velocityDiff.y << std::endl;
	//std::cout << "collision normal x: " << collisonNormal.x << "collision normal y: " << collisonNormal.y << std::endl;

	return resu.dotProduct(velocityDiff, collisonNormal);
}

void ParticleContact::resolveInterpenetration(float time)
{
	if (depth <= 0)
		return;

	float totalMass = 1.0f / particles[0]->mass;
	if (particles[1])
		totalMass += 1.0f / particles[1]->mass;

	if (totalMass <= 0)
		return;

	float depthMagnitude = depth / totalMass;
	MyVector depthVector = collisonNormal * depthMagnitude;

	MyVector aPos = depthVector * ((float)1 / particles[0]->mass);
	particles[0]->position = particles[0]->position + aPos;

	if (particles[1])
	{
		MyVector bPos = depthVector * (-(float)1 / particles[1]->mass);
		particles[1]->position = particles[1]->position + bPos;
	}

	depth = 0;
}

void ParticleContact::resolveVelocity(float time)
{
	float separatingSpeed = getSeparatingSpeed();

	if (separatingSpeed > 0.0f)
	{
		return;
	}

	//std::cout << "p1 velocity x: " << particles[0]->velocity.x << "velocity diff y: " << velocityDiff.y << std::endl;

	float sF = -restitution * separatingSpeed;
	float deltaSpeed = sF - separatingSpeed;

	float totalMass = 1.0f / particles[0]->mass;


	if (particles[1])
		totalMass += 1.0f / particles[1]->mass;


	if (totalMass <= 0)
	{
		return;
	}



	float impulse_mag = deltaSpeed / totalMass;
	MyVector impulse = collisonNormal * impulse_mag;

	MyVector v_0 = impulse * (1.0f / particles[0]->mass);
	particles[0]->velocity = (particles[0]->velocity + v_0);


	if (particles[1])
	{
		MyVector v_1 = impulse * (-1.0f / particles[1]->mass);
		particles[1]->velocity = (particles[1]->velocity + v_1);

	}
}