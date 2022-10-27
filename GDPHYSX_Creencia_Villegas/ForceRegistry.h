#ifndef FORCEREGISTRY
#define FORCEREGISTRY
#include  "particle.h";
#include  "ForceGenerator.h";
#include  <iostream>;
#include  <list>;

class ForceRegistry
{
public:
	void Add(particle* particle, ForceGenerator* generator);
	void Remove(particle* particle, ForceGenerator* generator);
	void Clear();
	void UpdateForces(float time);

protected:
	struct ParticleForceRegistry
	{
		particle* particle;
		ForceGenerator* generator;
	};

	std::list<ParticleForceRegistry> Registry;
};

#endif //FORCEREGISTRY