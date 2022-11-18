#pragma once
#include "ForceGenerator.h"
class GravityForceGenerator : public ForceGenerator
{
private:
	MyVector gravity = MyVector(0, 0, 0);
public:
	GravityForceGenerator(MyVector g) : gravity(g) {};
	GravityForceGenerator() {}

	void updateForce(MyParticle* p, float time) override;
};