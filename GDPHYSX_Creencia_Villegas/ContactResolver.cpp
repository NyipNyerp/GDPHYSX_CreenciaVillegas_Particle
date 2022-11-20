#include "ContactResolver.h"

void ContactResolver::resolveContacts(vector<ParticleContact*> contacts, float time)
{
	current_iterations = 0;

	while (current_iterations < max_iterations)
	{
		float minimum = contacts[0]->getSeparatingSpeed();
		float minimumIndex = 0;
		bool allResolved = true;

		//Get contact with LEAST SS
		for (unsigned i = 0; i < contacts.size(); i++)
		{

			if (contacts[i]->getSeparatingSpeed() < minimum) //check if separating speed of current contact is + or nah
			{
				allResolved = false;
				minimumIndex = i;
				minimum = contacts[i]->getSeparatingSpeed();
			}

			if (minimum > contacts[i]->getSeparatingSpeed())
			{

				//contacts[i] = contacts.front();
			}
		}

		//RESOLVE THAT CONTACT
		contacts[minimumIndex]->resolve(time);

		//ITERATE
		current_iterations++;

		//CHECK IF SS OF ALL CONTACTS ARE >=0

		if (allResolved == true)
		{
			break;
		}
	}
}