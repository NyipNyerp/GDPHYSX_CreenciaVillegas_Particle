#ifndef GRAVITYFORCEGENERATOR
#define GRAVITYFORCEGENERATOR
#include "ForceGenerator.h"

class GravityForceGenerator : public ForceGenerator
{
private:
	glm::vec3 Gravity = glm::vec3(0.0f, -9.8f, 0.0f);

public:
	GravityForceGenerator() {}
	GravityForceGenerator(const glm::vec3 gravity) : Gravity(gravity) {}

	void UpdateForce(particle* particle, float time)override;
};

#endif //GRAVITYFORCEGENERATOR