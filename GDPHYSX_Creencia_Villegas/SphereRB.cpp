#include "SphereRB.h"


SphereRB::SphereRB()
{
}

void SphereRB::addForceAtPoint(MyVector point, MyVector f)
{
	accumulatedForce = accumulatedForce + f;
	accumulatedTorque = accumulatedTorque + MyVector::crossProductF(point, f);
}