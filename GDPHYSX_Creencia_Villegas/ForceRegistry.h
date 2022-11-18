#pragma once
#include "MyParticle.h"
#include "ForceGenerator.h"
#include <list>

using namespace std;

class ForceRegistry
{
protected:
	struct ParticleGeneratorPair
	{
		MyParticle* particle;
		ForceGenerator* generator;
	};

	vector <ParticleGeneratorPair> registry;

public:
	void add(MyParticle* particle, ForceGenerator* generator);
	void remove(MyParticle* particle, ForceGenerator* generator);
	void clear();
	void updateForce(float time);
};