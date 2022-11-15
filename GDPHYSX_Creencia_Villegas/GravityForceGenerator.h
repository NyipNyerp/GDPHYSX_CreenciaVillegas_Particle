#pragma once
#include "ForceGenerator.h"

class GravityForceGenerator : public ForceGenerator
{
private:
	MyVector gravity = MyVector(0, -9.8,0);

public:
	GravityForceGenerator() {}
	GravityForceGenerator(MyVector g) : gravity(g) {}

	void updateForce(MyParticle* p, float time)override;
};