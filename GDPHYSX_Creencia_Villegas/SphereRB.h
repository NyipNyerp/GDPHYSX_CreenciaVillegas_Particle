#pragma once
#include "PhysicsWorld.h"
#include "MyParticle.h"
#include "MyVector.h"

class SphereRB : public MyParticle
{
public:
	SphereRB();
	void addForceAtPoint(MyVector point, MyVector f);
};

