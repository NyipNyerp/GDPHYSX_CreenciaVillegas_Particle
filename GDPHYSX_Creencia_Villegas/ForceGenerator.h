#ifndef FORCEGENERATOR
#define FORCEGENERATOR
#include "MyParticle.h"

class ForceGenerator
{
public:
	virtual void updateForce(MyParticle* p, float time)
	{
		//base definition
		p->addForce(MyVector(0,0,0));
	}
};

#endif //FORCEGENERATOR