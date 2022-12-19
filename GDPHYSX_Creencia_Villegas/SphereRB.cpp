#include "SphereRB.h"


SphereRB::SphereRB(int newType)
{
	this->type = newType;
	this->name = "BULLET";
	this->velocity = MyVector(35.0f, 0.0f, 0.0f);
	this->acceleration = MyVector(0.0f, -1.0f, 0.0f);
	this->mass = 2.0f;
	this->damping = 0.99f;
	this->lifeSpan = 15;
}

void SphereRB::addForceAtPoint(MyVector point, MyVector f)
{
	accumulatedForce = accumulatedForce + f;
	accumulatedTorque = accumulatedTorque + MyVector::crossProductF(point, f);
}