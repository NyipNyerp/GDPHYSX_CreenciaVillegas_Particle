#pragma once
#include "MyVector.h"
#include "MyParticle.h"
#include "ForceGenerator.h"

class BungeeSpring : public ForceGenerator
{
private:
	MyVector anchorPoint;
	float springConstant;
	float bungeeLength;

public:
	BungeeSpring(MyVector _pos, float _springConst, float _restLen) :
		anchorPoint(_pos), springConstant(_springConst), bungeeLength(_restLen) {}

	void updateForce(MyParticle* particle, float time) override;
};