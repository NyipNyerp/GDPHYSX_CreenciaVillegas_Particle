#ifndef ANCHOREDSPRING
#define ANCHOREDSPRING
#include "ForceGenerator.h"

class AnchoredSpring : public ForceGenerator
{
private:
	//other end of the spring
	MyVector anchorPoint;
	//the spring constant
	float springConstant;
	//the rest kength of this spring
	float restLength;

public:
	AnchoredSpring(MyVector _pos, float _spring, float _restLen) :
		anchorPoint(_pos), springConstant(_spring), restLength(_restLen) {}
	void UpdateForce(MyParticle* particle, float time)override;
};

#endif // !ANCHOREDSPRING