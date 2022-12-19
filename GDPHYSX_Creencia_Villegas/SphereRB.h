#pragma once
#include "PhysicsWorld.h"
#include "MyParticle.h"
#include "MyVector.h"

class SphereRB : public MyParticle
{
public:
	SphereRB(int newType);
	void addForceAtPoint(MyVector point, MyVector f);
};

