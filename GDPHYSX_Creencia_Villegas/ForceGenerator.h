#ifndef FORCEGENERATOR
#define FORCEGENERATOR
#include "particle.h"

class ForceGenerator
{
public:
	virtual void UpdateForce(particle* p, float time)
	{
		//base definition
		p->AddForce(MyVector());
	}
};

#endif //FORCEGENERATOR