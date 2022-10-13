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
        int count = 1;
        float ageLimit = 3;
    } pData;

    projectileData createBullet(int);
    projectileData fireBullet(std::vector<ObjData>*, std::vector<glm::mat4>*, std::vector<glm::mat4>*, std::vector<GLuint>*, GLuint, GLuint, int, glm::vec3);
    void deleteBullet(std::vector<ObjData>*, std::vector<glm::mat4>*, std::vector<glm::mat4>*, std::vector<GLuint>*, std::vector<projectileData>*, int);
};

#endif // !1