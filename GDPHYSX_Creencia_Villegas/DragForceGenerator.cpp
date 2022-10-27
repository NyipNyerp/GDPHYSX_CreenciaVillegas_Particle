#include "DragForceGenerator.h"
#include "MyVector.h"

void DragForceGenerator::UpdateForce(particle* particle, float time)
{
	MyVector Force = MyVector(0, 0);
	MyVector currV = particle->velo;

	float mag = currV.getMagnitude();
	float DragF = (k1 * mag) + (k2 * mag * mag);

	currV.Normalize();
	MyVector dir = currV;

	particle->AddForce(dir * (-DragF));
}