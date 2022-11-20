#pragma once
#include "ForceGenerator.h"
#include "MyVector.h"
#include "MyParticle.h"
class AnchoredSpring : public ForceGenerator
{
private:
	MyVector anchorPoint;
	float springConstant;
	float restLength;
public:
	AnchoredSpring(MyVector _pos, float _springConst, float _restLen) :
		anchorPoint(_pos), springConstant(_springConst), restLength(_restLen) {}

	void updateForce(MyParticle* particle, float time) override;
};