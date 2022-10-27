#ifndef GRAVITYFORCEGENERATOR
#define GRAVITYFORCEGENERATOR
#include "ForceGenerator.h"

class GravityForceGenerator : public ForceGenerator
{
private:
	MyVector Gravity = MyVector(0, -9.8);

public:
	GravityForceGenerator() {}
	GravityForceGenerator(const MyVector gravity) : Gravity(gravity) {}

	void UpdateForce(MyParticle* particle, float time)override;
};

#endif //GRAVITYFORCEGENERATOR