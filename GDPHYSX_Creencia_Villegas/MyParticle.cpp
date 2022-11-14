#include "MyParticle.h"
#include <iostream>
using namespace std;

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

/*
void MyParticle::updateDestroyed()
{
	//Change Particle Destructor here after the question mark
	if (timer.getElapsedTime().asSeconds() >= (rand() % 2 == 0 ? 0.5 : 1))
	{
		setIsDestroyed();
	}
}
*/

float MyParticle::measureTime()
{
	//sf::Clock clock;
	//sf::Time elapsed = clock.getElapsedTime();
	//return elapsed.asSeconds() * 1000;
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
	accumulatedForce = MyVector(0, 0,0);
	acceleration = MyVector(0, 0,0);
}

MyParticle::MyParticle()
{
}



