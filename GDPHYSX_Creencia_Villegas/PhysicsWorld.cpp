#include "PhysicsWorld.h"

void PhysicsWorld::update(float time)
{
	updateParticleList();

	if (!particles.empty())
	{
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

	// Update particle positions for drawing
	for (int i = 0; i < particles.size(); i++) 
	{
		particleTrans[i] = glm::mat4(1.0f);
		particleTrans[i] = glm::scale(particleTrans[i], glm::vec3(0.5f, 0.5f, 0.5f));
		particleTrans[i] = glm::translate(particleTrans[i], glm::vec3(particles[i]->getVec3Pos()));
	}
}

void PhysicsWorld::addParticle(int pType, MyVector currParticlePos)
{
	MyParticle* newParticle = new MyParticle(pType);

	glm::mat4 trans = glm::mat4(1.0f); // identity
	if (pType == 0) trans = glm::scale(trans, glm::vec3(0.1f, 0.1f, 0.1f));
	else trans = glm::scale(trans, glm::vec3(0.5f, 0.5f, 0.5f));
	trans = glm::translate(trans, glm::vec3(currParticlePos.x, currParticlePos.y, currParticlePos.z));
	particleTrans.push_back(trans);

	glm::mat4 normalTrans;
	normalTransArray.push_back(normalTrans);


	//Gravity = GravityForceGenerator(newParticle->acceleration);

	particles.push_back(newParticle);
	registry.add(newParticle, &Gravity);

	//cout << endl << "particleTrans = " << particleTrans.size() << endl << "normalTransArray = " << normalTransArray.size() << endl;
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

void PhysicsWorld::updateParticleList()
{
	for (int i = 0; i < particles.size(); i++)
	{
		if (particles[i]->isDestroyed && particles[i]->type != 0)
		{
			registry.remove(particles[i], &Gravity);
			deleteParticle(i);
		}
	}
}

void PhysicsWorld::generateContacts()
{
	contacts.clear();
	getOverlaps();
	for (int i = 0; i < links.size(); i++)
	{
		ParticleContact* contact = links[i]->getContact();
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

float PhysicsWorld::getDistance(float xPos1, float yPos1, float zPos1, float xPos2, float yPos2, float zPos2)
{
	return sqrt(pow(xPos2 - xPos1, 2) + pow(yPos2 - yPos1, 2) + pow(zPos2 - zPos1, 2));
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
