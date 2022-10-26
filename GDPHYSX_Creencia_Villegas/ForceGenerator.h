#ifndef FORCEGENERATOR
#define FORCEGENERATOR
#include "particle.h"

class ForceGenerator
{
	virtual void UpdateForce(particle* p, float time)
	{
		//base definition
		p->AddForce(glm::vec3(0.0f, 0.0f, 0.0f));
	}
};

#endif //FORCEGENERATOR