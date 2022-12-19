#include "PhysicsWorld.h"

void PhysicsWorld::update(float time)
{
	updateParticleList();

	if (!particles.empty())
	{
		//cout << "TEST" << endl;
		registry.updateForce(time);

		for (int i = 0; i < particles.size(); i++)
		{
			//cout << "particle position = " << particles[i]->position.x << ", " << particles[i]->position.y << ", " << particles[i]->position.z << endl;
			//particles[i]->acceleration = gravity;
			particles[i]->update(time);

			if (particles[i]->type == 0) 
			{
				BoxRB* cube = dynamic_cast<BoxRB*>(particles[i]);
				cube->updateBoxPos();
			}
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
		//if (particles[i]->type == 0) particleTrans[i] = glm::scale(particleTrans[i], glm::vec3(1.0f, 1.0f, 1.0f));
		particleTrans[i] = glm::scale(particleTrans[i], glm::vec3(0.5f, 0.5f, 0.5f));
		particleTrans[i] = glm::translate(particleTrans[i], glm::vec3(particles[i]->getVec3Pos()));
	}
}

void PhysicsWorld::addParticle(int pType, MyVector currParticlePos)
{
	SphereRB* newParticle = new SphereRB(pType);

	glm::mat4 trans = glm::mat4(1.0f); // identity
	//if (pType == 0) trans = glm::scale(trans, glm::vec3(1.0f, 1.0f, 1.0f));
	trans = glm::scale(trans, glm::vec3(0.5f, 0.5f, 0.5f));
	trans = glm::translate(trans, glm::vec3(currParticlePos.x, currParticlePos.y, currParticlePos.z));
	particleTrans.push_back(trans);

	glm::mat4 normalTrans;
	normalTransArray.push_back(normalTrans);


	//Gravity = GravityForceGenerator(newParticle->acceleration);

	particles.push_back(newParticle);
	registry.add(newParticle, &Gravity);

	//cout << endl << "particleTrans = " << particleTrans.size() << endl << "normalTransArray = " << normalTransArray.size() << endl;
}

void PhysicsWorld::addExistingParticle(MyParticle* particle)
{
	glm::mat4 trans = glm::mat4(1.0f); // identity
	trans = glm::scale(trans, glm::vec3(0.5f, 0.5f, 0.5f));
	trans = glm::translate(trans, glm::vec3(particle->position.x, particle->position.y, particle->position.z));
	particleTrans.push_back(trans);

	glm::mat4 normalTrans;
	normalTransArray.push_back(normalTrans);

	particles.push_back(particle);
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
			if (particles[i]->type == 0 || particles[h]->type == 0) // if either particle is of type BOX
				generateRigidbodyContacts(particles[i], particles[h]); //rigidbodies contact resolution
			else 
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

		if (a->type != 0 && b->type == 0) // if particleA is a sphere and particleB is a box
		{
			a->isDestroyed = true;
			b->isDestroyed = true;
		}
		else if (a->type == 0 && b->type != 0) // if particleA is a box and particleB is a sphere
		{
			a->isDestroyed = true;
			b->isDestroyed = true;
		}
	}
}
void PhysicsWorld::generateRigidbodyContacts(MyParticle* a, MyParticle* b)
{
	//if particles are both spheres
	if (a->type != 0 && b->type != 0)
	{
		generateParticleContacts(a, b);
	}
	//if mixed
	else
	{
		BoxRB* rect = dynamic_cast<BoxRB*>(a);
		if (rect == nullptr)
		{
			rect = dynamic_cast<BoxRB*>(b);
			processRigidBodyContact(rect, a); //a is a sphere
		}
		else
		{
			processRigidBodyContact(rect, b); //b is a sphere
		}
	}
}

void PhysicsWorld::processRigidBodyContact(BoxRB* a, MyParticle* b)
{
	MyVector relVector = b->position - a->position;
	float invAngle = -a->rotation;
	MyVector locVector = Utils::rotatePoint(relVector, invAngle);

	float minX = locVector.x;
	if ((a->length / 2) < minX)
	{
		minX = a->length / 2;
	}

	float maxX = minX;
	if (maxX < -a->length / 2)
	{
		maxX = -a->length / 2;
	}

	float minY = locVector.y;
	if ((a->length / 2) < minY)
	{
		minY = a->length / 2;
	}

	float maxY = minY;
	if (maxY < -a->length / 2)
	{
		maxY = -a->length / 2;
	}

	float D_X = locVector.x - maxX;
	float D_Y = locVector.y - maxY;

	bool col = (D_X * D_X + D_Y * D_Y) <= (b->radius * b->radius);

	//collision is detected
	if (col)
	{
		MyVector dir = a->position - b->position;
		dir.normalize();

		float restitution = a->restitution;
		if (b->restitution < a->restitution)
		{
			restitution = b->restitution;
		}

		addContact(a, b, restitution, dir);

		if (a->type != 0 && b->type == 0) // if particleA is a sphere and particleB is a box
		{
			a->isDestroyed = true;
			b->isDestroyed = true;
		}
		else if (a->type == 0 && b->type != 0) // if particleA is a box and particleB is a sphere
		{
			a->isDestroyed = true;
			b->isDestroyed = true;
		}
	}
}
