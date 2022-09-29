#pragma once
#include<stdio.h>
#include<iostream>
#include<vector>
#include<GL/glew.h>
#include "obj_mesh.h"
#include <glm/gtc/type_ptr.hpp>

const float skyboxVertices[] = {
    // positions          
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f
};

struct SkyBoxData
{
    GLuint VAO;
    GLuint textureID;
};

SkyBoxData LoadSkybox(std::string baseDir, std::vector<std::string> faces)
{
    //load textures
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int w, h;
    int comp;

    std::string textureFileName;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        textureFileName = baseDir + "/" + faces[i];
        if (!FileExists(textureFileName))
        {
            textureFileName = textureFileName;
            if (!FileExists(textureFileName))
            {
                std::cerr << "Unable to find file " << textureFileName << std::endl;
                exit(1);
            }
        }

        unsigned char* image = stbi_load(textureFileName.c_str(), &w, &h, &comp, STBI_rgb);

        if (!image)
        {
            std::cerr << "Unable to load texture " << textureFileName << std::endl;
            exit(1);
        }

        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
        0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        stbi_image_free(image);

    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    //load cube
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    return
    {
        VAO, 
        textureID
    };
}

void DrawSkybox(SkyBoxData skybox, GLuint shaderProgram, glm::mat4 view, glm::mat4 projection)
{
    GLuint viewLoc = glGetUniformLocation(shaderProgram, "u_view");
    GLuint projectionLoc = glGetUniformLocation(shaderProgram, "u_projection");

    //draw skybox as last
    glDepthMask(GL_FALSE);
    //don't draw front face
    glCullFace(GL_BACK);
    glUseProgram(shaderProgram);
    view = glm::mat4(glm::mat3(view));

    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    // skybox cube
    glBindVertexArray(skybox.VAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.textureID);

    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    //revert
    glDepthMask(GL_TRUE);
    glCullFace(GL_BACK);
}