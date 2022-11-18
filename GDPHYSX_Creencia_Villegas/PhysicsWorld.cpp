#include "PhysicsWorld.h"


void PhysicsWorld::addParticle(int pType, glm::vec3 currParticlePos)
{
	MyParticle* newParticle = new MyParticle(pType);

	particles.push_back(newParticle);
	registry.add(newParticle, &Gravity);

	glm::mat4 trans = glm::mat4(1.0f); // identity
	trans = glm::translate(trans, currParticlePos);
	trans = glm::scale(trans, glm::vec3(0.5f, 0.5f, 0.5f));
	particleTrans.push_back(trans);

	glm::mat4 normalTrans;
	normalTransArray.push_back(normalTrans);

	cout << endl << "particleTrans = " << particleTrans.size() << endl << "normalTransArray = " << normalTransArray.size() << endl;
}

void PhysicsWorld::update(float time)
{
	updateParticleList();
	registry.updateForce(time);

	for (int i = 0; i < particles.size(); i++)
	{
		//particles[i]->acceleration = gravity;
		particles[i]->update(time);
	}

	generateContacts();

	if (contacts.size() > 0)
	{
		contactResolver.max_iterations = contacts.size() * 2;
		contactResolver.resolveContacts(contacts, time);
	}
}

void PhysicsWorld::addContact(MyParticle* p1, MyParticle* p2, float restitution, MyVector collisionNormal)
{
	ParticleContact* toAdd = new ParticleContact();

	toAdd->particles[0] = p1;
	toAdd->particles[1] = p2;
	toAdd->restitution = restitution;
	toAdd->collisonNormal = collisionNormal;

	contacts.push_back(toAdd);
}

void PhysicsWorld::deleteParticle(int index)
{
	particleTrans.erase(particleTrans.begin() + index);
	normalTransArray.erase(normalTransArray.begin() + index);
	particles.erase(particles.begin() + index);
}

void PhysicsWorld::clearParticles()
{
	particleTrans.clear();
	normalTransArray.clear();
	particles.clear();
}

void PhysicsWorld::updateParticleList()
{
	for (int i = 0; i < particles.size(); i++)
	{
		if (particles[i]->isDestroyed)
		{
			deleteParticle(i);
		}
	}
}

void PhysicsWorld::generateContacts()
{
	contacts.clear();
	getOverlaps();

	for (list<ParticleSpring*>::iterator i = links.begin(); i != links.end(); i++)
	{
		ParticleContact* contact = (*i)->getContact();
		if (contact != nullptr)
		{
			contacts.push_back(contact);
		}
	}
}

void PhysicsWorld::getOverlaps()
{
	for (int i = 0; i < particles.size() - 1; i++)
	{
		for (int h = i + 1; h < particles.size(); h++)
		{
			generateParticleContacts(particles[i], particles[h]); //particle contact resolution
		}
	}
}

void PhysicsWorld::generateParticleContacts(MyParticle* a, MyParticle* b)
{
	float mag2 = (a->position - b->position).magnitudeSquared();
	float rad2 = (a->radius + b->radius) * (a->radius + b->radius);

	if (mag2 <= rad2)
	{
		MyVector dir = a->position - b->position;
		dir.normalize();
		float r = rad2 - mag2;
		float depth = sqrt(r);

		float restitution = a->restitution;
		if (b->restitution < restitution)
			restitution = b->restitution;

		addContact(a, b, restitution, dir);
	}
}
