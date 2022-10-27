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

class particle
{
public: // CONSTRUCTOR
    particle(float mass, MyVector position, MyVector velocity, MyVector acceleration, float lifeSpan, bool stationary = false);
    particle();
public: // VARIABLES
    MyVector pos;
    MyVector velo;
    MyVector accel;
    MyVector forceAccum = MyVector();
    float mass = 5.0f;
    float damp = 0.99f;
    float radius = 1.0f;
    float lifeSpan = 5.0f;
    int type = 1;
    int count = 1;
    int material = 0;

    bool isDestroyed = false;
    bool IsDestroyed() {
        return isDestroyed;
    }
    float restitution = 1;
    bool stationary;
    MyVector startPos;
    void stationarySetting();

public:
    particle createBullet(int);
    particle fireBullet(std::vector<glm::mat4>*, std::vector<glm::mat4>*, GLuint, GLuint, int, glm::vec3);
    void deleteBullet(std::vector<glm::mat4>*, std::vector<glm::mat4>*, std::vector<particle>*, int);

public: // METHODS
    void updatePosition(float time);
    void updateVelocity(float time);

    void Update(float time);
    void Destroy();
    void checkLifeSpan(float time);
    void AddForce(MyVector force);
    void ResetForce();
};

#endif // !1