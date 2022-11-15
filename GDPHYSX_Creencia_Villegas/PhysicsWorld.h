#pragma once
#include<list>
#include"MyParticle.h"
#include "ForceRegistry.h"
#include "GravityForceGenerator.h"
#include <vector>
#include "ParticleContact.h"
#include "ContactResolver.h"
#include "ParticleLink.h"

using namespace std;

class PhysicsWorld
{
public:
	ForceRegistry registry;
	list <MyParticle*> particles;
	list <ParticleLink*> links;

	//(DEPRECATED) Change Gravity Value here
	//MyVector gravity = MyVector(0, 0, 0);  // 20m/s2 down

	vector<ParticleContact*> contacts;

public:
	void addParticle(MyParticle* particle);
	void update(float time);
	void addContact(MyParticle* p1, MyParticle* p2, float restitution, MyVector collisionNormal);

protected:
	GravityForceGenerator Gravity = GravityForceGenerator();
	void updateParticleList();
	ContactResolver contactResolver = ContactResolver(20);

	void generateContacts();
	void getOverlaps();
};