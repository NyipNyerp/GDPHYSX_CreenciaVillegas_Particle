#include "ForceRegistry.h"

void ForceRegistry::Add(MyParticle* particle, ForceGenerator* generator)
{
	ParticleForceRegistry toAdd;

	toAdd.particle = particle;
	toAdd.generator = generator;

	Registry.push_back(toAdd);
}

void ForceRegistry::UpdateForces(float time)
{
	for (std::list<ParticleForceRegistry>::iterator i = Registry.begin();
		i != Registry.end(); i++)
	{
		i->generator->UpdateForce(i->particle, time);
	}
}

void ForceRegistry::Remove(MyParticle* particle, ForceGenerator* generator)
{
	Registry.remove_if([particle, generator](ParticleForceRegistry reg) {
		return reg.particle == particle && reg.generator == generator;
		}
	);
}

void ForceRegistry::Clear()
{
	Registry.clear();
}