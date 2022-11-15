#pragma once

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
	MyParticle(MyVector newPos, MyVector newVelo, MyVector newAccel, MyVector newTotalVelo, MyVector newInitPos);
	~MyParticle();
public:
	string name;
	float mass;
	float radius;
	float restitution = 1;
	MyVector position;
	MyVector velocity;
	MyVector acceleration;
	MyVector totalVelocity;
	MyVector initialPos;

	float lifeSpan = 5.0f;
	int type = 1;
	int count = 1;
	int material = 0;

	bool isFinished = false;
	//EDIT DAMPING VALUE HERE
	float damp = 1.0f;

public:
	void update(float time);
	bool getIsDestroyed();
	void setIsDestroyed();
	void addForce(MyVector f);
	void resetForce();

private:
	void updatePos(float time);
	void updateVelocity(float time);

private:
	bool isDestroyed = false;
	MyVector accumulatedForce = MyVector(0, 0, 0);
};