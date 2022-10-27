#ifndef FORCEREGISTRY
#define FORCEREGISTRY
#include  "MyParticle.h";
#include  "ForceGenerator.h";
#include  <iostream>;
#include  <list>;

class ForceRegistry
{
public:
	void Add(MyParticle* particle, ForceGenerator* generator);
	void Remove(MyParticle* particle, ForceGenerator* generator);
	void Clear();
	void UpdateForces(float time);

protected:
	struct ParticleForceRegistry
	{
		MyParticle* particle;
		ForceGenerator* generator;
	};

	std::list<ParticleForceRegistry> Registry;
};

#endif //FORCEREGISTRY