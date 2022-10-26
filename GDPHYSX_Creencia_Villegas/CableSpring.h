#ifndef CABLESPRING
#define CABLESPRING
#include "ForceGenerator.h"

class CableSpring : public ForceGenerator
{
private:
	//other end of the spring
	glm::vec3 anchorPoint;
	//the spring constant
	float springConstant;
	//the rest kength of this spring
	float restLength;

public:
	CableSpring(glm::vec3 _pos, float _spring, float _restLen) :
		anchorPoint(_pos), springConstant(_spring), restLength(_restLen) {}
	void UpdateForce(particle* particle, float time)override;
};

#endif // !CABLESPRING