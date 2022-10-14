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
#include "main.h"

class projectile
{
public:
    struct projectileData {
        glm::vec3 velo;
        glm::vec3 accel;
        float mass;
        float damp;
        float radius;
        int type;
        int count;
        float ageLimit;
        int material = 0;
    } pData;

    projectileData createBullet(int);
    projectileData fireBullet(std::vector<glm::mat4>*, std::vector<glm::mat4>*, GLuint, GLuint, int, glm::vec3);
    void deleteBullet(std::vector<glm::mat4>*, std::vector<glm::mat4>*, std::vector<projectileData>*, int);
};

#endif // !1