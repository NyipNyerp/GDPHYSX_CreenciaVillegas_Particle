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


#pragma region Mesh Loading

	ObjData backpack;
	LoadObjFile(&backpack, "earth/Earth.obj");
	GLfloat bunnyOffsets[] = { 0.0f, 0.0f, 0.0f };
	LoadObjToMemory(
		&backpack,
		1.0f,
		bunnyOffsets
	);

	ObjData platform;
	LoadObjFile(&platform, "earth/Earth.obj");
	GLfloat platformOffsets[] = { 0.0f, 0.0f, 0.0f };
	LoadObjToMemory(
		&platform,
		1.0f,
		platformOffsets
	);

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

	glm::mat4 trans = glm::mat4(1.0f); // identity
	glm::mat4 trans2 = glm::mat4(1.0f); // identity
	glm::mat4 trans3 = glm::mat4(1.0f); // identity


	glUniformMatrix4fv(modelTransformLoc, 1, GL_FALSE, glm::value_ptr(trans));
	glUniformMatrix4fv(modelTransformLoc, 1, GL_FALSE, glm::value_ptr(trans2));
	glUniformMatrix4fv(modelTransformLoc, 1, GL_FALSE, glm::value_ptr(trans3));

	// define projection matrix
	glm::mat4 projection = glm::mat4(1.0f);
	//glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

	GLuint lightPosLoc = glGetUniformLocation(shaderProgram, "u_light_pos");
	GLuint lightDirLoc = glGetUniformLocation(shaderProgram, "u_light_dir");
	glUniform3f(lightPosLoc, -trans2[3][0], -trans2[3][1], -trans2[3][2] + 2);
	glUniform3f(lightDirLoc, 0.0f, 0.0f, 0.0f);


#pragma endregion

	// set bg color to green
	glClearColor(0.4f, 0.4f, 0.0f, 0.0f);

	trans = glm::mat4(1.0f); // identity
	trans = glm::scale(trans, glm::vec3(0.5f, 0.5f, 0.5f)); // matrix * scale_matrix
	trans2 = glm::mat4(1.0f); // identity
	trans2 = glm::translate(trans2, glm::vec3(30, 0.0f, 0.0f)); // matrix * translate_matrix

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
	
	//Declaration for Collision
	projectile bullet1;
	projectile bullet2;
	projectile bullet3;
	projectile bullet4;
	projectile bullet5;
	projectile currBullet;


	bullet1.pData = bullet1.createBullet(glm::vec3(10, 0.0f, 0.0f), glm::vec3(-5.0f, 0.0f, 0.0f), 2.0f, 0.99f, 1.0f);
	bullet2.pData = bullet2.createBullet(glm::vec3(30, 0.0f, 0.0f), glm::vec3(-5.0f, 0.0f, 0.0f), 2.0f, 0.99f, 1.0f);
	bullet3.pData = bullet3.createBullet(glm::vec3(30, 0.0f, 0.0f), glm::vec3(-5.0f, 0.0f, 0.0f), 2.0f, 0.99f, 1.0f);
	bullet4.pData = bullet4.createBullet(glm::vec3(30, 0.0f, 0.0f), glm::vec3(-5.0f, 0.0f, 0.0f), 2.0f, 0.99f, 1.0f);
	bullet5.pData = bullet5.createBullet(glm::vec3(30, 0.0f, 0.0f), glm::vec3(-5.0f, 0.0f, 0.0f), 2.0f, 0.99f, 1.0f);

	glm::vec3 currVelo = glm::vec3(0.0f, 0.0f, 0.0f);

	/*
	bullet1.pData.velo = glm::vec3(30, 0.0f, 0.0f);
	bullet1.pData.accel = glm::vec3(-5.0f, 0.0f, 0.0f);
	bullet1.pData.mass = 2.0f; // kg
	bullet1.pData.damp = 0.99f; // value between 0 and 1
	
	bullet2.velo = glm::vec3(-35, 0.0f, 0.0f);
	bullet2.accel = glm::vec3(0.0f, 19.6f, 0.0f);
	bullet2.mass = 2.0f;
	bullet2.damp = 0.99f;

	bullet3.velo = glm::vec3(5, 0.0f, 0.0f);
	bullet3.accel = glm::vec3(0.0f, -19.6f, 0.0f);
	bullet3.mass = 2.0f;
	bullet3.damp = 0.99f;

	bullet4.velo = glm::vec3(-5, 0.0f, 0.0f);
	bullet4.accel = glm::vec3(0.0f, -19.6f, 0.0f);
	bullet4.mass = 2.0f;
	bullet4.damp = 0.99f;

	bullet5.velo = glm::vec3(100, 0.0f, 0.0f);
	bullet5.accel = glm::vec3(0.0f, 100.0f, 0.0f);
	bullet5.mass = 2.0f;
	bullet5.damp = 0.99f;
	*/

	currBullet = bullet1;
	currVelo = currBullet.pData.velo;

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
		// Orthopgraphic projection but make units same as pixels. origin is lower left of window
		// projection = glm::ortho(0.0f, (GLfloat)width, 0.0f, (GLfloat)height, 0.1f, 10.0f); // when using this scale objects really high at pixel unity size

		// Orthographic with stretching
		//projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 10.0f);

		// Orthographic with corection for stretching, resize window to see difference with previous example
		projection = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, 0.1f, 100.0f);
		// Set projection matrix in shader
		//glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

		//if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		//{
			// Perspective Projection
		projection = glm::perspective(glm::radians(90.0f), ratio, 0.1f, 100.0f),
			// Set projection matrix in shader
			glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
		//}



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

		//draw bunny
		glBindVertexArray(backpack.vaoId);
		glUseProgram(shaderProgram); //changes
		//////////////////////////////////////////////////
		
		if (getDistance(trans[3].x, trans[3].y, trans[3].z, trans2[3].x, trans2[3].y, trans2[3].z) <= 2)
		{
			std::cout << std::endl << "NAGCOLLIDE NA PUTANGINAMO" << std::endl;
		}
		else std::cout << std::endl << "BallsNiJR" << std::endl;
		
		// BALL CURRENT POS
		//std::cout << std::endl << trans[3].x << std::endl;
		//std::cout << trans[3].y << std::endl;
		//std::cout << trans[3].z << std::endl;

		// transforms
		if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) currBullet = bullet1;
		if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) currBullet = bullet2;
		if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) currBullet = bullet3;
		if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) currBullet = bullet4;
		if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS) currBullet = bullet5;

		//Q W E
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) { // reset ball position to origin
			trans = glm::mat4(1.0f);
			trans = glm::translate(trans, glm::vec3(0.0f, 0.0f, 0.0f)); // matrix * translate_matrix
			trans = glm::scale(trans, glm::vec3(0.5f, 0.5f, 0.5f));
			currVelo = glm::vec3(0.0f, 0.0f, 0.0f);
			isShoot = false;
		}

		if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) trans = glm::rotate(trans, glm::radians(rotFactor), glm::vec3(0.0f, 1.0f, 0.0f)); // matrix * rotation_matrix

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) isShoot = true; // push ball
		if (isShoot) {
			currVelo += currBullet.pData.accel * deltaTime;
			trans = glm::translate(trans, currVelo * deltaTime); // matrix * translate_matrix
		}
		
		//send to shader
		glm::mat4 normalTrans = glm::transpose(glm::inverse(trans));
		glUniformMatrix4fv(normalTransformLoc, 1, GL_FALSE, glm::value_ptr(normalTrans));
		glUniformMatrix4fv(modelTransformLoc, 1, GL_FALSE, glm::value_ptr(trans));


		glActiveTexture(GL_TEXTURE0);
		GLuint backpackTexture = backpack.textures[backpack.materials[0].diffuse_texname];
		glBindTexture(GL_TEXTURE_2D, backpackTexture);

		//drawbackpack
		glDrawElements(GL_TRIANGLES, backpack.numFaces, GL_UNSIGNED_INT, (void*)0);



		//send to shader
		glm::mat4 normalTrans2 = glm::transpose(glm::inverse(trans2));
		glUniformMatrix4fv(normalTransformLoc, 1, GL_FALSE, glm::value_ptr(normalTrans2));
		glUniformMatrix4fv(modelTransformLoc, 1, GL_FALSE, glm::value_ptr(trans2));


		glActiveTexture(GL_TEXTURE0);
		GLuint platformTexture = platform.textures[platform.materials[0].diffuse_texname];
		glBindTexture(GL_TEXTURE_2D, platformTexture);

		//drawbackpack
		glDrawElements(GL_TRIANGLES, platform.numFaces, GL_UNSIGNED_INT, (void*)0);

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
