#pragma once
#include "ForceGenerator.h"
class DragForceGenerator : public ForceGenerator
{
private:
	float k1 = 0.74;
	float k2 = 0.54;

public:
	DragForceGenerator() {}
	DragForceGenerator(float _k1, float _k2) : k1(_k1), k2(_k2) {}

	void updateForce(MyParticle* p, float time) override;
};