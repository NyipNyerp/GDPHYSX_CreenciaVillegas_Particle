#pragma once
#include <list>
#include <vector>
#include "Utils.h"
#include "MyParticle.h"
#include "ForceRegistry.h"
#include "ForceGenerator.h"
#include "GravityForceGenerator.h"
#include "DragForceGenerator.h"
#include "ParticleContact.h"
#include "ContactResolver.h"
#include "ParticleLink.h"
#include "BoxRB.h"
#include "SphereRB.h"

using namespace std;

class PhysicsWorld
{
public:
	ForceRegistry registry;
	vector <MyParticle*> particles; // CHANGE FROM LIST TO VECTOR

	vector<MyParticle*> boxParticles;
	vector <ParticleLink*> links; 

	MyVector gravity = MyVector(0, 0, 0);  // 20m/s2 down

	vector<ParticleContact*> contacts;

	vector<glm::mat4> particleTrans;
	vector<glm::mat4> normalTransArray;

public:
	void update(float time);

	void addParticle(int bType, MyVector currParticlePos);
	void addExistingParticle(MyParticle* particle);
	void addContact(MyParticle* p1, MyParticle* p2, float restitution, MyVector collisionNormal);

	void deleteParticle(int index);

	float getDistance(float xPos1, float yPos1, float zPos1, float xPos2, float yPos2, float zPos2);

protected:
	GravityForceGenerator Gravity = GravityForceGenerator(gravity);
	void updateParticleList();
	ContactResolver contactResolver = ContactResolver(20);

	void generateContacts();
	void getOverlaps();


	//generating contacts for both particles
	void generateParticleContacts(MyParticle* a, MyParticle* b);

	//generating contacts for RBs
	void generateRigidbodyContacts(MyParticle* a, MyParticle* b);

	//processing contacts
	void processRigidBodyContact(BoxRB* a, MyParticle* b);
};

