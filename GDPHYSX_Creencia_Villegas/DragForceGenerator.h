#ifndef DRAGFORCEGENERATOR
#define DRAGFORCEGENERATOR
#include "ForceGenerator.h"

class DragForceGenerator : public ForceGenerator
{
private:
	float k1 = 0.74; //friction x
	float k2 = 0.57; //friction y

public:
	DragForceGenerator() {}
	DragForceGenerator(float _k1, float _k2) : k1(_k1), k2(_k1) {}
	void UpdateForce(particle* particle, float time)override;

};

#endif // !DRAGFORCEGENERATOR