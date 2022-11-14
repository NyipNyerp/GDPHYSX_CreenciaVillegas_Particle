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
	registry.remove_if(
		[particle, generator](ParticleGeneratorPair pair) {
			return pair.particle == particle && pair.generator == generator;
		}
	);
}

void ForceRegistry::clear()
{
	registry.clear();
}

void ForceRegistry::updateForce(float time)
{

	for (list<ParticleGeneratorPair>::iterator i = registry.begin(); i != registry.end(); i++)
	{
		i->generator->updateForce(i->particle, time);
	}
}