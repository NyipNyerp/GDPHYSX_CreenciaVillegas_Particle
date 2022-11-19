#pragma once
#include "PhysicsWorld.h"
#include "main.h"
#include <algorithm>

// CHANGE NORMALTRANSARRAY AND PARTICLETRANS IN PHYSICSWORLD TO MYPARTICLE

// TRY CHANGING MYVECTOR USES TO GLM::VEC3

// CHANGE PARTICLESPRING CLASS NAME TO PARTICLELINKS

#pragma region CAMERA 
//YouTube. (2019). OpenGL - camera movement. YouTube. https://www.youtube.com/watch?v=AWM4CUfffos.
//camera
glm::vec3 cameraPos = glm::vec3(10.0f, 10.0f, 30.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
float yaw = -90.0f;
float pitch = 0.0f; 
float fov = 45.0f;

//mouse state
bool firstMouse = true; 
float lastX = 1024 / 2.0;
float lastY = 768 / 2.0;


// Mouse camera controls
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

#pragma endregion


//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
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

	ObjData particle;
	LoadObjFile(&particle, "earth/Earth.obj");
	GLfloat particleOffsets[] = { 0.0f, 0.0f, 0.0f };
	LoadObjToMemory(
		&particle,
		1.0f,
		particleOffsets
	);

	ObjData box;
	LoadObjFile(&box, "box/12279_Container_v1_l1.obj");
	GLfloat boxOffsets[] = { 0.0f, 0.0f, 0.0f };
	LoadObjToMemory(
		&box,
		0.2f,
		boxOffsets
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


	glm::mat4 boxTrans = glm::mat4(1.0f); // identity
	boxTrans = glm::translate(boxTrans, glm::vec3(30.0f, 0.0f, -4.0f));
	glUniformMatrix4fv(modelTransformLoc, 1, GL_FALSE, glm::value_ptr(boxTrans));


	// define projection matrix
	glm::mat4 projection = glm::mat4(1.0f);
	//glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

	GLuint lightPosLoc = glGetUniformLocation(shaderProgram, "u_light_pos");
	GLuint lightDirLoc = glGetUniformLocation(shaderProgram, "u_light_dir");
	glUniform3f(lightDirLoc, 0.0f, 0.0f, 0.0f);

#pragma endregion

	// set bg color to green
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// var for rdeltaTime
	float currentTime = glfwGetTime();
	float currentTime2 = glfwGetTime();
	float prevTime = 0.0f;
	float deltaTime2 = 0.0f;
	float dT = 1.0f / 60.0f;
	float t = 0.0f;


#pragma region Particle Physics Declarations

	PhysicsWorld pWorld;

	MyVector initialPos = MyVector(0, 0, 0);
	int currType = 1;
	bool isCollide = false;
	float boxRadius = 4;
	float boxMass = 25;
	float cooldown = 1;
	float force = 0;
	float stopper = 0;

#pragma endregion

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
		projection = glm::perspective(glm::radians(90.0f), ratio, 0.1f, 300.0f),
			// Set projection matrix in shader
			glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

#pragma endregion

#pragma region View

		glfwSetCursorPosCallback(window, mouse_callback);
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

		// Keyboard camera controls
		//source: https://www.youtube.com/watch?v=AWM4CUfffos
		float cameraSpeed = deltaTime2 * 10;
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			cameraPos += cameraSpeed * cameraFront;
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			cameraPos -= cameraSpeed * cameraFront;
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
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

#pragma region Box Object

		glBindVertexArray(box.vaoId);
		glUseProgram(shaderProgram);

		glm::mat4 boxNormalTrans = glm::transpose(glm::inverse(boxTrans));
		glUniformMatrix4fv(normalTransformLoc, 1, GL_FALSE, glm::value_ptr(boxNormalTrans));
		glUniformMatrix4fv(modelTransformLoc, 1, GL_FALSE, glm::value_ptr(boxTrans));

		glActiveTexture(GL_TEXTURE0);
		GLuint boxTexture = box.textures[box.materials[0].diffuse_texname];
		glBindTexture(GL_TEXTURE_2D, boxTexture);
		glDrawElements(GL_TRIANGLES, box.numFaces, GL_UNSIGNED_INT, (void*)0);

#pragma endregion

#pragma region Particle Objects

		glBindVertexArray(particle.vaoId);
		glUseProgram(shaderProgram);

		// Change bullet type
		if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) currType = 1;
		if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) currType = 2;
		if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) currType = 3;
		if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) currType = 4;
		if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS) currType = 5;

		currentTime2 = glfwGetTime();
		deltaTime2 = currentTime2 - prevTime;
		prevTime = currentTime2;

		// Fire particle key
		int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);

		// Firing cooldown
		cooldown += cooldown * deltaTime2;
		if (cooldown >= 1.5)
		{
			if (state == GLFW_PRESS) {
				pWorld.addParticle(currType, initialPos);
				cooldown = 1;
			}
		}

		if (!pWorld.particles.empty())
		{
			// Collision detection
			for (int i = 0; i < pWorld.particles.size(); i++)
			{
				if (pWorld.getDistance(pWorld.particleTrans[i][3].x, pWorld.particleTrans[i][3].y, pWorld.particleTrans[i][3].z, boxTrans[3].x, boxTrans[3].y, boxTrans[3].z) <= pWorld.particles[i]->radius + boxRadius) {
					force = (pWorld.particles[i]->mass * pWorld.particles[i]->velocity.x) / boxMass;
					stopper = force;

					if (pWorld.particles[i]->type != 4)
					{
						pWorld.particles[i]->isDestroyed = true;
					}
					cout << endl << endl << "COLLIDED" << endl << endl;
				}
				else if (pWorld.particles[i]->isDestroyed)
				{
					if (pWorld.particles[i]->type == 5)
					{
						for (int j = 0; j < pWorld.particles[i]->count; j++)
						{
							pWorld.addParticle(6, pWorld.particles[i]->position);
						}
						for (int l = 0; l < pWorld.particles[i]->count * 3; l++)
						{
							pWorld.addParticle(7, pWorld.particles[i]->position);
						}
					}
					else if (pWorld.particles[i]->type == 6)
					{
						for (int k = 0; k < pWorld.particles[i]->count; k++)
						{
							pWorld.addParticle(7, pWorld.particles[i]->position);
						}
					}
				}
			}

			// Update physics world
			pWorld.update(deltaTime2);

			// Drawing Particles
			for (int i = 0; i < pWorld.particles.size(); i++) {
				glm::vec3 temp = glm::vec3(pWorld.particles[i]->getVec3Pos());
				pWorld.particleTrans[i] = glm::mat4(1.0f);
				pWorld.particleTrans[i] = glm::scale(pWorld.particleTrans[i], glm::vec3(0.5f, 0.5f, 0.5f));
				pWorld.particleTrans[i] = glm::translate(pWorld.particleTrans[i], temp);

				pWorld.normalTransArray[i] = glm::transpose(glm::inverse(pWorld.particleTrans[i]));
				glUniformMatrix4fv(normalTransformLoc, 1, GL_FALSE, glm::value_ptr(pWorld.normalTransArray[i]));
				glUniformMatrix4fv(modelTransformLoc, 1, GL_FALSE, glm::value_ptr(pWorld.particleTrans[i]));

				glActiveTexture(GL_TEXTURE0);
				GLuint particleTexture = particle.textures[particle.materials[pWorld.particles[i]->material].diffuse_texname];
				glBindTexture(GL_TEXTURE_2D, particleTexture);
				glDrawElements(GL_TRIANGLES, particle.numFaces, GL_UNSIGNED_INT, (void*)0);
			}

			/*
			float newTime = glfwGetTime();
			float frameTime = newTime - currentTime;
			currentTime = newTime;
			while (frameTime > 0.0f)
			{
				float deltaTime = std::min(frameTime, dT);
				frameTime -= deltaTime;
				
			}
			*/
		}
		
		// Move box when hit
		boxTrans = glm::translate(boxTrans, glm::vec3(force, 0.0f, 0.0f) * deltaTime2);
		if (force >= 0) force -= stopper * deltaTime2;
		else force = 0;
		
#pragma endregion

		//unbindtexture after rendering
		glBindTexture(GL_TEXTURE_2D, 0);

		

		//--- stop drawing here ---
#pragma endregion

		glfwSwapBuffers(window);
		//listen for glfw input events
		glfwPollEvents();
	}
	return 0;
}