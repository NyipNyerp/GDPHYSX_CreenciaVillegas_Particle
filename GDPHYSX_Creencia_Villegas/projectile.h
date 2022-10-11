#pragma once
#ifndef PROJECTILE
#define PROJECTILE

#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class projectile
{
public:
    struct projectileData {
        glm::vec3 accel;
        glm::vec3 velo;
        float mass;
        float damp;
        float radius;
        int type;
    } pData;

    projectileData createBullet(glm::vec3, glm::vec3, float, float, float, int);
};

#endif // !1