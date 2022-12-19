#pragma once
#ifndef MYPARTICLE_H
#define MYPARTICLE_H

#include "Utils.h"
#include "MyVector.h"
#include <stdio.h>
#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class MyParticle
{
public:
	string name = "particle";
	float mass = 5;
	float radius = 2.5;
	float damping = 0.99f;
	float restitution = 1;
	MyVector position = MyVector(0, 0, 0);
	MyVector velocity = MyVector(0, 0, 0);
	MyVector acceleration = MyVector(0, 0, 0);
	MyVector initialPos = MyVector(0, 0, 0);

	float angularDamping = 0.9f;
	float angularVelocity = 0;
	float rotation = 0;

	float lifeSpan = 5;
	int type = 1;
	int count = 1;
	int material = 0;
	bool isDestroyed = false;

public:
	MyParticle(int newType);
	MyParticle();
	void particleType();

	void update(float time);
	void addForce(MyVector f);
	void resetForce();
	void checkLifeSpan(float time);

	glm::vec3 getVec3Pos();

	virtual void addForceAtPoint(MyVector point, MyVector f) = 0;
	float momentOfInertia();

private:
	void updatePos(float time);
	void updateVelocity(float time);

protected:
	MyVector accumulatedForce = MyVector(0, 0, 0);
	float accumulatedTorque = 0;
};

#endif