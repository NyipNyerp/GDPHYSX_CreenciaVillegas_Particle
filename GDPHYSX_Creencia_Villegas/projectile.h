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
    projectile();

    glm::vec3 accel;
    glm::vec3 velo;
    float mass;
    float damp;

    static void initialize();
    static projectile* getInstance();

private:
    static projectile* sharedInstance;
};

#endif // !1