#ifndef PARTICLESPRING
#define PARTICLESPRING
#include "particle.h"
#include "ForceGenerator.h"

class ParticleSpring : public ForceGenerator
{
private:
	particle* otherParticle;
	float springConstant;
	float restLength;
public:
	ParticleSpring(particle* _otherParticle, float _springConst, float _restLen) :
		otherParticle(_otherParticle), springConstant(_springConst), restLength(_restLen) {}
	void UpdateForce(particle* particle, float time)override;
};

#endif // !PARTICLESPRING
