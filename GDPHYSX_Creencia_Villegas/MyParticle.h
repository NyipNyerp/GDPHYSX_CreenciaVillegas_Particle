#pragma once
#ifndef PROJECTILE
#define PROJECTILE

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
public: // CONSTRUCTOR
   // MyParticle(float mass, MyVector position, MyVector velocity, MyVector acceleration, float lifeSpan, bool stationary = false);
   // MyParticle();

    //public: // VARIABLES
    MyVector pos;
    MyVector velo;
    MyVector accel;
    MyVector forceAccum = MyVector(0,0,0);
    //float mass = 5.0f;
    //float damp = 0.99f;
    //float radius = 1.0f;
    float lifeSpan = 5.0f;
    int type = 1;
    int count = 1;
    int material = 0;

public:
	string name;
	float mass;
	float radius;
	float restitution = 1;
	MyVector position = MyVector(0, 0,0);
	MyVector velocity = MyVector(0, 0,0);
	MyVector acceleration = MyVector(0, 0,0);
	MyVector totalVelocity = MyVector(0, 0,0);
	MyVector initialPos = MyVector(0, 0,0);

	bool isFinished = false;
	//EDIT DAMPING VALUE HERE
	float damp = 1.0f;

public:
	void update(float time);
	float measureTime();
	bool getIsDestroyed();
	void setIsDestroyed();
	void addForce(MyVector f);
	void resetForce();
	MyParticle() = default;


private:
	void updatePos(float time);
	void updateVelocity(float time);
	//void updateDestroyed();

private:
	bool isDestroyed = false;
	MyVector accumulatedForce = MyVector(0, 0,0);
};

#endif // !1