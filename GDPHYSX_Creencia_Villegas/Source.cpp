#pragma once
#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#include "glm/glm.hpp"
#include "obj_mesh.h";
#include "shader.h"
#include "skybox.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "projectile.h"
#include <vector>

//YouTube. (2019). OpenGL - camera movement. YouTube. https://www.youtube.com/watch?v=AWM4CUfffos.

//camera
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
float yaw = -90.0f;
float pitch = 0.0f;
float fov = 45.0f;

//mouse state
bool firstMouse = true;
float lastX = 1024 / 2.0;
float lastY = 768 / 2.0;

void fireBullet(	std::vector<ObjData>* bulletsArray, std::vector<glm::mat4>* bulletsTrans,
					std::vector<glm::mat4>* normalTransArray, std::vector<GLuint>* textureArray,
					std::vector<glm::vec3>* currVelos, GLuint modelTransLoc, GLuint normTransLoc) {


	ObjData bullet;
	LoadObjFile(&bullet, "earth/Earth.obj");
	GLfloat bulletOffsets[] = { 0.0f, 0.0f, 0.0f };
	LoadObjToMemory(
		&bullet,
		1.0f,
		bulletOffsets
	);

	glm::mat4 trans = glm::mat4(1.0f); // identity
	trans = glm::mat4(1.0f); // identity
	trans = glm::scale(trans, glm::vec3(0.5f, 0.5f, 0.5f));
	glm::mat4 normalTrans;
	GLuint bulletTexture;

	bulletsArray->push_back(bullet);
	bulletsTrans->push_back(trans);
	normalTransArray->push_back(normalTrans);
	textureArray->push_back(bulletTexture);

	std::cout << std::endl	<< "bulletsArray" << bulletsArray->size() << std::endl
							<< "bulletsTrans" << bulletsTrans->size() << std::endl
							<< "normalTransArray" << normalTransArray->size() << std::endl
							<< "textureArray" << textureArray->size() << std::endl;
}

float getDistance(float xPos1, float yPos1, float zPos1, float xPos2, float yPos2, float zPos2) {
	return sqrt(pow(xPos2 - xPos1, 2) + pow(yPos2 - yPos1, 2) + pow(zPos2 - zPos1, 2));
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float mouseOffsetX = xpos - lastX;
	float mouseOffsetY = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.1f;
	mouseOffsetX *= sensitivity;
	mouseOffsetY *= sensitivity;

	yaw += mouseOffsetX;
	pitch += mouseOffsetY;

	if (pitch > 89.0f)
	{
		pitch = 89.0f;
	}
	if (pitch < -89.0f)
	{
		pitch = -89.0f;
	}



	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);
}

int main() {
	stbi_set_flip_vertically_on_load(true);
#pragma region Initialization
	//initialize glfw
	if (glfwInit() != GLFW_TRUE) {
		fprintf(stderr, "Failed to initialized! \n");
		return -1;
	}

	// set opengl version to 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// create window 
	GLFWwindow* window;
	window = glfwCreateWindow(1024, 768, "Creencia_Alain", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to load window! \n");
		return -1;
	}
	glfwMakeContextCurrent(window);

	//initialize glew
	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}
#pragma endregion


#pragma region Ballistics Array Declaration

	std::vector<ObjData> bulletsArray;
	std::vector<glm::mat4> bulletsTrans;
	std::vector<glm::mat4> normalTransArray;
	std::vector<GLuint> textureArray;
	std::vector<glm::vec3> currVelos;
	//Declaration for Collision
	projectile bullet1;
	projectile bullet2;
	projectile bullet3;
	projectile bullet4;
	projectile bullet5;
	projectile currBullet;


	bullet1.pData = bullet1.createBullet(glm::vec3(3, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 2.0f, 0.99f, 1.0f, 1);
	bullet2.pData = bullet2.createBullet(glm::vec3(6, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 2.0f, 0.99f, 1.0f, 2);
	bullet3.pData = bullet3.createBullet(glm::vec3(9, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 2.0f, 0.99f, 1.0f, 3);
	bullet4.pData = bullet4.createBullet(glm::vec3(12, 0.0f, 0.0f), glm::vec3(-0.0f, 0.0f, 0.0f), 2.0f, 0.99f, 1.0f, 4);
	bullet5.pData = bullet5.createBullet(glm::vec3(15, 0.0f, 0.0f), glm::vec3(-0.0f, 0.0f, 0.0f), 2.0f, 0.99f, 1.0f, 5);

#pragma endregion

#pragma region Mesh Loading


	std::vector<std::string> faces
	{
		"right.png",
		"left.png",
		"bottom.png",
		"top.png",
		"front.png",
		"back.png"
	};
	SkyBoxData skybox = LoadSkybox("Assets/skybox", faces);
#pragma endregion

#pragma region Shader Loading
	GLuint skyboxShaderProgram = LoadShaders("Shaders/skybox_vertex.shader", "Shaders/skybox_fragment.shader");
	//GLuint shaderProgram = LoadShaders("Shaders/phong_vertex.shader", "Shaders/phong_fragment.shader");
	//glUseProgram(shaderProgram2);
	GLuint shaderProgram = LoadShaders("Shaders/vertex.shader", "Shaders/fragment.shader");
	glUseProgram(shaderProgram);

	GLuint colorLoc = glGetUniformLocation(shaderProgram, "u_color");
	glUniform3f(colorLoc, 1.0f, 1.0f, 1.0f);


	// initialize MVP
	GLuint modelTransformLoc = glGetUniformLocation(shaderProgram, "u_model");
	GLuint viewLoc = glGetUniformLocation(shaderProgram, "u_view");
	GLuint projectionLoc = glGetUniformLocation(shaderProgram, "u_projection");

	GLuint normalTransformLoc = glGetUniformLocation(shaderProgram, "u_normal");
	GLuint cameraPosLoc = glGetUniformLocation(shaderProgram, "u_camera_pos");
	GLuint ambientColorLoc = glGetUniformLocation(shaderProgram, "u_ambient_color");
	glUniform3f(ambientColorLoc, 0.1f, 0.1f, 0.1f);

	fireBullet(&bulletsArray, &bulletsTrans, &normalTransArray, &textureArray, &currVelos, modelTransformLoc, normalTransformLoc);


	// define projection matrix
	glm::mat4 projection = glm::mat4(1.0f);
	//glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

	GLuint lightPosLoc = glGetUniformLocation(shaderProgram, "u_light_pos");
	GLuint lightDirLoc = glGetUniformLocation(shaderProgram, "u_light_dir");
	glUniform3f(lightDirLoc, 0.0f, 0.0f, 0.0f);


#pragma endregion

	// set bg color to green
	glClearColor(0.4f, 0.4f, 0.0f, 0.0f);


	// var for rotations
	float xFactor = 0.0f;
	float xSpeed = 1.0f;
	float currentTime = glfwGetTime();
	float prevTime = 0.0f;
	float deltaTime = 0.0f;
	float rotFactor = 0.0f;

	//Declaration for Particle motion
	bool isStart = true;
	bool isShoot = false;
	bool isCollide = false;

	float cooldown = 1;

	currBullet = bullet1;
	currVelos.push_back(bullet1.pData.velo);
	//depth testing
	glEnable(GL_DEPTH_TEST);

	//face culling
	glEnable(GL_CULL_FACE);

	while (!glfwWindowShouldClose(window)) {

#pragma region Viewport

		float ratio;
		int width, height;

		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float)height;

		glViewport(0, 0, width, height);

#pragma endregion

#pragma region Projection

		// Orthographic with corection for stretching, resize window to see difference with previous example
		projection = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, 0.1f, 100.0f);

		// Perspective Projection
		projection = glm::perspective(glm::radians(90.0f), ratio, 0.1f, 100.0f),
		// Set projection matrix in shader
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

#pragma endregion

#pragma region View

		glfwSetCursorPosCallback(window, mouse_callback);
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);


		//source: https://www.youtube.com/watch?v=AWM4CUfffos
		float cameraSpeed = deltaTime * 10;
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
			cameraPos += cameraSpeed * cameraFront;
		}
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
			cameraPos -= cameraSpeed * cameraFront;
		}
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
			cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		}
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
			cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		}
		if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) {
			cameraPos += cameraSpeed * cameraUp;
		}
		if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {
			cameraPos -= cameraSpeed * cameraUp;
		}

		glUniform3f(cameraPosLoc, cameraPos.x, cameraPos.y, cameraPos.y);
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));


#pragma endregion

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//toggle to render wit GL_FILL or GL_LINE
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

#pragma region Draw

		DrawSkybox(skybox, skyboxShaderProgram, view, projection);
		glUseProgram(shaderProgram);

		glBindVertexArray(bulletsArray[0].vaoId);
		glUseProgram(shaderProgram); //changes
		//////////////////////////////////////////////////
		
		glActiveTexture(GL_TEXTURE0);

		// Collider
		/*
		for (int i = 0; i < bulletsArray.size(); i++)
		{
			if (getDistance(bulletsTrans[i][3].x, bulletsTrans[i][3].y, bulletsTrans[i][3].z, colliderTrans[3].x, colliderTrans[3].y, colliderTrans[3].z) <= 2)
				std::cout << std::endl << "NAGCOLLIDE NA" << std::endl;
			else 
				std::cout << std::endl << "Not colliding" << std::endl;
		}
		*/

		// transforms
		if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) currBullet = bullet1;
		if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) currBullet = bullet2;
		if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) currBullet = bullet3;
		if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) currBullet = bullet4;
		if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS) currBullet = bullet5;

		if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) bulletsTrans[0] = glm::rotate(bulletsTrans[0], glm::radians(rotFactor), glm::vec3(0.0f, 1.0f, 0.0f)); // matrix * rotation_matrix

		cooldown += cooldown * deltaTime;
		if (cooldown >= 4)
		{
			if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
				fireBullet(&bulletsArray, &bulletsTrans, &normalTransArray, &textureArray, &currVelos, modelTransformLoc, normalTransformLoc);
				if (currBullet.pData.type == 1) currVelos.push_back(bullet1.pData.velo);
				else if (currBullet.pData.type == 2) currVelos.push_back(bullet2.pData.velo);
				else if (currBullet.pData.type == 3) currVelos.push_back(bullet3.pData.velo);
				else if (currBullet.pData.type == 4) currVelos.push_back(bullet4.pData.velo);
				else if (currBullet.pData.type == 5) currVelos.push_back(bullet5.pData.velo);
				cooldown = 1;
			}
		}

		

		for (int i = 0; i < bulletsArray.size(); i++) {
			currVelos[i] += currBullet.pData.accel * deltaTime;
			bulletsTrans[i] = glm::translate(bulletsTrans[i], currVelos[i] * deltaTime); // matrix * translate_matrix
			std::cout << std::endl << "currVELOS" << i << " = " << currVelos[i].x << ", " << currVelos[i].y << ", " << currVelos[i].z << std::endl;
			std::cout << "BULLETTRANS" << i << " = " << bulletsTrans[i][3].x << ", " << bulletsTrans[i][3].y << ", " << bulletsTrans[i][3].z << std::endl;

			normalTransArray[i] = glm::transpose(glm::inverse(bulletsTrans[i]));
			glUniformMatrix4fv(normalTransformLoc, 1, GL_FALSE, glm::value_ptr(normalTransArray[i]));
			glUniformMatrix4fv(modelTransformLoc, 1, GL_FALSE, glm::value_ptr(bulletsTrans[i]));

			textureArray[i] = bulletsArray[i].textures[bulletsArray[i].materials[0].diffuse_texname];
			glBindTexture(GL_TEXTURE_2D, textureArray[i]);

			glDrawElements(GL_TRIANGLES, bulletsArray[i].numFaces, GL_UNSIGNED_INT, (void*)0);
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////


		//unbindtexture after rendering
		glBindTexture(GL_TEXTURE_2D, 0);

		currentTime = glfwGetTime();
		deltaTime = currentTime - prevTime;
		rotFactor += deltaTime * 900;
		if (rotFactor > 360.0f) {
			rotFactor -= 360.0f;
		}
		prevTime = currentTime;

		//--- stop drawing here ---
#pragma endregion

		glfwSwapBuffers(window);
		//listen for glfw input events
		glfwPollEvents();
	}
	return 0;
}
