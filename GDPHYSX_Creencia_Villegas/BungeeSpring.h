#ifndef BUNGEESPRING
#define BUNGEESPRING
#include "particle.h"
#include "ForceGenerator.h"

class BungeeSpring : public ForceGenerator
{
private:
	glm::vec3 anchorPoint;
	float springConstant;
	float bungeeLength;

public:
	BungeeSpring(glm::vec3 _pos, float _springConst, float _restLen) :
		anchorPoint(_pos), springConstant(_springConst), bungeeLength(_restLen) {}

	void UpdateForce(particle* partice, float time) override;
};

#endif // !BUNGEESPRING