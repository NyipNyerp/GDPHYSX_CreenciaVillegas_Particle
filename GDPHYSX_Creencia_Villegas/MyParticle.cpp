#include "MyParticle.h"
#include <iostream>
using namespace std;

MyParticle::MyParticle(MyVector newPos, MyVector newVelo, MyVector newAccel, MyVector newTotalVelo, MyVector newInitPos)
	: position(newPos), velocity(newVelo), acceleration(newAccel), totalVelocity(newTotalVelo), initialPos(newInitPos)
{
}

MyParticle::~MyParticle()
{
}

void MyParticle::update(float time)
{
	if (mass == 0)
	{
		return;
	}
	updatePos(time);
	updateVelocity(time);
	resetForce();
}

void MyParticle::updatePos(float time)
{
	position = position + (velocity * time) + ((acceleration * powf(time, 2)) * (1 / 2));
}

void MyParticle::updateVelocity(float time)
{
	//setting acceleration
	acceleration = acceleration + accumulatedForce * (1 / mass);

	velocity = velocity + acceleration * time;
	//velocity with damping
	velocity = velocity * powf(damp, time);

	//calculate total velocity over the sim
	totalVelocity = totalVelocity + velocity;
}

bool MyParticle::getIsDestroyed()
{
	return isDestroyed;
}

void MyParticle::setIsDestroyed()
{
	isDestroyed = true;
}

void MyParticle::addForce(MyVector f)
{
	accumulatedForce = accumulatedForce + f;
}

void MyParticle::resetForce()
{
	accumulatedForce = MyVector(0, 0, 0);
	acceleration = MyVector(0, 0, 0);
}



