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
        glm::vec3 accel;
        glm::vec3 velo;
        float mass;
        float damp;
        float radius;
        int type;
    } pData;

    projectileData createBullet(int);
    projectileData fireBullet(std::vector<ObjData>*, std::vector<glm::mat4>*, std::vector<glm::mat4>*, std::vector<GLuint>*, GLuint, GLuint, int);
};

#endif // !1