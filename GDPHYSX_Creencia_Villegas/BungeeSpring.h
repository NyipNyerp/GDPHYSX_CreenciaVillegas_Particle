#ifndef BUNGEESPRING
#define BUNGEESPRING
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

	void UpdateForce(MyParticle* partice, float time) override;
};

#endif // !BUNGEESPRING