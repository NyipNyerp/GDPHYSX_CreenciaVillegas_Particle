#include "SphereRB.h"


SphereRB::SphereRB(int newType)
{
	this->type = newType;
	particleType();
}

void SphereRB::addForceAtPoint(MyVector point, MyVector f)
{
	//accumulatedForce = accumulatedForce + f;
	//accumulatedTorque = accumulatedTorque + MyVector::crossProductF(point, f);
}