#pragma once
#ifndef PROJECTILE
#define PROJECTILE

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
//public: // CONSTRUCTOR

public: // VARIABLES
    struct particleData {
        glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 velo = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 accel = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 forceAccum = glm::vec3(0.0f, 0.0f, 0.0f);
        bool isDestroyed = false;
        float mass = 5.0f;
        float damp = 0.99f;
        float radius = 1.0f;
        float lifeSpan = 5.0f;
        int type = 1;
        int count = 1;
        int material = 0;
    } pData;

public:
    particleData createBullet(int);
    particleData fireBullet(std::vector<glm::mat4>*, std::vector<glm::mat4>*, GLuint, GLuint, int, glm::vec3);
    void deleteBullet(std::vector<glm::mat4>*, std::vector<glm::mat4>*, std::vector<particleData>*, int);

public: // METHODS
    void updatePosition(float time);
    void updateVelocity(float time);

    void Update(float time);
    void Destroy();
    void checkLifeSpan(float time);
    void AddForce(glm::vec3 force);
    void ResetForce();
};

#endif // !1