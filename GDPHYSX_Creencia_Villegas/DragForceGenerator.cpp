#include "DragForceGenerator.h"

void DragForceGenerator::updateForce(MyParticle* p, float time)
{
	MyVector force = MyVector(0, 0, 0);

	MyVector currV = p->velocity;
	float mag = currV.magnitude();
	float dragF = (k1 * mag) + (k2 * mag * mag);

	currV.normalize();
	MyVector dir = currV;

	p->addForce(dir * (-dragF));
}