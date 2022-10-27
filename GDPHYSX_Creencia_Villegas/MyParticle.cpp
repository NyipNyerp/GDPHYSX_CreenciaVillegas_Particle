#include "MyParticle.h"

// Determines Particle data for new bullets
MyParticle MyParticle::createBullet(int type)
{
	float randX = rand() % 60 + -30;
	float randY = rand() % 30 + -5;
	float randZ = rand() % 30 + -15;
	float randAge = rand() % 3 + 1;

	MyParticle newData;
	switch (type) {
	case 0:
		newData.velo = MyVector();
		newData.accel = MyVector();
		newData.mass = 5.0f;
		newData.damp = 1.0f;
		newData.radius = 1.0f;
		newData.accel.y *= newData.damp;
		newData.type = 0;
		newData.count = 1;
		newData.lifeSpan = 10000;
		break;
	case 1:
		newData.velo = MyVector(35.0f, 0.0f, 0.0f);
		newData.accel = MyVector(0.0f, -1.0f, 0.0f);
		newData.mass = 2.0f;
		newData.damp = 0.99f;
		newData.radius = 1.0f;
		newData.accel.y *= newData.damp;
		newData.type = 1;
		newData.count = 1;
		newData.lifeSpan = 5;
		break;
	case 2:
		newData.velo = MyVector(40.0f, 30.0f, 0.0f);
		newData.accel = MyVector(0.0f, -20.0f, 0.0f);
		newData.mass = 200.0f;
		newData.damp = 0.99f;
		newData.radius = 1.0f;
		newData.accel.y *= newData.damp;
		newData.type = 2;
		newData.count = 1;
		newData.lifeSpan = 5;
		break;
	case 3:
		newData.velo = MyVector(10.0f, 0.0f, 0.0f);
		newData.accel = MyVector(0.0f, 0.6f, 0.0f);
		newData.mass = 1.0f;
		newData.damp = 0.9f;
		newData.radius = 1.0f;
		newData.accel.y *= newData.damp;
		newData.type = 3;
		newData.count = 1;
		newData.lifeSpan = 5;
		break;
	case 4:
		newData.velo = MyVector(100.0f, 0.0f, 0.0f);
		newData.accel = MyVector(0.0f, 0.0f, 0.0f);
		newData.mass = 0.1f;
		newData.damp = 0.99f;
		newData.radius = 1.0f;
		newData.accel.y *= newData.damp;
		newData.type = 4;
		newData.count = 1;
		newData.lifeSpan = 5;
		break;
	case 5:
		newData.velo = MyVector(randX, 50.0f, 0.0f);
		newData.accel = MyVector(0.0f, 0.0f, 0.0f);
		newData.mass = 1.0;
		newData.damp = 0.99f;
		newData.radius = 1.0f;
		newData.accel.y *= newData.damp;
		newData.type = 5;
		newData.count = 5;
		newData.lifeSpan = 3;
		newData.material = 3;
		break;
	case 6:
		newData.velo = MyVector(randX, randY, randZ);
		newData.accel = MyVector(0.0f, -5.0f, 0.0f);
		newData.mass = 1.0;
		newData.damp = 0.99f;
		newData.radius = 1.0f;
		newData.accel.y *= newData.damp;
		newData.type = 6;
		newData.count = 10;
		newData.lifeSpan = randAge;
		newData.material = 2;
		break;
	case 7:
		newData.velo = MyVector(randX, randY, randZ);
		newData.accel = MyVector(0.0f, -10.0f, 0.0f);
		newData.mass = 1.0;
		newData.damp = 0.99f;
		newData.radius = 1.0f;
		newData.accel.y *= newData.damp;
		newData.type = 6;
		newData.count = 0;
		newData.lifeSpan = randAge;
		newData.material = 1;
		break;
	}

	return newData;
}

// Creates new Particles
MyParticle MyParticle::fireBullet(std::vector<glm::mat4>* MyParticleTrans, std::vector<glm::mat4>* normalTransArray, GLuint modelTransLoc, GLuint normTransLoc, int bType, glm::vec3 currParticlePos)
{
	MyParticle newParticle;
	newParticle = newParticle.createBullet(bType);

	glm::mat4 trans = glm::mat4(1.0f); // identity
	if (bType == 6) trans = glm::translate(trans, currParticlePos);
	else if (bType == 7) trans = glm::translate(trans, currParticlePos);
	if (bType == 0) trans = glm::scale(trans, glm::vec3(0.0f, 0.0f, 0.0f));
	else trans = glm::scale(trans, glm::vec3(0.5f, 0.5f, 0.5f));
	glm::mat4 normalTrans;

	MyParticleTrans->push_back(trans);
	normalTransArray->push_back(normalTrans);

	/*
	std::cout << std::endl
		<< "MyParticleTrans = " << MyParticleTrans->size() << std::endl
		<< "normalTransArray = " << normalTransArray->size() << std::endl
	*/

	return newParticle;
}

// Deletes existing Particles
void MyParticle::deleteBullet(std::vector<glm::mat4>* MyParticleTrans, std::vector<glm::mat4>* normalTransArray, std::vector<MyParticle>* MyParticleDatas, int index)
{
	MyParticleTrans->erase(MyParticleTrans->begin() + index);
	normalTransArray->erase(normalTransArray->begin() + index);
	MyParticleDatas->erase(MyParticleDatas->begin() + index);
}

void MyParticle::updatePosition(float time)
{
	//P2
	this->pos = this->pos + (this->velo * time) + ((this->accel * powf(time, 2)) * (1.0f / 2.0f));
}

void MyParticle::updateVelocity(float time)
{
	this->accel = this->accel + this->forceAccum * (1 / this->mass);
	this->velo = this->velo + this->accel * time;
	this->velo = this->velo * powf(this->damp, time);
}

void MyParticle::Update(float time)
{
	if (this->mass == 0) return;

	updatePosition(time);
	updateVelocity(time);
	checkLifeSpan(time);
	ResetForce();
	//exclusive for cable anchors
	if (stationary)
		stationarySetting();
}

void MyParticle::Destroy()
{
	this->isDestroyed = true;
}

void MyParticle::checkLifeSpan(float time)
{
	this->lifeSpan -= time;
	if (this->lifeSpan <= 0.0f) {
		Destroy();
	}
}

void MyParticle::AddForce(MyVector force)
{
	this->forceAccum = this->forceAccum + force;
}

void MyParticle::ResetForce()
{
	this->forceAccum = MyVector(0.0f, 0.0f, 0.0f);
	this->accel = MyVector(0.0f, 0.0f, 0.0f);
}

void MyParticle::stationarySetting()
{
	this->pos.x = startPos.x;
	this->pos.y = startPos.y;
	this->velo.x = 0;
	this->velo.y = 0;
}
