#pragma once
#include <list>
#include <vector>
#include "MyParticle.h"
#include "ForceRegistry.h"
#include "ForceGenerator.h"
#include "GravityForceGenerator.h"
#include "DragForceGenerator.h"
#include "ParticleContact.h"
#include "ContactResolver.h"
#include "ParticleSpring.h"
#include "AnchoredSpring.h"
#include "BungeeSpring.h"


using namespace std;

class PhysicsWorld
{
public:
	ForceRegistry registry;
	vector <MyParticle*> particles; // CHANGE FROM LIST TO VECTOR
	list <ParticleSpring*> links; // CHANGE FROM LIST TO VECTOR

	MyVector gravity = MyVector(0, 0, 0);  // 20m/s2 down

	vector<ParticleContact*> contacts;

	vector<glm::mat4> particleTrans;
	vector<glm::mat4> normalTransArray;

public:
	void addParticle(int bType, glm::vec3 currParticlePos);
	void update(float time);
	void addContact(MyParticle* p1, MyParticle* p2, float restitution, MyVector collisionNormal);

	void deleteParticle(int index);
	void clearParticles();

	float getDistance(float xPos1, float yPos1, float zPos1, float xPos2, float yPos2, float zPos2);


	// Put Mono's extra registry functions here


protected:
	GravityForceGenerator Gravity = GravityForceGenerator(gravity);
	void updateParticleList();
	ContactResolver contactResolver = ContactResolver(20);

	void generateContacts();
	void getOverlaps();


	//generating contacts for both particles
	void generateParticleContacts(MyParticle* a, MyParticle* b);
};

