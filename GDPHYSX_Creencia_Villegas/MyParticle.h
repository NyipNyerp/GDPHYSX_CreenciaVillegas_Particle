#pragma once
#ifndef MYPARTICLE_H
#define MYPARTICLE_H

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
	float mass = 5;
	float radius = 2.5;
	float damping = 0.99f;
	float restitution = 1;
	MyVector position = MyVector(0, 0, 0);
	MyVector velocity = MyVector(0, 0, 0);
	MyVector acceleration = MyVector(0, 0, 0);
	MyVector initialPos = MyVector(0, 0, 0);

	float lifeSpan = 5;
	int type = 1;
	int count = 1;
	int material = 0;
	bool isDestroyed = false;

public:
	MyParticle(int newType);
	void particleType();

	void update(float time);
	void addForce(MyVector f);
	void resetForce();
	void checkLifeSpan(float time);

	glm::vec3 getVec3Pos();

private:
	void updatePos(float time);
	void updateVelocity(float time);

protected:
	MyVector accumulatedForce = MyVector(0, 0, 0);
};

#endif