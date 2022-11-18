#pragma once
#include "ParticleContact.h"
#include <vector>

using namespace std;

class ContactResolver
{
public:
	unsigned max_iterations;
	ContactResolver(unsigned _maxIterations) : max_iterations(_maxIterations) {}
	void resolveContacts(vector<ParticleContact*> contacts, float time);

protected:
	unsigned current_iterations = 0;

};