#include "ForceRegistry.h"

void ForceRegistry::add(MyParticle* particle, ForceGenerator* generator)
{
	ParticleGeneratorPair toAdd;
	toAdd.particle = particle;
	toAdd.generator = generator;

	registry.push_back(toAdd);
}

void ForceRegistry::remove(MyParticle* particle, ForceGenerator* generator)
{
	for (int i = 0; i < registry.size(); i++)
	{
		if (registry[i].particle == particle && registry[i].generator == generator)
		{
			registry.erase(registry.begin() + i);
		}
	}
}

void ForceRegistry::clear()
{
	registry.clear();
}

void ForceRegistry::updateForce(float time)
{
	for (int i = 0; i < registry.size(); i++)
	{
		registry[i].generator->updateForce(registry[i].particle, time);
	}
}
