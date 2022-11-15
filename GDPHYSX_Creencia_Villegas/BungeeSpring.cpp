#include "BungeeSpring.h"
#include <iostream>

void BungeeSpring::updateForce(MyParticle* particle, float time)
{
	/*
	* Note: Will only work with SFML coordinate system (+Y going down, -Y going up)
	*/

	//current pos of particle
	MyVector pos = particle->position;
	//dynamic force from current position of particle to the fixed anchor point
	MyVector force = pos - anchorPoint;
	//calculate magnitude of force
	float mag = force.magnitude();

	//calculate the spring force using the equation
	// f = -k * (|lcurr - lrest|)
	//absolute value was removed here though because a bungee cord does not need
	//to spring up its anchor point
	float springForce = -springConstant * (mag - bungeeLength);

	//direction from the current position of particle to the anchor point
	force.normalize();
	MyVector dir = force;

	//apply the force
	//0.2 * springforce * time was used because as time progresses, the springforce of the bungee cord decreases
	//as the particle springs up and down below the cord's anchor point

	//note: the person doing a bungee jump does not immediately spring back to its jumping point, he/she springs up
	//and down the jumping point

	//kind of acts like the damping attribute of the spring
	//particle->addForce(dir * springForce);
	particle->addForce(dir * ((0.2) * springForce) * time);
}