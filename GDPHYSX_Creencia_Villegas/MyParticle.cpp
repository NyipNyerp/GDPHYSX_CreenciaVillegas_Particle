#include "MyParticle.h"

MyParticle::MyParticle(int newType) : type(newType)
{
	particleType();
}

void MyParticle::particleType()
{
	float randX = rand() % 60 + -30;
	float randY = rand() % 30 + -5;
	float randZ = rand() % 30 + -15;
	float randAge = rand() % 3 + 1;

	switch (type) {
	case 0: // BOX PARTICLES
		velocity = MyVector(0.0f, 0.0f, 0.0f);
		acceleration = MyVector(0.0f, 0.0f, 0.0f);
		mass = 5.0f;
		damping = 0.99f;
		count = 1;
		break;
	case 1: // BULLET
		velocity = MyVector(35.0f, 0.0f, 0.0f);
		acceleration = MyVector(0.0f, -1.0f, 0.0f);
		mass = 2.0f;
		damping = 0.99f;
		count = 1;
		lifeSpan = 5;
		break;
	case 2: // ARTILLERY
		velocity = MyVector(40.0f, 30.0f, 0.0f);
		acceleration = MyVector(0.0f, -20.0f, 0.0f);
		mass = 200.0f;
		damping = 0.99f;
		count = 1;
		lifeSpan = 50;
		break;
	case 3: // FIREBALL
		velocity = MyVector(10.0f, 0.0f, 0.0f);
		acceleration = MyVector(0.0f, 0.6f, 0.0f);
		mass = 1.0f;
		damping = 0.9f;
		count = 1;
		lifeSpan = 5;
		break;
	case 4: // LASER
		velocity = MyVector(100.0f, 0.0f, 0.0f);
		acceleration = MyVector(0.0f, 0.0f, 0.0f);
		mass = 0.1f;
		damping = 0.99f;
		count = 1;
		lifeSpan = 5;
		break;
	case 5: // FIREWORKS TYPE A
		velocity = MyVector(randX, 50.0f, 0.0f);
		acceleration = MyVector(0.0f, 0.0f, 0.0f);
		mass = 1.0;
		damping = 0.99f;
		count = 5;
		lifeSpan = 3;
		material = 3;
		break;
	case 6: // FIREWORKS TYPE B
		velocity = MyVector(randX, randY, randZ);
		acceleration = MyVector(0.0f, -5.0f, 0.0f);
		mass = 1.0;
		damping = 0.99f;
		count = 10;
		lifeSpan = randAge;
		material = 2;
		break;
	case 7: // FIREWORKS TYPE C
		velocity = MyVector(randX, randY, randZ);
		acceleration = MyVector(0.0f, -10.0f, 0.0f);
		mass = 1.0;
		damping = 0.99f;
		count = 0;
		lifeSpan = randAge;
		material = 1;
		break;
	}
}

void MyParticle::update(float time)
{
	if (mass == 0)
	{
		return;
	}
	checkLifeSpan(time);
	updatePos(time);
	updateVelocity(time);
	cout << "particle position = " << position.x << ", " << position.y << ", " << position.z << endl;
	//cout << "velocity = " << velocity.x << ", " << velocity.y << ", " << velocity.z << endl << endl;
	//cout << "acceleration = " << acceleration.x << ", " << acceleration.y << ", " << acceleration.z << endl;
	//cout << "accumulatedForce = " << accumulatedForce.x << ", " << accumulatedForce.y << ", " << accumulatedForce.z << endl;
	//cout << "totalVelocity = " << totalVelocity.x << ", " << totalVelocity.y << ", " << totalVelocity.z << endl;
	resetForce();
}

void MyParticle::updatePos(float time)
{
	position = position + (velocity * time) + ((acceleration * powf(time, 2)) * (1 / 2));
}

void MyParticle::updateVelocity(float time)
{	
	MyVector tempAccel = acceleration;
	tempAccel = tempAccel + (accumulatedForce * (1 / mass));

	velocity = velocity + (tempAccel * time);

	velocity = velocity * powf(damping, time);
}

void MyParticle::addForce(MyVector f)
{
	accumulatedForce = accumulatedForce + f;
}

void MyParticle::resetForce()
{
	accumulatedForce = MyVector(0, 0, 0);
	//acceleration = MyVector(0, 0, 0);
}

void MyParticle::checkLifeSpan(float time)
{
	lifeSpan -= time;

	//cout << "lifespan = " << lifeSpan << endl;

	if (lifeSpan <= 0) isDestroyed = true;
}

glm::vec3 MyParticle::getVec3Pos()
{
	return glm::vec3(position.x, position.y, position.z);
}
