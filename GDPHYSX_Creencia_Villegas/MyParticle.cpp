#include "MyParticle.h"

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
	//checkLifeSpan(time);
	//ResetForce();
	//exclusive for cable anchors
	//if (stationary)
	//	stationarySetting();
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
