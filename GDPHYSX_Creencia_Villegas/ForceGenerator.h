#pragma once
#include "MyParticle.h"

class ForceGenerator
{
public:
	virtual void updateForce(MyParticle* p, float time)
	{
		p->addForce(MyVector(0, 0, 0));
	}
};